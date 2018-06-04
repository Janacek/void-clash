/********************************************************************
**
**		Chat.cpp
**		Created by : Joris VIal
**
********************************************************************/

#include "Chat.hpp"
#include "Event.hpp"
#include "GUIManager.hpp"
#include "HudRessources.hpp"
#include "Map.hpp"
#include "GraphicHud.hpp"

// Auto hide chat window
#define CHAT_AUTO_HIDE_TIME 5.0f

// Animxation
#define CHAT_ANIMATION_SPEED 2.0f
#define CHAT_MIN_ALPHA 0.5f

// Message size
#define CHAT_MAX_MESSAGE_SIZE 256

// Define how many messages we will keep in the history
#define CHAT_HISTORY_SIZE 256
#define CHAT_MESSAGES_DISPLAYED 14

/////////////////////////////////////////////////////////////////////
/////	Ctor/Dtor
/////////////////////////////////////////////////////////////////////

Chat::Chat(void)
{
	// Module type
	_type = GUI_CHAT_MENU;
}

Chat::~Chat(void)
{

}

/////////////////////////////////////////////////////////////////////
/////	Start
/////////////////////////////////////////////////////////////////////

void Chat::start(CEGUI::Window *root)
{
	_root = root;
	_ignoreNextFocusRequest = false;

	// Reset active time
	resetActiveTime();

	// Load content and subscribe to events

	addButton("ChatWindow", true);
	addButton("ChatInput", true);
	addButton("ChatHistory", true);
	addButton("ChatScrollbar", true);

	// Set input size
	get("ChatInput")->setFont("Jura-9");

	// Independant alpha for history
	get("ChatHistory")->setInheritsAlpha(false);

	// Clear scrollbar properties
	std::memset(&_scrollbarProperties, 0, sizeof(ChatScrollbarProperties));

	/////// Setup chat history ///////

	ChatMessage welcome;
	welcome.message = " <<<< Historique >>>>";
	welcome.level = -3;
	_messages.push_back(welcome);

	// Version
	welcome.message = "o VoidClash v" + std::to_string(VOID_CLASH_VERSION); welcome.level = 1;
	_messages.push_back(welcome);

	// Welcome
	welcome.message = "o Bienvenue !"; welcome.level = -1;
	_messages.push_back(welcome);

	// Shortcut
	welcome.message = "o {Entr�e} Ouvrir discussion"; welcome.level = -2;
	_messages.push_back(welcome);
	welcome.message = "o {Echap} Fermer discussion - Ouvrir menu principal"; welcome.level = -1;
	_messages.push_back(welcome);
	welcome.message = "o {N} Choisir �quipe"; welcome.level = -2;
	_messages.push_back(welcome);
	welcome.message = "o {B} Choisir arme"; welcome.level = -1;
	_messages.push_back(welcome);
	welcome.message = "o {F2} Quitter rapidement le jeu"; welcome.level = -1;
	_messages.push_back(welcome);

	// Commands
	welcome.message = "o Tapper '/nick XXX' pour changer votre pseudo"; welcome.level = -2;
	_messages.push_back(welcome);

	// Refresh the chat
	_needRefresh = true;

	/////// Setup scrollbar ///////

	_scrollTimeout = 0.0f;

	_scrollbarProperties.documentSize = 100.0f;
	_scrollbarProperties.stepSize = 1.0f;
	_scrollbarProperties.position = 100.0f;
	_scrollbarProperties.pageSize = 10.0f;

	CEGUI::Scrollbar *scroll = static_cast<CEGUI::Scrollbar*>(get("ChatScrollbar"));
	if (scroll != NULL)
	{
		scroll->setDocumentSize(_scrollbarProperties.documentSize);
		scroll->setStepSize(_scrollbarProperties.stepSize);
		scroll->setScrollPosition(_scrollbarProperties.position);
	}
}

/////////////////////////////////////////////////////////////////////
/////	Update
/////////////////////////////////////////////////////////////////////

void Chat::update(float deltatime)
{
	// Update animation
	if (_animationType != CHAT_FADE_NONE)
		updateAnimation(deltatime);

	// Update active time
	if (get("ChatWindow")->isVisible() == true && isInputEnabled() == false)
		updateActiveTime(deltatime);

	// Update the timeout
	if (_scrollTimeout > 0.0f)
		_scrollTimeout += deltatime;

	// Check for new messages
	checkNewMessages();

	// Display messages according to scroll position
	refreshMessages();
}

/////////////////////////////////////////////////////////////////////
/////	Enable / Disable
/////////////////////////////////////////////////////////////////////

void Chat::setActive(bool active, bool autofocus)
{
	(void)autofocus;

	if (active == true)
	{
		setVisible(true);
		setChatInputFocus(true);
	}
	else
	{
		setChatInputFocus(false);
	}

	_active = active;

	// Reset active time
	resetActiveTime();

	// Clear chat input
	resetInputText();
}

/////////////////////////////////////////////////////////////////////
/////	Show/Hide chat before/after fade in/out
/////////////////////////////////////////////////////////////////////

void Chat::setVisible(bool visible)
{
	for (auto it = _windows.begin(); it != _windows.end(); ++it)
	{
		if (it->second == NULL)
		{
			VC_WARNING("Chat::setActive, " +
				it->first + " doesn't exist. Check your code strings");
		}
		else
		{
			it->second->setVisible(visible);
		}
	}

	// Clear chat input
	resetInputText();
}

/////////////////////////////////////////////////////////////////////
/////	Callbacks
/////////////////////////////////////////////////////////////////////

bool Chat::onClick(const CEGUI::EventArgs &eIN)
{
	(void)eIN;
	return false;
}

bool Chat::onEnterArea(const CEGUI::EventArgs &eIN)
{
	(void)eIN;
	return true;
}

bool Chat::onLeaveArea(const CEGUI::EventArgs &eIN)
{
	(void)eIN;
	return true;
}

bool Chat::onTextAccepted(const CEGUI::EventArgs &eIN)
{
	// Get the window which fired the event <3
	const CEGUI::WindowEventArgs& e =
		static_cast<const CEGUI::WindowEventArgs&>(eIN);

	VC_INFO_CRITICAL("Chat::onTextAccepted, trigger: " + getWindowName(e.window));

	// Get chat box
	CEGUI::Editbox *chatBox = dynamic_cast<CEGUI::Editbox *>(get("ChatInput"));
	if (chatBox == NULL)
		return true;

	// Convert this string
	char str[CHAT_MAX_MESSAGE_SIZE + 1]; // + 1 Because Gegon need a '\0' to feel good <3
	memset(str, 0, CHAT_MAX_MESSAGE_SIZE + 1); // + 1 Because Gegon need a '\0' to feel good <3

	for (unsigned int i = 0; i < chatBox->getText().size() && i < CHAT_MAX_MESSAGE_SIZE; ++i)
		str[i] = chatBox->getText().at(i);

	// Just to be sure...
	str[CHAT_MAX_MESSAGE_SIZE] = '\0';

	std::string *toto = new std::string(str);

	// Send event
	S_GUI->addDelayedEvent(ev_CHAT_TO_SEND,
		s_event(NULL, toto));

	// Clear chat input
	resetInputText();

	// Refresh
	_needRefresh = true;

	// Reset active time
	resetActiveTime();

	// Close chat :)
	setChatInputFocus(false);

	_ignoreNextFocusRequest = true;

	return true;
}

bool Chat::onKeyReleased(const CEGUI::EventArgs &eIN)
{
	// Get the key event <3
	const CEGUI::KeyEventArgs& e =
		static_cast<const CEGUI::KeyEventArgs&>(eIN);

	VC_INFO_CRITICAL("Chat::onKeyReleased, trigger: " + getWindowName(e.window));

	return true;
}

bool Chat::onThumbTrackEnded(const CEGUI::EventArgs &eIN)
{
	// Get the window which fired the event <3
	const CEGUI::WindowEventArgs& e =
		static_cast<const CEGUI::WindowEventArgs&>(eIN);

	VC_INFO_CRITICAL("Chat::onThumbTrackEnded, trigger: " + getWindowName(e.window));

	if (e.window == get("ChatScrollbar"))
	{
		// Re focus the chat input
		get("ChatInput")->activate();
	}

	return true;
}

bool Chat::onMouseWheel(const CEGUI::EventArgs &eIN)
{
	// Get mouse event <3
	const CEGUI::MouseEventArgs& e =
		static_cast<const CEGUI::MouseEventArgs&>(eIN);

	VC_INFO_CRITICAL("Chat::onMouseWheel, trigger: " + getWindowName(e.window));

	// Get the scrollbard
	CEGUI::Scrollbar *scroll = static_cast<CEGUI::Scrollbar*>(get("ChatScrollbar"));
	if (scroll == NULL)
		return true;

	// Position
	_scrollbarProperties.position = std::max(0.0f, _scrollbarProperties.position - e.wheelChange);
	_scrollbarProperties.position = std::min(100.0f, _scrollbarProperties.position);

	// Update scrollbar
	scroll->setScrollPosition(_scrollbarProperties.position);

	// Start the timeout
	_scrollTimeout = 0.01f;

	// Refresh the chat
	_needRefresh = true;

	// Reset active time
	resetActiveTime();

	return true;
}

bool Chat::onEventScrollPositionChanged(const CEGUI::EventArgs &eIN)
{
	// Get the window which fired the event <3
	const CEGUI::WindowEventArgs& e =
		static_cast<const CEGUI::WindowEventArgs&>(eIN);

	VC_INFO_CRITICAL("Chat::onEventScrollPositionChanged, trigger: " + getWindowName(e.window));

	// Get the scrollbar
	CEGUI::Scrollbar *scroll = static_cast<CEGUI::Scrollbar*>(get("ChatScrollbar"));
	if (scroll == NULL)
		return true;

	// Position
	_scrollbarProperties.position = std::max(0.0f, scroll->getScrollPosition());
	_scrollbarProperties.position = std::min(100.0f, _scrollbarProperties.position);

	// Update scrollbar
	scroll->setScrollPosition(_scrollbarProperties.position);

	// Refresh the chat
	_needRefresh = true;

	// Reset active time
	resetActiveTime();

	return true;
}

/////////////////////////////////////////////////////////////////////
/////	Get chat input focus state
/////////////////////////////////////////////////////////////////////

void Chat::setChatInputFocus(bool focused, bool endOfChat)
{
	if (_ignoreNextFocusRequest == true)
	{
		_ignoreNextFocusRequest = false;
		return;
	}

	// User wants to use the chat
	if (focused == true)
	{
		startAnimation(CHAT_FADE_IN);
		setVisible(true);
		get("ChatInput")->activate();
		get("ChatHistory")->setSize(CEGUI::USize(CEGUI::UDim(1.0f, 0.0f),
			CEGUI::UDim(0.82, 0.0f))); // This value needs
		// to match the value size in the XML file
	}
	// User quit the chat (or chat is displayed when we launch the game)
	else if (endOfChat == false)
	{
		startAnimation(CHAT_FADE_IN);
		setVisible(true);

		get("ChatInput")->deactivate();
		get("ChatHistory")->setSize(CEGUI::USize(CEGUI::UDim(1.0f, 0.0f),
			CEGUI::UDim(1.0f, 0.0f)));

		// Scroll to the bottom
		CEGUI::Scrollbar *scroll = static_cast<CEGUI::Scrollbar*>(get("ChatScrollbar"));
		_scrollbarProperties.position = 100;
		scroll->setScrollPosition(100);
		_scrollTimeout = 0.0f;
	}

	// Chat is closing himself after timing out, animation CHAT_FADE_OUT already done
	else if (endOfChat == true)
	{
		startAnimation(CHAT_FADE_OUT);
		get("ChatInput")->deactivate();
	}

	// Refresh the chat
	_needRefresh = true;

	// Refresh the message list
	refreshMessages();
}

/////////////////////////////////////////////////////////////////////
/////	Animation transition
/////////////////////////////////////////////////////////////////////

void Chat::startAnimation(eChatAnimationType type)
{
	_animationType = type;

	if (type == CHAT_FADE_IN)
		_smooth = 0.0f;
	else
		_smooth = 1.0f;
}

void Chat::updateAnimation(float deltatime)
{
	/////// Update animation ///////

	// Fade in
	if (_animationType == CHAT_FADE_IN)
	{
		_smooth = std::min(1.0f, _smooth + deltatime * CHAT_ANIMATION_SPEED);

		// Update alpha
		get("ChatWindow")->setAlpha(_smooth);

		// Input focus, animation "ChatInput"
		if (isInputEnabled() == true)
		{
			get("ChatHistory")->setAlpha(std::min(1.0f, _smooth));
			get("ChatInput")->setAlpha(_smooth);
			get("ChatScrollbar")->setAlpha(_smooth);
		}
		// No input focus, hide "ChatInput"
		else
		{
			get("ChatHistory")->setAlpha(std::min(CHAT_MIN_ALPHA, _smooth));
			get("ChatInput")->setAlpha(0.0f);
			get("ChatScrollbar")->setAlpha(0.0f);
		}

		if (_smooth == 1.0f)
			_animationType = CHAT_FADE_NONE;
	}

	// Fade out (only used when the chat closed himself)
	else // CHAT_FADE_OUT
	{
		_smooth = std::max(0.0f, _smooth - deltatime * CHAT_ANIMATION_SPEED);

		get("ChatWindow")->setAlpha(_smooth);
		get("ChatHistory")->setAlpha(std::min(CHAT_MIN_ALPHA, _smooth));
		get("ChatInput")->setAlpha(0.0f);
		get("ChatScrollbar")->setAlpha(0.0f);

		// Fade out done, disable chat
		if (_smooth == 0.0f)
			setVisible(false);
	}
}

/////////////////////////////////////////////////////////////////////
/////	Get chat input focus state
/////////////////////////////////////////////////////////////////////

bool Chat::isInputEnabled(void)
{
	return get("ChatInput")->isActive();
}

/////////////////////////////////////////////////////////////////////
/////	Print a message
/////////////////////////////////////////////////////////////////////

void Chat::printMessage(const ChatMessage &message)
{
	// Get the chat history
	CEGUI::Window *history = get("ChatHistory");
	if (history == NULL)
		return;

	// Format the string
	std::string format("");

	// Get the nickname
	std::string nick("");
	if (message.owner != NULL)
		nick = message.owner->getNick() + ": ";

	// Get the color for the nickname
	std::string nickColor("");
	if (message.owner != NULL)
		nickColor = getTeamColor(message.owner->getTeam());

	// Server
	if (message.owner == NULL)
		nickColor = getServerColor(message.level);

	// Get the color for the message
	std::string messageColor("");

	// Player
	messageColor = "[colour='FFFFFFFF']";

	// Server
	if (message.owner == NULL)
		messageColor = nickColor;

	// Debug
	std::string finalDebug(nickColor + nick + messageColor + message.message.c_str());

	// Final string
	CEGUI::String final(nickColor + nick + messageColor + message.message + "\n");
	history->appendText(final);
}

/////////////////////////////////////////////////////////////////////
/////	Print messages in the chat history
/////////////////////////////////////////////////////////////////////

void Chat::refreshMessages(void)
{
	if (_needRefresh == false)
		return;

	// Clear the chat history
	CEGUI::Window *history = get("ChatHistory");
	if (history == NULL)
		return;
	history->setText("[font = 'Jura-8']");

	// Get the scroll position
	CEGUI::Scrollbar *scroll = static_cast<CEGUI::Scrollbar*>(get("ChatScrollbar"));
	if (scroll == NULL)
		return;

	float scrollPosition = scroll->getScrollPosition() * 0.01f;
	int last = (_messages.size() - 1) * scrollPosition;

	for (int i = CHAT_MESSAGES_DISPLAYED; i >= 0; --i)
	{
		if (last - i >= 0)
			printMessage(_messages[last - i]);
	}

	// Refresh done
	_needRefresh = false;
}

/////////////////////////////////////////////////////////////////////
/////	Check for new messages
/////////////////////////////////////////////////////////////////////

void Chat::checkNewMessages(void)
{
	// Get new messages
	auto ev = Event::getEventByType(ev_CHAT_MSG_RECEIVED);
	if (ev == NULL)
		return;

	// Get the message handler
	t_msgHandler *msg = (t_msgHandler*)(ev->begin()->second.data);

	// Create a new message
	ChatMessage newMessage;

	newMessage.owner = std::dynamic_pointer_cast<Player>(S_Map->findPlayerWithID(msg->_id));
	newMessage.message = CEGUI::String(msg->_msg);
	newMessage.level = msg->_level;

	// If we don't have any slot, delete the first one (the older one)
	if (_messages.size() >= CHAT_HISTORY_SIZE)
		_messages.erase(_messages.begin());

	// Save the new message
	_messages.push_back(newMessage);

	// Delete the message handler
	delete msg;

	// Refresh the chat history
	_needRefresh = true;

	// Reset active time
	resetActiveTime();

	// Open the chat window if it's not already opened
	// and disable the input focus
	if (get("ChatWindow")->isVisible() == false)
		setChatInputFocus(false);

	// Get the scrollbar
	CEGUI::Scrollbar *scroll = static_cast<CEGUI::Scrollbar*>(get("ChatScrollbar"));
	if (scroll == NULL)
		return;

	// Scroll to the bottom if the player didn't scroll for 10 seconds
	if (_scrollTimeout >= 10)
	{
		_scrollbarProperties.position = 100;
		scroll->setScrollPosition(100);
		_scrollTimeout = 0.0f;
	}
}

/////////////////////////////////////////////////////////////////////
/////	Auto hide management
/////////////////////////////////////////////////////////////////////

void Chat::resetActiveTime(void)
{
	_activeTime = 0.0f;
}

void Chat::updateActiveTime(float deltatime)
{
	// Update active time
	_activeTime += deltatime;

	// Hide automaticaly the chat if we are out of time
	if (_activeTime > CHAT_AUTO_HIDE_TIME - (1.0f / CHAT_ANIMATION_SPEED) &&
		_animationType == CHAT_FADE_NONE)
	{
		setChatInputFocus(false, true);
		_activeTime = 0.0f;
	}
}

/////////////////////////////////////////////////////////////////////
/////	Generate the string color according to team
/////////////////////////////////////////////////////////////////////

std::string Chat::getTeamColor(unsigned int team)
{
	switch (team)
	{
		// Green (team 1)
	case 1: return "[colour = 'FF00FF00']";
		// Green (team 1)
	case 2: return "[colour = 'FFFF0000']";
		// Yellow (spectator)
	default: return "[colour = 'FFFFFF00']";
	}
}

std::string Chat::getServerColor(char level)
{
	switch (level)
	{
		// Error (red)
	case 0: return "[colour='FFFF0000']";
		// Success (green)
	case 1: return "[colour='FF00FF00']";
		// Info (cyan)
	case 2: return "[colour='FF00FFFF']";
		// Yellow
	case -1: return "[colour='FFFFFF00']";
		// Orange
	case -2: return "[colour='FFFF9A00']";
		// Default
	case -3: return "[colour='FF0000FF']";
		// Default
	default: return "[colour = 'FFFFFFFF']";
	}
}

/////////////////////////////////////////////////////////////////////
/////	Reset the input text area
/////////////////////////////////////////////////////////////////////

void Chat::resetInputText(void)
{
	CEGUI::Editbox *chatBox = dynamic_cast<CEGUI::Editbox *>(get("ChatInput"));
	if (chatBox == NULL)
		return;

	chatBox->setText(CEGUI::String(""));
}

/////////////////////////////////////////////////////////////////////
/////	Add button and bind callbacks (could be templated?)
/////////////////////////////////////////////////////////////////////

void Chat::addButton(const std::string &name, bool hasOver)
{
	CEGUI::Window *w = addWindow(name);

	// Mouse hovered
	if (hasOver == true)
	{
		w->subscribeEvent(
			CEGUI::Window::EventMouseEntersArea,
			CEGUI::Event::Subscriber(&Chat::onEnterArea, this));

		w->subscribeEvent(
			CEGUI::Window::EventMouseLeavesArea,
			CEGUI::Event::Subscriber(&Chat::onLeaveArea, this));
	}

	// Mouse click
	w->subscribeEvent(
		CEGUI::Window::EventMouseClick,
		CEGUI::Event::Subscriber(&Chat::onClick, this));

	// Mouse release on scrollbar
	w->subscribeEvent(
		CEGUI::Scrollbar::EventThumbTrackEnded,
		CEGUI::Event::Subscriber(&Chat::onThumbTrackEnded, this));

	// Key released
	w->subscribeEvent(
		CEGUI::Window::EventKeyUp,
		CEGUI::Event::Subscriber(&Chat::onKeyReleased, this));

	// Specific event for "ChatInput"
	w->subscribeEvent(
		CEGUI::Editbox::EventTextAccepted,
		CEGUI::Event::Subscriber(&Chat::onTextAccepted, this));

	// Specific event for "ChatScrollbar"
	w->subscribeEvent(
		CEGUI::Scrollbar::EventMouseWheel,
		CEGUI::Event::Subscriber(&Chat::onMouseWheel, this));

	// Specific event for "ChatScrollbar"
	w->subscribeEvent(
		CEGUI::Scrollbar::EventScrollPositionChanged,
		CEGUI::Event::Subscriber(&Chat::onEventScrollPositionChanged, this));
}

/////////////////////////////////////////////////////////////////////
/////	Stop
/////////////////////////////////////////////////////////////////////

void Chat::stop(void)
{
	// Clear messages history
	_messages.clear();
}

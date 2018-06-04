/********************************************************************
**
**		Chat.hpp
**		Created by : Joris VIal
**
********************************************************************/

#ifndef CHAT_MENU
#define CHAT_MENU

#include "Player.hpp"
#include "GUIEntity.hpp"

// Message container
struct ChatMessage
{
	// Default value
	ChatMessage(void)
	{
		owner = NULL;
		message = "";

		// Define the color of the message (used for welcome and server messages)
		// 0 -> Red (server)
		// 1 -> Green (server)
		// 2 -> Cyan (server)
		// -1 -> Yellow
		// -2 -> Orange
		// -3 -> Blue
		level = -1;
	}

	// Player who sent the message
	std::shared_ptr<Player> owner;

	// The message itself
	CEGUI::String message;
	
	// Only for server messages
	char level;
};

// Scrollbar properties
struct ChatScrollbarProperties
{
	float documentSize;
	float pageSize;
	float stepSize;
	float overlapSize;
	float position;
};

// Animation type
enum eChatAnimationType
{
	CHAT_FADE_IN,
	CHAT_FADE_OUT,
	CHAT_FADE_NONE
};

class Chat : public GUIEntity
{
public:
	Chat(void);
	virtual ~Chat(void);

	virtual void start(CEGUI::Window *root);
	virtual void update(float deltatime);
	virtual void stop(void);

	// Status management
	virtual void setActive(bool active, bool autofocus = true); // overrided

	// Enable / Disable chat focus
	void setChatInputFocus(bool focused, bool endOfChat = false);
	bool isInputEnabled(void);

private:
	// Add button and bind callbacks(could be templated ? )
	void addButton(const std::string &normal, bool hasHover = false);

	// Messages management
	void checkNewMessages(void);

	// Fill the history according the scroll position
	void refreshMessages(void);
	void printMessage(const ChatMessage &message);
	void refreshScrollbar(void);

	// Generate the color
	std::string getTeamColor(unsigned int team);
	std::string getServerColor(char level);

	// Manage auto hide
	void resetActiveTime(void);
	void updateActiveTime(float deltatime);

	// Fade transition
	void startAnimation(eChatAnimationType type);
	void updateAnimation(float deltatime);
	void setVisible(bool visible);

	// Reset the input text area
	void resetInputText(void);

private:
	// Callbacks
	bool onClick(const CEGUI::EventArgs &eIN);
	bool onEnterArea(const CEGUI::EventArgs &eIN);
	bool onLeaveArea(const CEGUI::EventArgs &eIN);
	bool onTextAccepted(const CEGUI::EventArgs &eIN);
	bool onKeyReleased(const CEGUI::EventArgs &eIN);
	bool onMouseWheel(const CEGUI::EventArgs &eIN);
	bool onEventScrollPositionChanged(const CEGUI::EventArgs &eIN);
	bool onThumbTrackEnded(const CEGUI::EventArgs &eIN);

private:
	bool _ignoreNextFocusRequest;

	// Check if we need to refill the history
	bool _needRefresh;

	// Scroll to bottom on new message if the player didn't scroll for 10 seconds
	float _scrollTimeout;

	// Message container
	std::vector<ChatMessage> _messages;

	// Scrollbar attribs
	ChatScrollbarProperties _scrollbarProperties;

	// Auto hide the chat
	float _activeTime;

	// Fade transition
	float _smooth;
	eChatAnimationType _animationType;
};

#endif 
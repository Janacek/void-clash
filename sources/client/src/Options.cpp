/***************************-*****************************************
**
**		Options.cpp
**		Created by : Joris VIal
**
********************************************************************/

#include "Options.hpp"
#include "GUIManager.hpp"

/////////////////////////////////////////////////////////////////////
/////	Ctor/Dtor
/////////////////////////////////////////////////////////////////////

Options::Options(void)
{
	_type = GUI_OPTIONS_MENU;
}

Options::~Options(void)
{

}

/////////////////////////////////////////////////////////////////////
/////	Start
/////////////////////////////////////////////////////////////////////

void Options::start(CEGUI::Window *root)
{
	_root = root;

	// Load content and subscribe to events

	addButton("OptionsWindow");
	addButton("OptionsGraphicButton", true);
	addButton("OptionsSoundButton", true);
	addButton("OptionsKeysButton", true);
	addButton("OptionsNickButton", true);
	addButton("OptionsNickEditbox", true);
	addButton("OptionsValidate", true);

	get("OptionsNickEditbox")->setVisible(false);
	get("OptionsNickButton")->setVisible(true);
}

/////////////////////////////////////////////////////////////////////
/////	Update
/////////////////////////////////////////////////////////////////////

void Options::update(float deltatime)
{
	(void)deltatime;
}

/////////////////////////////////////////////////////////////////////
/////	Enable / Disable
/////////////////////////////////////////////////////////////////////

void Options::setActive(bool active, bool autofocus)
{
	(void)autofocus;

	GUIEntity::setActive(active);

	// Reset the nick button
	get("OptionsNickEditbox")->setVisible(false);
	get("OptionsNickButton")->setVisible(true);
}

/////////////////////////////////////////////////////////////////////
/////	Callbacks
/////////////////////////////////////////////////////////////////////

bool Options::onClick(const CEGUI::EventArgs &eIN)
{
	// Get the window which fired the event <3
	const CEGUI::WindowEventArgs& e =
		static_cast<const CEGUI::WindowEventArgs&>(eIN);

	VC_INFO_CRITICAL("Options::onClick, trigger: " + getWindowName(e.window));

	// Enable nickname edit box
	if (e.window == get("OptionsNickButton"))
	{
		get("OptionsNickEditbox")->setVisible(true);
		get("OptionsNickEditbox")->activate();
		get("OptionsNickButton")->setVisible(false);
	}

	// "OK" button
	else if (e.window == get("OptionsValidate"))
	{
		// TODO - Save Options
		S_GUI->disableTopModule();
	}

	// Graphic options
	else if (e.window == get("OptionsGraphicButton"))
		S_GUI->enableModule(GUI_OPTIONS_GRAPHIC);

	// Keys options
	else if (e.window == get("OptionsKeysButton"))
		S_GUI->enableModule(GUI_OPTIONS_KEYS);

	// Sound Options
	else if (e.window == get("OptionsSoundButton"))
		S_GUI->enableModule(GUI_OPTIONS_SOUND);

	return true;
}

bool Options::onEnterArea(const CEGUI::EventArgs &eIN)
{
	(void)eIN;
	return true;
}

bool Options::onLeaveArea(const CEGUI::EventArgs &eIN)
{
	(void)eIN;
	return true;
}

bool Options::onKeyReleased(const CEGUI::EventArgs &eIN)
{
	(void)eIN;
	return true;
}

bool Options::onTextAccepted(const CEGUI::EventArgs &eIN)
{
	// Get the window which fired the event <3
	const CEGUI::WindowEventArgs& e =
		static_cast<const CEGUI::WindowEventArgs&>(eIN);

	VC_INFO_CRITICAL("Chat::onTextAccepted, trigger: " + getWindowName(e.window));

	// Get chat box
	CEGUI::Editbox *nickBox =
		dynamic_cast<CEGUI::Editbox *>(get("OptionsNickEditbox"));
	if (nickBox == NULL)
		return true;

	// Convert string
	char str[11];
	memset(str, 0, 11);

	for (unsigned int i = 0; i < nickBox->getText().size() && i < 11; ++i)
		str[i] = nickBox->getText().at(i);

	// Just to be sure...
	str[10] = '\0';

	std::string *toto = new std::string(str);

	// Send event
	S_GUI->addDelayedEvent(ev_NICKNAME_CHANGED,
		s_event(NULL, toto));

	// Clear chatBox
	nickBox->setText(CEGUI::String(""));

	// Enable again the nick button
	get("OptionsNickEditbox")->setVisible(false);
	get("OptionsNickButton")->setVisible(true);

	return true;
}

/////////////////////////////////////////////////////////////////////
/////	Add button and bind callbacks (could be templated?)
/////////////////////////////////////////////////////////////////////

void Options::addButton(const std::string &name, bool hasHover)
{
	CEGUI::Window *w = addWindow(name);

	// Mouse hovered
	if (hasHover == true)
	{
		w->subscribeEvent(
			CEGUI::Window::EventMouseEntersArea,
			CEGUI::Event::Subscriber(&Options::onEnterArea, this));

		w->subscribeEvent(
			CEGUI::Window::EventMouseLeavesArea,
			CEGUI::Event::Subscriber(&Options::onLeaveArea, this));
	}

	// Mouse click
	w->subscribeEvent(
		CEGUI::PushButton::EventMouseClick,
		CEGUI::Event::Subscriber(&Options::onClick, this));

	// Key released
	w->subscribeEvent(
		CEGUI::Window::EventKeyUp,
		CEGUI::Event::Subscriber(&Options::onKeyReleased, this));

	// Specific event for "NicknameEditbox"
	w->subscribeEvent(
		CEGUI::Editbox::EventTextAccepted,
		CEGUI::Event::Subscriber(&Options::onTextAccepted, this));
}

/////////////////////////////////////////////////////////////////////
/////	Stop
/////////////////////////////////////////////////////////////////////

void Options::stop(void)
{
	GUIEntity::stop();
}

/********************************************************************
**
**		KeyPreferences.cpp
**		Created by : Joris VIal
**
********************************************************************/

#include "KeyPreferences.hpp"
#include "UserKeys.hpp"
#include "AssetPath.h"
#include "GUIManager.hpp"

/////////////////////////////////////////////////////////////////////
/////	Ctor/Dtor
/////////////////////////////////////////////////////////////////////

KeyPreferences::KeyPreferences(void)
{
	_type = GUI_OPTIONS_KEYS;
}

KeyPreferences::~KeyPreferences(void)
{

}

/////////////////////////////////////////////////////////////////////
/////	Start
/////////////////////////////////////////////////////////////////////

void KeyPreferences::start(CEGUI::Window *root)
{
	// Load custom keys
	S_Keys->setConfigFilePath(ASSETS_PATH + "UserKeys.xml");
	S_Keys->loadConfig();

	_root = root;
	_changingKey = false;
	_keyToChange = NULL;

	// Load content and subscribe to events

	addButton("KeysSelectionWindow");
	addButton("KeysListbox", true);
	addButton("ValidateKeyConfigurationButton", true);

	// Setup key list
	setupKeyList();
}

/////////////////////////////////////////////////////////////////////
/////	Setup key list
/////////////////////////////////////////////////////////////////////

void KeyPreferences::setupKeyList(void)
{
	// Get list box
	CEGUI::Listbox *box =
		static_cast<CEGUI::Listbox *>(get("KeysListbox"));

	// Set entries
	std::string action("");
	std::string bind("");

	for (int i = 0; i < ACTION_MAX; ++i)
	{
		// Get strings
		action = S_Keys->actionToString((UserAction)i);
		bind = S_Keys->actionToKeyString((UserAction)i);

		// Add new list item
		CEGUI::ListboxTextItem *entry = new CEGUI::ListboxTextItem(action, i);
		float uSize = entry->getPixelSize().d_width;
		entry->setText(action + "[padding='l:" + std::to_string(300 - uSize) +
			"t:0 r : 0 b : 0']" + bind);

		entry->setSelectionBrushImage("GlossySerpentFHD/MultiListSelectionBrush");

		box->addItem(entry);
	}
}

/////////////////////////////////////////////////////////////////////
/////	Update
/////////////////////////////////////////////////////////////////////

void KeyPreferences::update(float deltatime)
{
	(void)deltatime;
}

/////////////////////////////////////////////////////////////////////
/////	Callbacks
/////////////////////////////////////////////////////////////////////

bool KeyPreferences::onClick(const CEGUI::EventArgs &eIN)
{
	// Get the window which fired the event <3
	const CEGUI::WindowEventArgs& e =
		static_cast<const CEGUI::WindowEventArgs&>(eIN);

	VC_INFO_CRITICAL("KeyPreferences::onClick, trigger: " + getWindowName(e.window));

	// Save config
	if (e.window == get("ValidateKeyConfigurationButton"))
	{
		S_Keys->saveConfig();
		S_GUI->disableTopModule();
	}

	return false;
}

bool KeyPreferences::onEventSelectionChanged(const CEGUI::EventArgs &eIN)
{
	// Get the window which fired the event <3
	const CEGUI::WindowEventArgs& e =
		static_cast<const CEGUI::WindowEventArgs&>(eIN);

	VC_INFO_CRITICAL("KeyPreferences::onEventSelectionChanged, trigger: " + getWindowName(e.window));

	// Get the list box
	if (e.window == get("KeysListbox") && _changingKey == false)
	{
		// Get the selected item
		CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(e.window);
		CEGUI::ListboxTextItem *item = (CEGUI::ListboxTextItem *)box->getFirstSelectedItem();

		if (item == NULL)
			return false;

		// Ignore events
		_ingoreNextKeyUp = true;

		// Change mode
		_changingKey = true;
		_keyToChange = item;

		// Update string
		updateText(item, "Press a key or button");

		return true;
	}

	return false;
}

bool KeyPreferences::onCharKey(const CEGUI::EventArgs &eIN)
{
	// Get the window which fired the event <3
	const CEGUI::KeyEventArgs& e =
		static_cast<const CEGUI::KeyEventArgs&>(eIN);

	VC_INFO_CRITICAL("KeyPreferences::onCharKey, trigger: " + getWindowName(e.window));

	// Currently changing key
	if (_changingKey == true)
	{
		changeKey(_keyToChange, S_Keys->keyToString(S_Keys->getCustomKeyPressed()));
		return true;
	}

	return false;
}

bool KeyPreferences::onKeyReleased(const CEGUI::EventArgs &eIN)
{
	// Get the window which fired the event <3
	const CEGUI::KeyEventArgs& e =
		static_cast<const CEGUI::KeyEventArgs&>(eIN);

	VC_INFO_CRITICAL("KeyPreferences::onKeyReleased, trigger: " + getWindowName(e.window));

	// Currently changing key
	if (_changingKey == true)
	{
		changeKey(_keyToChange, S_Keys->keyToString(S_Keys->getCustomKeyPressed()));
		return true;
	}

	return false;
}

bool KeyPreferences::onButtonReleased(const CEGUI::EventArgs &eIN)
{
	if (_ingoreNextKeyUp == true)
	{
		_ingoreNextKeyUp = false;
		return false;
	}

	// Get the window which fired the event <3
	const CEGUI::MouseEventArgs& e =
		static_cast<const CEGUI::MouseEventArgs&>(eIN);

	VC_INFO_CRITICAL("KeyPreferences::onButtonReleased, trigger: " + getWindowName(e.window));

	// Currently changing key
	if (_changingKey == true)
	{
		changeKey(_keyToChange, S_Keys->keyToString(S_Keys->getCustomKeyPressed()));
		return true;
	}

	return false;
}

bool KeyPreferences::onEnterArea(const CEGUI::EventArgs &eIN)
{
	(void)eIN;
	return true;
}

bool KeyPreferences::onLeaveArea(const CEGUI::EventArgs &eIN)
{
	(void)eIN;
	return true;
}

void KeyPreferences::changeKey(CEGUI::ListboxTextItem *item, std::string text)
{
	// Update GUI
	_changingKey = false;
	updateText(item, text);
	refreshListBox();

	// Update S_Keys
	S_Keys->setNewBind((UserAction)item->getID(), S_Keys->getCustomKeyPressed());
}

/////////////////////////////////////////////////////////////////////
/////	Update a item text
/////////////////////////////////////////////////////////////////////

void KeyPreferences::updateText(CEGUI::ListboxTextItem *item, std::string text)
{
	CEGUI::String current(item->getText());
	CEGUI::String::size_type lastSpace = current.find_first_of("]") + 1;

	current = current.substr(0, lastSpace);
	item->setText(current + text);
}

void KeyPreferences::refreshListBox(void)
{
	CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(get("KeysListbox"));
	box->handleUpdatedItemData();
}

/////////////////////////////////////////////////////////////////////
/////	Add button and bind callback
/////////////////////////////////////////////////////////////////////

void KeyPreferences::addButton(const std::string &name, bool hasHover)
{
	CEGUI::Window *w = addWindow(name);

	// Mouse hovered
	if (hasHover == true)
	{
		w->subscribeEvent(
			CEGUI::Window::EventMouseEntersArea,
			CEGUI::Event::Subscriber(&KeyPreferences::onEnterArea, this));

		w->subscribeEvent(
			CEGUI::Window::EventMouseLeavesArea,
			CEGUI::Event::Subscriber(&KeyPreferences::onLeaveArea, this));
	}

	// Mouse click
	w->subscribeEvent(
		CEGUI::PushButton::EventMouseClick,
		CEGUI::Event::Subscriber(&KeyPreferences::onClick, this));

	// Key released
	w->subscribeEvent(
		CEGUI::Window::EventKeyUp,
		CEGUI::Event::Subscriber(&KeyPreferences::onKeyReleased, this));

	// EventCharacterKey
	w->subscribeEvent(
		CEGUI::Window::EventCharacterKey,
		CEGUI::Event::Subscriber(&KeyPreferences::onCharKey, this));

	// Key button released
	w->subscribeEvent(
		CEGUI::Window::EventMouseButtonUp,
		CEGUI::Event::Subscriber(&KeyPreferences::onButtonReleased, this));

	// Selection changed
	w->subscribeEvent(
		CEGUI::Listbox::EventSelectionChanged,
		CEGUI::Event::Subscriber(&KeyPreferences::onEventSelectionChanged, this));
}

/////////////////////////////////////////////////////////////////////
/////	Stop
/////////////////////////////////////////////////////////////////////

void KeyPreferences::stop(void)
{
	GUIEntity::stop();
}

/********************************************************************
**
**		SoundPreferences.cpp
**		Created by : Joris VIal
**
********************************************************************/

#include "SoundPreferences.hpp"
#include "GUIManager.hpp"

// Globals
extern int volume;

/////////////////////////////////////////////////////////////////////
/////	Ctor/Dtor
/////////////////////////////////////////////////////////////////////

SoundPreferences::SoundPreferences(void)
{
	_type = GUI_OPTIONS_SOUND;
}

SoundPreferences::~SoundPreferences(void)
{

}

/////////////////////////////////////////////////////////////////////
/////	Start
/////////////////////////////////////////////////////////////////////

void SoundPreferences::start(CEGUI::Window *root)
{
	_root = root;

	// Load content and subscribe to events

	addButton("SoundPreferences");
	addButton("SoundVolumeScrollbar", true);
	addButton("SoundMuteCheckbox", true);
	addButton("SoundValidate", true);
}

/////////////////////////////////////////////////////////////////////
/////	Update
/////////////////////////////////////////////////////////////////////

void SoundPreferences::update(float deltatime)
{
	(void)deltatime;
}

/////////////////////////////////////////////////////////////////////
/////	Callbacks
/////////////////////////////////////////////////////////////////////

bool SoundPreferences::onClick(const CEGUI::EventArgs &eIN)
{
	// Get the window which fired the event <3
	const CEGUI::WindowEventArgs& e =
		static_cast<const CEGUI::WindowEventArgs&>(eIN);

	if (e.window == get("SoundValidate"))
		S_GUI->disableTopModule();

	return false;
}

bool SoundPreferences::onEnterArea(const CEGUI::EventArgs &eIN)
{
	(void)eIN;
	return false;
}

bool SoundPreferences::onLeaveArea(const CEGUI::EventArgs &eIN)
{
	(void)eIN;
	return false;
}

bool SoundPreferences::onEventCheckStateChanged(const CEGUI::EventArgs &eIN)
{
	(void)eIN;

	// Send event
	S_GUI->addDelayedEvent(ev_MUTE_SOUND);

	return true;
}

bool SoundPreferences::onEventScrollPositionChanged(const CEGUI::EventArgs &eIN)
{
	// Get the window which fired the event <3
	const CEGUI::WindowEventArgs& e =
		static_cast<const CEGUI::WindowEventArgs&>(eIN);

	// Get scrollbar
	CEGUI::Scrollbar *scroll = static_cast<CEGUI::Scrollbar*>(e.window);

	// Update global volume
	volume = (1.0f - scroll->getScrollPosition()) * 100.0f;

	return true;
}

/////////////////////////////////////////////////////////////////////
/////	Add button and bind callbacks (could be templated?)
/////////////////////////////////////////////////////////////////////

void SoundPreferences::addButton(const std::string &name, bool hasHover)
{
	CEGUI::Window *w = addWindow(name);

	// Mouse hovered
	if (hasHover == true)
	{
		w->subscribeEvent(
			CEGUI::Window::EventMouseEntersArea,
			CEGUI::Event::Subscriber(&SoundPreferences::onEnterArea, this));

		w->subscribeEvent(
			CEGUI::Window::EventMouseLeavesArea,
			CEGUI::Event::Subscriber(&SoundPreferences::onLeaveArea, this));
	}

	// Mouse click
	w->subscribeEvent(
		CEGUI::PushButton::EventMouseClick,
		CEGUI::Event::Subscriber(&SoundPreferences::onClick, this));

	// Volume
	w->subscribeEvent(
		CEGUI::Scrollbar::EventScrollPositionChanged,
		CEGUI::Event::Subscriber(&SoundPreferences::onEventScrollPositionChanged, this));

	// Mute
	w->subscribeEvent(
		CEGUI::ToggleButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(&SoundPreferences::onEventCheckStateChanged, this));
}

/////////////////////////////////////////////////////////////////////
/////	Stop
/////////////////////////////////////////////////////////////////////

void SoundPreferences::stop(void)
{
	GUIEntity::stop();
}

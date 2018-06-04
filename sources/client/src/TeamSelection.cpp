/********************************************************************
**
**		TeamSelection.cpp
**		Created by : Joris VIal
**
********************************************************************/

#include "TeamSelection.hpp"
#include "Event.hpp"
#include "GUIManager.hpp"
#include "Map.hpp"
#include "Player.hpp"

/////////////////////////////////////////////////////////////////////
/////	Ctor/Dtor
/////////////////////////////////////////////////////////////////////

TeamSelection::TeamSelection(void)
{
	// Module type
	_type = GUI_TEAM_SELECTION_MENU;
}

TeamSelection::~TeamSelection(void)
{

}

/////////////////////////////////////////////////////////////////////
/////	Start
/////////////////////////////////////////////////////////////////////

void TeamSelection::start(CEGUI::Window *root)
{
	_root = root;
	_refreshTime = 0.0f;

	// Load content and subscribe to events

	addButton("SelectTeamWindow");
	addButton("PlayersTeamOneButton", true);
	addButton("PlayersTeamTwoButton", true);
	addButton("PlayersTeamOneListBox");
	addButton("PlayersTeamTwoListBox");
	addButton("SpectateButton", true);
}

/////////////////////////////////////////////////////////////////////
/////	Update
/////////////////////////////////////////////////////////////////////

void TeamSelection::update(float deltatime)
{
	if (isActive() == false)
		return;

	// Update time
	_refreshTime += deltatime;

	// Update player list every second
	if (_refreshTime >= 1.0f)
		fillPlayerList();
}

/////////////////////////////////////////////////////////////////////
/////	Enable / Disable
/////////////////////////////////////////////////////////////////////

void TeamSelection::setActive(bool active, bool autofocus)
{
	for (auto it = _windows.begin(); it != _windows.end(); ++it)
	{
		if (it->second == NULL)
		{
			VC_WARNING("TeamSelection::setActive, " +
				it->first + " doesn't exist. Check your code strings");
		}
		else
		{
			it->second->setVisible(active);
			if (autofocus == true)
				(active == true) ? (it->second->activate()) : (it->second->deactivate());
		}
	}
	_active = active;

	// Refresh the player list
	if (active == true)
		fillPlayerList();
}

/////////////////////////////////////////////////////////////////////
/////	Callbacks
/////////////////////////////////////////////////////////////////////

bool TeamSelection::onClick(const CEGUI::EventArgs &eIN)
{
	// Get the window which fired the event <3
	const CEGUI::WindowEventArgs& e =
		static_cast<const CEGUI::WindowEventArgs&>(eIN);

	VC_INFO_CRITICAL("TeamSelection::onClick, trigger: " + getWindowName(e.window));

	// Spectator
	if (e.window == get("SpectateButton"))
		joinTeam(0);

	// Team 1
	else if (e.window == get("PlayersTeamOneButton") ||
		e.window == get("PlayersTeamOneListBox"))
		joinTeam(1);

	// Team 2
	else if (e.window == get("PlayersTeamTwoButton") ||
		e.window == get("PlayersTeamTwoListBox"))
		joinTeam(2);

	return true;
}

bool TeamSelection::onEnterArea(const CEGUI::EventArgs &eIN)
{
	(void)eIN;
	return true;
}

bool TeamSelection::onLeaveArea(const CEGUI::EventArgs &eIN)
{
	(void)eIN;
	return true;
}

bool TeamSelection::onKeyReleased(const CEGUI::EventArgs &eIN)
{
	// Get the key pressed <3
	const CEGUI::KeyEventArgs& e =
		static_cast<const CEGUI::KeyEventArgs&>(eIN);

	VC_INFO_CRITICAL("TeamSelection::onKeyEvent, trigger: " + getWindowName(e.window));

	// Team 1
	if (e.scancode == CEGUI::Key::One)
		joinTeam(1);

	// Team 1
	if (e.scancode == CEGUI::Key::Two)
		joinTeam(2);

	return true;
}

bool TeamSelection::onMouseWheel(const CEGUI::EventArgs &eIN)
{
	// Get mouse event <3
	const CEGUI::MouseEventArgs& e =
		static_cast<const CEGUI::MouseEventArgs&>(eIN);

	VC_INFO_CRITICAL("TeamSelection::onMouseWheel, trigger: " + getWindowName(e.window));

	// Get the listbox
	CEGUI::Listbox *list = NULL;

	if (e.window == get("PlayersTeamOneButton"))
		list = static_cast<CEGUI::Listbox*>(get("PlayersTeamOneListBox"));
	else if (e.window == get("PlayersTeamTwoButton"))
		list = static_cast<CEGUI::Listbox*>(get("PlayersTeamTwoListBox"));

	if (list == NULL)
		return true;

	// Get the scrollbard
	CEGUI::Scrollbar *s = list->getVertScrollbar();
	if (s == NULL)
		return true;

	// Scroll
	s->setScrollPosition(s->getScrollPosition() - e.wheelChange);

	return true;
}

/////////////////////////////////////////////////////////////////////
/////	Join a team
/////////////////////////////////////////////////////////////////////

void TeamSelection::joinTeam(int team)
{
	S_GUI->addDelayedEvent(ev_JOIN_TEAM, s_event(NULL, (void *)team));
	S_GUI->disableTopModule();
}

/////////////////////////////////////////////////////////////////////
/////	Fill the player list
/////////////////////////////////////////////////////////////////////

void TeamSelection::fillPlayerList(void)
{
	CEGUI::Listbox *boxOne =
		static_cast<CEGUI::Listbox *>(get("PlayersTeamOneListBox"));
	CEGUI::Listbox *boxTwo =
		static_cast<CEGUI::Listbox *>(get("PlayersTeamTwoListBox"));

	// Delete all ListBoxTextItem
	boxOne->resetList();
	boxTwo->resetList();

	for (auto it = S_Map->getPlayers()->begin(); it != S_Map->getPlayers()->end(); ++it)
	{
		Player *p = (*it).get();
		CEGUI::ListboxTextItem *entry =
			new CEGUI::ListboxTextItem(p->getNick(), p->getId());

		entry->setSelectionBrushImage("GlossySerpentFHD/MultiListSelectionBrush");

		if (p->getTeam() == 1)
			boxOne->addItem(entry);
		else if (p->getTeam() == 2)
			boxTwo->addItem(entry);
	}

	// Schedule next refresh
	_refreshTime = 0.0f;
}

/////////////////////////////////////////////////////////////////////
/////	Add button and bind callbacks (could be templated?)
/////////////////////////////////////////////////////////////////////

void TeamSelection::addButton(const std::string &name, bool hasHover)
{
	CEGUI::Window *w = addWindow(name);

	// Mouse hovered
	if (hasHover == true)
	{
		w->subscribeEvent(
			CEGUI::Window::EventMouseEntersArea,
			CEGUI::Event::Subscriber(&TeamSelection::onEnterArea, this));

		w->subscribeEvent(
			CEGUI::Window::EventMouseLeavesArea,
			CEGUI::Event::Subscriber(&TeamSelection::onLeaveArea, this));
	}

	// Mouse click
	w->subscribeEvent(
		CEGUI::PushButton::EventMouseClick,
		CEGUI::Event::Subscriber(&TeamSelection::onClick, this));

	// Key released
	w->subscribeEvent(
		CEGUI::Window::EventKeyUp,
		CEGUI::Event::Subscriber(&TeamSelection::onKeyReleased, this));

	// Specific event for "PlayersTeamXListBox"
	w->subscribeEvent(
		CEGUI::Scrollbar::EventMouseWheel,
		CEGUI::Event::Subscriber(&TeamSelection::onMouseWheel, this));
}

/////////////////////////////////////////////////////////////////////
/////	Stop
/////////////////////////////////////////////////////////////////////

void TeamSelection::stop(void)
{
	GUIEntity::stop();
}

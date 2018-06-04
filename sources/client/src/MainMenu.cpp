/********************************************************************
**
**		MainMenu.cpp
**		Created by : Joris VIal
**
********************************************************************/

#include "MainMenu.hpp"
#include "GUIManager.hpp"

/////////////////////////////////////////////////////////////////////
/////	Ctor/Dtor
/////////////////////////////////////////////////////////////////////

MainMenu::MainMenu(void)
{
	// Module type
	_type = GUI_MAIN_MENU;
}

MainMenu::~MainMenu(void)
{

}

/////////////////////////////////////////////////////////////////////
/////	Start
/////////////////////////////////////////////////////////////////////

void MainMenu::start(CEGUI::Window *root)
{
	_root = root;

	// Load content and subscribe to events

	addButton("Menus");
	addButton("QuitMenusButton", true);
	addButton("ResumeMenusButton", true);
	addButton("WeaponSelectionButton", true);
	addButton("ServerAdministrationButton", true);
	addButton("ChangeTeamButton", true);
	addButton("OptionsButton", true);
}

/////////////////////////////////////////////////////////////////////
/////	Update
/////////////////////////////////////////////////////////////////////

void MainMenu::update(float deltatime)
{
	(void)deltatime;
}

/////////////////////////////////////////////////////////////////////
/////	Callbacks
/////////////////////////////////////////////////////////////////////

bool MainMenu::onClick(const CEGUI::EventArgs &eIN)
{
	// Get the window which fired the event <3
	const CEGUI::WindowEventArgs& e =
		static_cast<const CEGUI::WindowEventArgs&>(eIN);

	VC_INFO_CRITICAL("MainMenu::onClick, trigger: " + getWindowName(e.window));

	// Resume
	if (e.window == get("ResumeMenusButton"))
		S_GUI->disableTopModule();

	// Quit
	else if (e.window == get("QuitMenusButton"))
		S_GUI->addDelayedEvent(ev_QUIT);

	// Weapon selection
	else if (e.window == get("WeaponSelectionButton"))
		S_GUI->enableModule(GUI_WEAPON_SELECTION_MENU);

	// Server admin
	else if (e.window == get("ServerAdministrationButton"))
		S_GUI->enableModule(GUI_SERVER_ADMIN_MENU);

	// Options
	else if (e.window == get("OptionsButton"))
		S_GUI->enableModule(GUI_OPTIONS_MENU);

	// Team selection
	else if (e.window == get("ChangeTeamButton"))
		S_GUI->enableModule(GUI_TEAM_SELECTION_MENU);

	return true;
}

bool MainMenu::onEnterArea(const CEGUI::EventArgs &eIN)
{
	(void)eIN;
	return true;
}

bool MainMenu::onLeaveArea(const CEGUI::EventArgs &eIN)
{
	(void)eIN;
	return true;
}

/////////////////////////////////////////////////////////////////////
/////	Add button and bind callbacks (could be templated?)
/////////////////////////////////////////////////////////////////////

void MainMenu::addButton(const std::string &name, bool hasOver)
{
	CEGUI::Window *w = addWindow(name);

	// Mouse hovered
	if (hasOver == true)
	{
		w->subscribeEvent(
			CEGUI::Window::EventMouseEntersArea,
			CEGUI::Event::Subscriber(&MainMenu::onEnterArea, this));

		w->subscribeEvent(
			CEGUI::Window::EventMouseLeavesArea,
			CEGUI::Event::Subscriber(&MainMenu::onLeaveArea, this));
	}

	// Mouse click
	w->subscribeEvent(
		CEGUI::PushButton::EventMouseClick,
		CEGUI::Event::Subscriber(&MainMenu::onClick, this));
}

/////////////////////////////////////////////////////////////////////
/////	Stop
/////////////////////////////////////////////////////////////////////

void MainMenu::stop(void)
{
	GUIEntity::stop();
}

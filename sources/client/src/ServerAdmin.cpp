/***************************-*****************************************
**
**		ServerAdmin.hpp
**		Created by : Joris VIal
**
********************************************************************/

#include "ServerAdmin.hpp"
#include "GUIManager.hpp"

/////////////////////////////////////////////////////////////////////
/////	Ctor/Dtor
/////////////////////////////////////////////////////////////////////

ServerAdmin::ServerAdmin(void)
{
	_type = GUI_SERVER_ADMIN_MENU;
}

ServerAdmin::~ServerAdmin(void)
{

}

/////////////////////////////////////////////////////////////////////
/////	Start
/////////////////////////////////////////////////////////////////////

void ServerAdmin::start(CEGUI::Window *root)
{
	_root = root;

	// Load content and subscribe to events

	addButton("AdministrationWindow");
	addButton("ChangeMapAdminButton", true);
	addButton("ChangeMapModeAdminButton", true);
	addButton("KickBanAdminButton", true);
	addButton("ValidateButton", true);
}

/////////////////////////////////////////////////////////////////////
/////	Update
/////////////////////////////////////////////////////////////////////

void ServerAdmin::update(float deltatime)
{
	(void)deltatime;
}

/////////////////////////////////////////////////////////////////////
/////	Callbacks
/////////////////////////////////////////////////////////////////////

bool ServerAdmin::onClick(const CEGUI::EventArgs &eIN)
{
	// Get the window which fired the event <3
	const CEGUI::WindowEventArgs& e =
		static_cast<const CEGUI::WindowEventArgs&>(eIN);

	VC_INFO_CRITICAL("ServerAdmin::onClick, trigger: " + getWindowName(e.window));

	// Map administration
	if (e.window == get("ChangeMapAdminButton"))
		S_GUI->enableModule(GUI_SERVER_ADMIN_MAP);

	// Mode administration
	if (e.window == get("ChangeMapModeAdminButton"))
		S_GUI->enableModule(GUI_SERVER_ADMIN_MODE);

	// Quit
	if (e.window == get("ValidateButton"))
		S_GUI->disableTopModule();

	return true;
}

bool ServerAdmin::onEnterArea(const CEGUI::EventArgs &eIN)
{
	(void)eIN;
	return true;
}

bool ServerAdmin::onLeaveArea(const CEGUI::EventArgs &eIN)
{
	(void)eIN;
	return true;
}

/////////////////////////////////////////////////////////////////////
/////	Add button and bind callbacks (could be templated?)
/////////////////////////////////////////////////////////////////////

void ServerAdmin::addButton(const std::string &name, bool hasHover)
{
	CEGUI::Window *w = addWindow(name);

	// Mouse hovered
	if (hasHover == true)
	{
		w->subscribeEvent(
			CEGUI::Window::EventMouseEntersArea,
			CEGUI::Event::Subscriber(&ServerAdmin::onEnterArea, this));

		w->subscribeEvent(
			CEGUI::Window::EventMouseLeavesArea,
			CEGUI::Event::Subscriber(&ServerAdmin::onLeaveArea, this));
	}

	// Mouse click
	w->subscribeEvent(
		CEGUI::PushButton::EventMouseClick,
		CEGUI::Event::Subscriber(&ServerAdmin::onClick, this));
}

/////////////////////////////////////////////////////////////////////
/////	Stop
/////////////////////////////////////////////////////////////////////

void ServerAdmin::stop(void)
{
	GUIEntity::stop();
}

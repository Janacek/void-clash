/********************************************************************
**
**		GUIEntity.hpp
**		Created by : Joris VIal
**
********************************************************************/

#ifndef GUI_ENTITY
#define GUI_ENTITY

#include <CEGUI/CEGUI.h>
#include <list>
#include "Log.hpp"

// Define shortcuts to CEGUI singletons
#define CEGUI_SYSTEM CEGUI::System::getSingleton()
#define CEGUI_SHEME CEGUI::SchemeManager::getSingleton()
#define CEGUI_FONT CEGUI::FontManager::getSingleton()
#define CEGUI_WINDOW CEGUI::WindowManager::getSingleton()
#define CEGUI_IMAGES CEGUI::ImageManager::getSingleton()

// Define entity type
enum eGuiComponents
{
	// Main menu
	GUI_MAIN_MENU,

	// Team selection
	GUI_TEAM_SELECTION_MENU,

	// Weapon selection
	GUI_WEAPON_SELECTION_MENU,

	// Options
	GUI_OPTIONS_MENU,
	GUI_OPTIONS_KEYS,
	GUI_OPTIONS_GRAPHIC,
	GUI_OPTIONS_SOUND,

	// Server admin
	GUI_SERVER_ADMIN_MENU,
	GUI_SERVER_ADMIN_KICK,
	GUI_SERVER_ADMIN_MAP,
	GUI_SERVER_ADMIN_MODE,

	// Chat
	GUI_CHAT_MENU,

	// Scoreboard
	GUI_SCORE_BOARD,

	// Hide all
	GUI_NONE_MENU,
};

// GUI view interface
class GUIEntity
{
public:
	GUIEntity(void)
	{
		// Allow chat everywhere
		_allowedWindow.push_back(GUI_CHAT_MENU);
	}
	virtual ~GUIEntity(void) {}

	virtual void start(CEGUI::Window *root) = 0;
	virtual void update(float deltatime) = 0;
	virtual void stop(void)
	{
		for (auto it = _windows.begin(); it != _windows.end(); ++it)
		{
			delete it->second;
		}
	}

	// Status management
	virtual void setActive(bool active, bool autofocus = true)
	{
		for (auto it = _windows.begin(); it != _windows.end(); ++it)
		{
			if (it->second == NULL)
				VC_WARNING("GUIEntity::setActive, " +
					   it->first +
					   " doesn't exist. Check your code strings");
			else
			{
				it->second->setVisible(active);
				if (active == true && autofocus == true)
					it->second->activate();
				else if (active == false)
					it->second->deactivate();
			}
		}
		_active = active;
	}

	// Find the CEGUI::Window name
	std::string getWindowName(CEGUI::Window *w)
	{
		for (auto it = _windows.begin(); it != _windows.end(); ++it)
		{
			if (it->second == NULL)
			{
				VC_WARNING("GUIEntity::getWindowName, " +
					it->first + " doesn't exist. Check your code strings");
			}
			else if (it->second == w)
				return it->first;
		}
		return "Unknown";
	}

	// Find the CEGUI::Window pointer
	CEGUI::Window* get(const std::string &name)
	{
		auto found = _windows.find(name);
		if (found == _windows.end())
		{
			VC_ERROR("GUIEntity::get, " + name +
				" doesn't exist. You probably forgot to add it with addWindow()",
				true);
		}
		return found->second;
	}

	// Check if the window type is allow to be displayed while I'm active
	bool allows(eGuiComponents type)
	{
		for (auto it = _allowedWindow.begin(); it != _allowedWindow.end(); ++it)
			if (*it == type)
				return true;
		return false;
	}

	// Getters
	bool isActive(void) { return _active; }
	eGuiComponents getType(void) { return _type; }

protected:
	// Window management
	CEGUI::Window* addWindow(const std::string &name)
	{
		if (_root->getChildRecursive(name) == NULL)
			VC_WARNING("GUIEntity::addWindow, unable to find " + name);
		_windows[name] = _root->getChildRecursive(name);
		return _windows[name];
	}

protected:
	bool _active;
	bool _allowOtherWindow;
	eGuiComponents _type;

	CEGUI::Window *_root;
	std::map<std::string, CEGUI::Window*> _windows;
	std::list<eGuiComponents> _allowedWindow;
};

#endif

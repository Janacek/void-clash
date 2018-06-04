/********************************************************************
**
**		MainMenu.hpp
**		Created by : Joris VIal
**
********************************************************************/

#ifndef MAIN_MENU
#define MAIN_MENU

#include "GUIEntity.hpp"

class MainMenu : public GUIEntity
{
public:
	MainMenu(void);
	virtual ~MainMenu(void);

	virtual void start(CEGUI::Window *root);
	virtual void update(float deltatime);
	virtual void stop(void);

private:
	// Add button and bind callbacks(could be templated ? )
	void addButton(const std::string &normal, bool hasHover = false);

private:
	// Callbacks
	bool onClick(const CEGUI::EventArgs &eIN);
	bool onEnterArea(const CEGUI::EventArgs &eIN);
	bool onLeaveArea(const CEGUI::EventArgs &eIN);
};

#endif 
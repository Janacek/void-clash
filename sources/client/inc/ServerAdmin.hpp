/********************************************************************
**
**		ServerAdmin.hpp
**		Created by : Joris VIal
**
********************************************************************/

#ifndef SERVER_ADMIN_GUI
#define SERVER_ADMIN_GUI

#include "GUIEntity.hpp"

class ServerAdmin : public GUIEntity
{
public:
	ServerAdmin(void);
	virtual ~ServerAdmin(void);

	virtual void start(CEGUI::Window *root);
	virtual void update(float deltatime);
	virtual void stop(void);

private:
	// Add button and bind callbacks(could be templated ? )
	void addButton(const std::string &name, bool hasHover = false);

private:
	// Callbacks
	bool onClick(const CEGUI::EventArgs &eIN);
	bool onEnterArea(const CEGUI::EventArgs &eIN);
	bool onLeaveArea(const CEGUI::EventArgs &eIN);
};

#endif
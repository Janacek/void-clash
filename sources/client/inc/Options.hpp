/********************************************************************
**
**		Options.hpp
**		Created by : Joris VIal
**
********************************************************************/

#ifndef OPTIONS_GUI
#define OPTIONS_GUI

#include "GUIEntity.hpp"

class Options : public GUIEntity
{
public:
	Options(void);
	virtual ~Options(void);

	virtual void start(CEGUI::Window *root);
	virtual void update(float deltatime);
	virtual void stop(void);

	// Status management
	virtual void setActive(bool active, bool autofocus = true); // overrided

private:
	// Add button and bind callbacks(could be templated ? )
	void addButton(const std::string &name, bool hasHover = false);

private:
	// Callbacks
	bool onClick(const CEGUI::EventArgs &eIN);
	bool onEnterArea(const CEGUI::EventArgs &eIN);
	bool onLeaveArea(const CEGUI::EventArgs &eIN);
	bool onKeyReleased(const CEGUI::EventArgs &eIN);
	bool onTextAccepted(const CEGUI::EventArgs &eIN);
};

#endif
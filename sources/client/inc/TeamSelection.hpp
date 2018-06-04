/********************************************************************
**
**		TeamSelection.hpp
**		Created by : Joris VIal
**
********************************************************************/

#ifndef TEAM_SELECTION
#define TEAM_SELECTION

#include "GUIEntity.hpp"

class TeamSelection : public GUIEntity
{
public:
	TeamSelection(void);
	virtual ~TeamSelection(void);

	virtual void start(CEGUI::Window *root);
	virtual void update(float deltatime);
	virtual void stop(void);

	// Status management
	virtual void setActive(bool active, bool autofocus = true); // overrided

private:
	// Add button and bind callbacks(could be templated ? )
	void addButton(const std::string &name, bool hasHover = false);

	// Fill the player list
	void fillPlayerList(void);

	// Join team
	void joinTeam(int team);

	// Callbacks
	bool onClick(const CEGUI::EventArgs &eIN);
	bool onEnterArea(const CEGUI::EventArgs &eIN);
	bool onLeaveArea(const CEGUI::EventArgs &eIN);
	bool onKeyReleased(const CEGUI::EventArgs &eIN);
	bool onMouseWheel(const CEGUI::EventArgs &eIN);

private:
	// Update player list only every seconds
	float _refreshTime;
};

#endif 

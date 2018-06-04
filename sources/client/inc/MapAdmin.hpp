/********************************************************************
**
**		MapAdmin.hpp
**		Created by : Joris VIal
**
********************************************************************/

#ifndef MAP_ADMIN
#define MAP_ADMIN

#include "GUIEntity.hpp"

class MapAdmin : public GUIEntity
{
public:
	MapAdmin(void);
	virtual ~MapAdmin(void);

	virtual void start(CEGUI::Window *root);
	virtual void update(float deltatime);
	virtual void stop(void);

private:
	// Add button and bind callbacks(could be templated ? )
	void addButton(const std::string &name, bool hasHover = false);

	// Fill map list box
	void setupMapList(void);

	// Change the map
	void changeMap(void);

	// Utils
	CEGUI::ListboxTextItem* createEntry(const std::string &name, int index);

private:
	// Callbacks
	bool onClick(const CEGUI::EventArgs &eIN);
	bool onEnterArea(const CEGUI::EventArgs &eIN);
	bool onLeaveArea(const CEGUI::EventArgs &eIN);

private:
	std::string _currentMapName;
};

#endif
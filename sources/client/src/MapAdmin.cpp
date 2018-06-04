/********************************************************************
**
**		MapAdmin.cpp
**		Created by : Joris VIal
**
********************************************************************/

#include "MapAdmin.hpp"
#include "Map.hpp"
#include "GUIManager.hpp"

/////////////////////////////////////////////////////////////////////
/////	Ctor/Dtor
/////////////////////////////////////////////////////////////////////

MapAdmin::MapAdmin(void)
{
	_type = GUI_SERVER_ADMIN_MAP;
}

MapAdmin::~MapAdmin(void)
{

}

/////////////////////////////////////////////////////////////////////
/////	Start
/////////////////////////////////////////////////////////////////////

void MapAdmin::start(CEGUI::Window *root)
{
	_root = root;

	// Load content and subscribe to events

	addButton("ChangeMapAdminWindow");
	addButton("MapsListBox", true);
	addButton("ChangeMapAdminWindowButton", true);

	// Fill maps list box
	setupMapList();
}

/////////////////////////////////////////////////////////////////////
/////	Update
/////////////////////////////////////////////////////////////////////

void MapAdmin::update(float deltatime)
{
	(void)deltatime;
}

/////////////////////////////////////////////////////////////////////
/////	Callbacks
/////////////////////////////////////////////////////////////////////

bool MapAdmin::onClick(const CEGUI::EventArgs &eIN)
{
	// Get the window which fired the event <3
	const CEGUI::WindowEventArgs& e =
		static_cast<const CEGUI::WindowEventArgs&>(eIN);

	VC_INFO_CRITICAL("MapAdmin::onClick, trigger: " + getWindowName(e.window));

	// Change the map
	if (e.window == get("ChangeMapAdminWindowButton"))
		changeMap();

	return true;
}

bool MapAdmin::onEnterArea(const CEGUI::EventArgs &eIN)
{
	// Get the window which fired the event <3
	const CEGUI::WindowEventArgs& e =
		static_cast<const CEGUI::WindowEventArgs&>(eIN);

	VC_INFO_CRITICAL("MapAdmin::onClick, trigger: " + getWindowName(e.window));

	return true;
}

bool MapAdmin::onLeaveArea(const CEGUI::EventArgs &eIN)
{
	// Get the window which fired the event <3
	const CEGUI::WindowEventArgs& e =
		static_cast<const CEGUI::WindowEventArgs&>(eIN);

	VC_INFO_CRITICAL("MapAdmin::onClick, trigger: " + getWindowName(e.window));

	return true;
}

/////////////////////////////////////////////////////////////////////
/////	Add button and bind callbacks (could be templated?)
/////////////////////////////////////////////////////////////////////

void MapAdmin::addButton(const std::string &name, bool hasHover)
{
	CEGUI::Window *w = addWindow(name);

	// Mouse hovered
	if (hasHover == true)
	{
		w->subscribeEvent(
			CEGUI::Window::EventMouseEntersArea,
			CEGUI::Event::Subscriber(&MapAdmin::onEnterArea, this));

		w->subscribeEvent(
			CEGUI::Window::EventMouseLeavesArea,
			CEGUI::Event::Subscriber(&MapAdmin::onLeaveArea, this));
	}

	// Mouse click
	w->subscribeEvent(
		CEGUI::PushButton::EventMouseClick,
		CEGUI::Event::Subscriber(&MapAdmin::onClick, this));
}

/////////////////////////////////////////////////////////////////////
/////	Fill map list box
/////////////////////////////////////////////////////////////////////

void MapAdmin::setupMapList(void)
{
	// Get list box
	CEGUI::Listbox *mapsBox =
		static_cast<CEGUI::Listbox *>(get("MapsListBox"));
	if (mapsBox == NULL)
		return;

	// Clear list
	mapsBox->resetList();

	// Fill the list
	std::deque<std::string>& mapList(S_Map->getMapDatabase()->getMapList());

	for (unsigned int i = 0; i < mapList.size(); ++i)
	{
		// Get the map name
		std::string	mapName = mapList.at(i);

		// Create an entry
		CEGUI::ListboxItem* entry = createEntry(mapName, i);
		mapsBox->addItem(entry);

		// Select the current played map
		if (_currentMapName == mapName)
			entry->setSelected(true);
	}
}

/////////////////////////////////////////////////////////////////////
/////	Change the current map
/////////////////////////////////////////////////////////////////////

void MapAdmin::changeMap(void)
{
	// Get list box
	CEGUI::Listbox *mapsBox =
		static_cast<CEGUI::Listbox *>(get("MapsListBox"));
	if (mapsBox == NULL)
		return;

	// Get the item selected
	CEGUI::ListboxItem *entry = mapsBox->getFirstSelectedItem();
	if (entry == NULL)
		return;

	unsigned int id = entry->getID();

	// Get the map name
	std::deque<std::string>& mapList(S_Map->getMapDatabase()->getMapList());

	if (id >= mapList.size()) // Impossible?
		return;

	// Change the map ! :D
	_currentMapName = mapList.at(entry->getID());
	S_GUI->addDelayedEvent(ev_REQUEST_CHANGE_MAP,
		s_event(NULL, (void *)_currentMapName.c_str()));

	// Close the window
	S_GUI->disableTopModule();
}

/////////////////////////////////////////////////////////////////////
/////	Utils
/////////////////////////////////////////////////////////////////////

CEGUI::ListboxTextItem* MapAdmin::createEntry(const std::string &name, int index)
{
	CEGUI::ListboxTextItem *entry = new CEGUI::ListboxTextItem(name, index);

	entry->setSelectionBrushImage("GlossySerpentFHD/MultiListSelectionBrush");

	return entry;
}

/////////////////////////////////////////////////////////////////////
/////	Stop
/////////////////////////////////////////////////////////////////////

void MapAdmin::stop(void)
{
	GUIEntity::stop();
}

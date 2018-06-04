/********************************************************************
**
**		ModeAdmin.cpp
**		Created by : Joris VIal
**
********************************************************************/

#include "ModeAdmin.hpp"
#include "GUIManager.hpp"

/////////////////////////////////////////////////////////////////////
/////	Ctor/Dtor
/////////////////////////////////////////////////////////////////////

ModeAdmin::ModeAdmin(void)
{
	_type = GUI_SERVER_ADMIN_MODE;
}

ModeAdmin::~ModeAdmin(void)
{

}

/////////////////////////////////////////////////////////////////////
/////	Start
/////////////////////////////////////////////////////////////////////

void ModeAdmin::start(CEGUI::Window *root)
{
	_root = root;

	// Load content and subscribe to events

	addButton("ChangeMapModeAdminWindow");
	addButton("MapModesListBox", true);
	addButton("ChangeMapModeAdminWindowButton", true);

	// Fill maps list box
	setupModeList();
}

/////////////////////////////////////////////////////////////////////
/////	Update
/////////////////////////////////////////////////////////////////////

void ModeAdmin::update(float deltatime)
{
	(void)deltatime;
}

/////////////////////////////////////////////////////////////////////
/////	Callbacks
/////////////////////////////////////////////////////////////////////

bool ModeAdmin::onClick(const CEGUI::EventArgs &eIN)
{
	// Get the window which fired the event <3
	const CEGUI::WindowEventArgs& e =
		static_cast<const CEGUI::WindowEventArgs&>(eIN);

	VC_INFO_CRITICAL("ModeAdmin::onClick, trigger: " + getWindowName(e.window));

	// Change the map
	if (e.window == get("ChangeMapModeAdminWindowButton"))
		changeMode();

	return true;
}

bool ModeAdmin::onEnterArea(const CEGUI::EventArgs &eIN)
{
	// Get the window which fired the event <3
	const CEGUI::WindowEventArgs& e =
		static_cast<const CEGUI::WindowEventArgs&>(eIN);

	VC_INFO_CRITICAL("ModeAdmin::onClick, trigger: " + getWindowName(e.window));

	return true;
}

bool ModeAdmin::onLeaveArea(const CEGUI::EventArgs &eIN)
{
	// Get the window which fired the event <3
	const CEGUI::WindowEventArgs& e =
		static_cast<const CEGUI::WindowEventArgs&>(eIN);

	VC_INFO_CRITICAL("ModeAdmin::onClick, trigger: " + getWindowName(e.window));

	return true;
}

/////////////////////////////////////////////////////////////////////
/////	Add button and bind callbacks (could be templated?)
/////////////////////////////////////////////////////////////////////

void ModeAdmin::addButton(const std::string &name, bool hasHover)
{
	CEGUI::Window *w = addWindow(name);

	// Mouse hovered
	if (hasHover == true)
	{
		w->subscribeEvent(
			CEGUI::Window::EventMouseEntersArea,
			CEGUI::Event::Subscriber(&ModeAdmin::onEnterArea, this));

		w->subscribeEvent(
			CEGUI::Window::EventMouseLeavesArea,
			CEGUI::Event::Subscriber(&ModeAdmin::onLeaveArea, this));
	}

	// Mouse click
	w->subscribeEvent(
		CEGUI::PushButton::EventMouseClick,
		CEGUI::Event::Subscriber(&ModeAdmin::onClick, this));
}

/////////////////////////////////////////////////////////////////////
/////	Fill mode list box
/////////////////////////////////////////////////////////////////////

void ModeAdmin::setupModeList(void)
{
	// Get list box
	CEGUI::Listbox *mapsBox =
		static_cast<CEGUI::Listbox *>(get("MapModesListBox"));
	if (mapsBox == NULL)
		return;

	// Get the current mode
	_currentMode = S_Map->getMode()->getModeEnum();

	// Clear list
	mapsBox->resetList();

	// Create manualy entries
	addMode(mapsBox, "Free For All", mode_FFA);
	addMode(mapsBox, "Team Death Match", mode_TEAM_DM);
	addMode(mapsBox, "Capture The Flag", mode_CTF);
	addMode(mapsBox, "Capture Zone", mode_CAPTURE);
	addMode(mapsBox, "Last Man Standing", mode_SURVIVOR);
	addMode(mapsBox, "Last Team Standing", mode_TEAM_SURVIVOR);
}

/////////////////////////////////////////////////////////////////////
/////	Add a mode in the list and select the current one
/////////////////////////////////////////////////////////////////////

void ModeAdmin::addMode(CEGUI::Listbox *list, const std::string &name, eMapMode mode)
{
	CEGUI::ListboxItem* entry = createEntry(name, mode);

	list->addItem(entry);

	if (_currentMode == mode)
		entry->setSelected(true);
}

/////////////////////////////////////////////////////////////////////
/////	Change the current mode
/////////////////////////////////////////////////////////////////////

void ModeAdmin::changeMode(void)
{
	// Get list box
	CEGUI::Listbox *mapsBox =
		static_cast<CEGUI::Listbox *>(get("MapModesListBox"));
	if (mapsBox == NULL)
		return;

	// Get the item selected
	CEGUI::ListboxItem *entry = mapsBox->getFirstSelectedItem();
	if (entry == NULL)
		return;

	// The ID correponds to the eMapMode enum
	_currentMode = static_cast<eMapMode>(entry->getID());

	// Change the mode ! :D
	S_GUI->addDelayedEvent(ev_SWITCH_MAP_MODE, s_event(NULL, (void *)_currentMode));

	// Close the window
	S_GUI->disableTopModule();
}

/////////////////////////////////////////////////////////////////////
/////	Utils
/////////////////////////////////////////////////////////////////////

CEGUI::ListboxTextItem* ModeAdmin::createEntry(const std::string &name, int index)
{
	CEGUI::ListboxTextItem *entry = new CEGUI::ListboxTextItem(name, index);

	entry->setSelectionBrushImage("GlossySerpentFHD/MultiListSelectionBrush");

	return entry;
}

/////////////////////////////////////////////////////////////////////
/////	Stop
/////////////////////////////////////////////////////////////////////

void ModeAdmin::stop(void)
{
	GUIEntity::stop();
}

/********************************************************************
**
**		GraphicOptions.cpp
**		Created by : Joris VIal
**
********************************************************************/

#include "WeaponSelection.hpp"
#include "GUIManager.hpp"
#include "Map.hpp"

extern t_config *G_conf;

// Alpha
#define WEAPON_FULL_ALPHA 1.0f
#define WEAPON_MIDDLE_ALPHA 0.25f
#define WEAPON_LOW_ALPHA 0.0f

/////////////////////////////////////////////////////////////////////
/////	Ctor/Dtor
/////////////////////////////////////////////////////////////////////

WeaponSelection::WeaponSelection(void)
{
	_type = GUI_WEAPON_SELECTION_MENU;
	_weapons.first = NULL;
	_weapons.second = NULL;
}

WeaponSelection::~WeaponSelection(void)
{

}

/////////////////////////////////////////////////////////////////////
/////	Start
/////////////////////////////////////////////////////////////////////

void WeaponSelection::start(CEGUI::Window *root)
{
	_root = root;
	_autoselect = false;
	_firstUpdate = true;

	// Load content and subscribe to events
	addButton("WeaponSelectionWindow");

	// Main
	addButton("WeaponValidate", true);
	addButton("Weapon1Button", true);
	addButton("Weapon2Button", true);

	// Category
	_categories["BulletButton"] = addButton("BulletButton", true);
	_categories["BombButton"] = addButton("BombButton", true);
	_categories["GravityButton"] = addButton("GravityButton", true);
	_categories["TurretButton"] = addButton("TurretButton", true);
	_categories["ShieldButton"] = addButton("ShieldButton", true);

	// Description
	addButton("WeaponAttributes");
	addButton("WeaponAttributesLabel");

	// Weapon list
	addButton("WeaponList");
	addButton("WeaponListLabel");

	CEGUI::Listbox *weaponList =
		dynamic_cast<CEGUI::Listbox*>(get("WeaponList"));

	// Special event for weapon list
	weaponList->subscribeEvent(CEGUI::Listbox::EventSelectionChanged,
		CEGUI::Event::Subscriber(&WeaponSelection::onWeaponSelectionChanged, this));

	// Disable multi selection
	weaponList->setMultiselectEnabled(false);

	// Reset state
	resetSelection(1);
	resetSelection(2);
}

/////////////////////////////////////////////////////////////////////
/////	Update
/////////////////////////////////////////////////////////////////////

void WeaponSelection::update(float deltatime)
{
	(void)deltatime;

	if (S_Map->getCurrentPlayer() != NULL && _firstUpdate == true)
	{
		_weapons.first = S_Map->getCurrentPlayer()->getWeapons(true, false);
		_weapons.second = S_Map->getCurrentPlayer()->getWeapons(false, false);
		_firstUpdate = false;

		// Reset button slot 1 text
		if (_weapons.first == NULL)
			get("Weapon1Button")->setText(CEGUI::String("Arme 1"));
		else
			get("Weapon1Button")->setText(CEGUI::String(_weapons.first->name));

		// Display weapon 1 (more intuitive I think)
		selectWeaponSlot(GUI_WEAPON_SLOT_1);

		// Reset button slot 2 text
		if (_weapons.second == NULL)
			get("Weapon2Button")->setText(CEGUI::String("Arme 2"));
		else
			get("Weapon2Button")->setText(CEGUI::String(_weapons.second->name));
	}
}

/////////////////////////////////////////////////////////////////////
/////	Enable / Disable
/////////////////////////////////////////////////////////////////////

void WeaponSelection::setActive(bool active, bool autofocus)
{
	(void)autofocus;

	GUIEntity::setActive(active);

	// Tell other engines weapons changed if we close the GUI
	if (active == false)
	{
		std::shared_ptr<Player> p = S_Map->getCurrentPlayer();
		if (p != NULL)
			S_GUI->addDelayedEvent(ev_WEAPON_SELECTION, s_event(p, &_weapons));

		// Reset state
		resetSelection(1);
		resetSelection(2);
	}
}

/////////////////////////////////////////////////////////////////////
/////	Callbacks
/////////////////////////////////////////////////////////////////////

bool WeaponSelection::onClick(const CEGUI::EventArgs &eIN)
{
	// Get the window which fired the event <3
	const CEGUI::WindowEventArgs& e =
		static_cast<const CEGUI::WindowEventArgs&>(eIN);

	VC_INFO_CRITICAL("WeaponSelection::onClick, trigger: " + getWindowName(e.window));

	// Validate button
	if (e.window == get("WeaponValidate"))
	{
		// Send event if we have a current player
		std::shared_ptr<Player> p = S_Map->getCurrentPlayer();
		if (p != NULL)
			S_GUI->addDelayedEvent(ev_WEAPON_SELECTION, s_event(p, &_weapons));

		// Close the weapon menu
		S_GUI->disableTopModule();
	}

	// Weapon slot or category
	return handleSelection(e.window);

	return true;
}

bool WeaponSelection::onEnterArea(const CEGUI::EventArgs &eIN)
{
	(void)eIN;
	return true;
}

bool WeaponSelection::onLeaveArea(const CEGUI::EventArgs &eIN)
{
	(void)eIN;
	return true;
}

bool WeaponSelection::onWeaponSelectionChanged(const CEGUI::EventArgs &eIN)
{
	// Get the window which fired the event <3
	const CEGUI::WindowEventArgs& e =
		static_cast<const CEGUI::WindowEventArgs&>(eIN);

	VC_INFO_CRITICAL("WeaponSelection::onWeaponSelectionChanged, trigger: " + getWindowName(e.window));

	// Get the description box
	CEGUI::Listbox *weaponList =
		dynamic_cast<CEGUI::Listbox*>(get("WeaponList"));

	if (weaponList == NULL)
		return false;

	// Get the item selected
	CEGUI::ListboxItem *entry = weaponList->getFirstSelectedItem();

	// The user clicked outside the list
	if (entry == NULL)
	{
		selectWeapon(-1);
		return false;
	}

	// Get the id
	selectWeapon(entry->getID());

	return true;
}

/////////////////////////////////////////////////////////////////////
/////	Handle button selection
/////////////////////////////////////////////////////////////////////

bool WeaponSelection::handleSelection(CEGUI::Window *w)
{
	// Slot
	if (w == get("Weapon1Button"))
		return selectWeaponSlot(GUI_WEAPON_SLOT_1);
	else if (w == get("Weapon2Button"))
		return selectWeaponSlot(GUI_WEAPON_SLOT_2);

	// Category
	else if (w == get("BulletButton"))
		return selectWeaponCategory(GUI_WEAPON_BULLET);
	else if (w == get("BombButton"))
		return selectWeaponCategory(GUI_WEAPON_BOMB);
	else if (w == get("GravityButton"))
		return selectWeaponCategory(GUI_WEAPON_GRAVITY);
	else if (w == get("ShieldButton"))
		return selectWeaponCategory(GUI_WEAPON_SHIELD);
	else if (w == get("TurretButton"))
		return selectWeaponCategory(GUI_WEAPON_TURRET);

	return false;
}

/////////////////////////////////////////////////////////////////////
/////	Reset selection
/////////////////////////////////////////////////////////////////////

void WeaponSelection::resetSelection(int weapon)
{
	// Reset current state
	if (weapon == 1)
	{
		_weapon1.active = false;
		_weapon1.category = GUI_WEAPON_CATEGORY_NONE;
		_weapon1.idWeapon = -1;
		_weapon1.weaponSlot = GUI_WEAPON_SLOT_1;
	}
	else
	{
		_weapon2.active = false;
		_weapon2.category = GUI_WEAPON_CATEGORY_NONE;
		_weapon2.idWeapon = -1;
		_weapon2.weaponSlot = GUI_WEAPON_SLOT_2;
	}

	// Reset GUI
	_current = NULL;
	updateGUI();
}

/////////////////////////////////////////////////////////////////////
/////	Update the GUI
/////////////////////////////////////////////////////////////////////

void WeaponSelection::updateGUI(void)
{
	// Weapon slot selected
	if (_current != NULL)
	{
		selectWeaponSlot(_current->weaponSlot);
		selectWeaponCategory(_current->category);
		selectWeapon(_current->idWeapon);
	}
	// No slot selected, reset all GUI
	else
	{
		resetSelectionGUI();
	}
}

/////////////////////////////////////////////////////////////////////
/////	Reset all the GUI
/////////////////////////////////////////////////////////////////////

void WeaponSelection::resetSelectionGUI(void)
{
	// Weapon slot (lowered alpha)
	setWeaponSlotAlpha(WEAPON_MIDDLE_ALPHA);

	// Category selected
	setWeaponCategoryAlpha(WEAPON_LOW_ALPHA);

	// Description
	CEGUI::MultiLineEditbox *description =
		dynamic_cast<CEGUI::MultiLineEditbox*>(get("WeaponAttributes"));

	if (description != NULL)
	{
		description->setText("");
		setWeaponListDescAlpha(WEAPON_LOW_ALPHA);
	}

	// Reset button slot 1 text
	if (_weapons.first == NULL)
		get("Weapon1Button")->setText(CEGUI::String("Arme 1"));
	else
	{
		get("Weapon1Button")->setText(CEGUI::String(_weapons.first->name));

		// Display weapon 1 (more intuitive I think)
		selectWeaponSlot(GUI_WEAPON_SLOT_1);
	}

	// Reset button slot 2 text
	if (_weapons.second == NULL)
		get("Weapon2Button")->setText(CEGUI::String("Arme 2"));
	else
		get("Weapon2Button")->setText(CEGUI::String(_weapons.second->name));
}

/////////////////////////////////////////////////////////////////////
/////	Select a weapon slot
/////////////////////////////////////////////////////////////////////

bool WeaponSelection::selectWeaponSlot(eWeaponSlot slot)
{
	// Slot 1
	if (slot == 1)
	{
		setGuiElementAlpha("Weapon1Button", WEAPON_FULL_ALPHA);
		setGuiElementAlpha("Weapon2Button", WEAPON_MIDDLE_ALPHA);
		_weapon1.active = true;
		_weapon2.active = false;
		_current = &_weapon1;

		// Show weapon category
		setWeaponCategoryAlpha(WEAPON_MIDDLE_ALPHA);

		// Hide weapon list
		setWeaponListDescAlpha(WEAPON_LOW_ALPHA);

		// Preset GUI category if a weapon has already been selected
		if (_weapons.first != NULL)
		{
			_autoselect = true;
			selectWeaponCategory(stringToCategory(_weapons.first->category), slot);
			_autoselect = false;
		}

		return true;
	}

	// Slot 2
	else if (slot == 2)
	{
		setGuiElementAlpha("Weapon1Button", WEAPON_MIDDLE_ALPHA);
		setGuiElementAlpha("Weapon2Button", WEAPON_FULL_ALPHA);
		_weapon1.active = false;
		_weapon2.active = true;
		_current = &_weapon2;

		// Show weapon category
		setWeaponCategoryAlpha(WEAPON_MIDDLE_ALPHA);

		// Hide weapon list
		setWeaponListDescAlpha(WEAPON_LOW_ALPHA);

		// Preset GUI category if a weapon has already been selected
		if (_weapons.second != NULL)
		{
			_autoselect = true;
			selectWeaponCategory(stringToCategory(_weapons.second->category), slot);
			_autoselect = false;
		}

		return true;
	}

	// No slot
	resetSelectionGUI();
	setWeaponCategoryAlpha(WEAPON_LOW_ALPHA);
	_weapon1.active = false;
	_weapon2.active = false;
	_current = NULL;
	return false;
}

/////////////////////////////////////////////////////////////////////
/////	Select a weapon category
/////////////////////////////////////////////////////////////////////

bool WeaponSelection::selectWeaponCategory(eWeaponCategoryGUI category, eWeaponSlot slot)
{
	// Do nothing if the weapon slot is not selected yet
	if (_current != NULL)
	{
		_current->category = category;

		// Update weapon list
		updateWeaponList(category, slot);

		// Set default description (deselect all weapon)
		selectWeapon(-1);

		// Select button
		switch (category)
		{
		case GUI_WEAPON_BULLET: selectWeaponCategoryGUI("BulletButton"); return true;
		case GUI_WEAPON_BOMB: selectWeaponCategoryGUI("BombButton"); return true;
		case GUI_WEAPON_GRAVITY: selectWeaponCategoryGUI("GravityButton"); return true;
		case GUI_WEAPON_SHIELD: selectWeaponCategoryGUI("ShieldButton"); return true;
		case GUI_WEAPON_TURRET: selectWeaponCategoryGUI("TurretButton"); return true;
		case GUI_WEAPON_CATEGORY_NONE: selectWeaponCategoryGUI(""); return false;
		default: selectWeaponCategoryGUI(""); return false;
		}
	}

	return false;
}

/////////////////////////////////////////////////////////////////////
/////	Update the category selection
/////////////////////////////////////////////////////////////////////

void WeaponSelection::selectWeaponCategoryGUI(const std::string &buttonName)
{
	// Lower alpha on unselected categories
	auto it = _categories.begin();
	auto end = _categories.end();

	while (it != end)
	{
		if (it->first != buttonName)
			setGuiElementAlpha(it->first, WEAPON_MIDDLE_ALPHA);

		++it;
	}

	// Full alpha on selected category if any
	if (buttonName.empty() == false)
		setGuiElementAlpha(buttonName, WEAPON_FULL_ALPHA);
}

/////////////////////////////////////////////////////////////////////
/////	Display the weapon list and description
////	according to current button selection
/////////////////////////////////////////////////////////////////////

void WeaponSelection::updateWeaponList(eWeaponCategoryGUI category, eWeaponSlot slot)
{
	if (category == GUI_WEAPON_CATEGORY_NONE)
		return;

	// Check if the current configuration has already been loaded
	if (G_conf == NULL || G_conf->weapons == NULL)
		return;

	// Show the weapon list / desc
	setWeaponListDescAlpha(WEAPON_FULL_ALPHA);

	// Clear current list
	CEGUI::Listbox *list =
		dynamic_cast<CEGUI::Listbox*>(get("WeaponList"));

	if (list == NULL)
		return;

	list->resetList();

	// Clear the t_weapon pointer vector
	_currentWeaponList.clear();

	// Get the category string
	std::string categoryStr(categoryToString(category));

	// Update the list
	int id = 0;
	auto it = G_conf->weapons->begin();
	auto end = G_conf->weapons->end();

	while (it != end)
	{
		// Add weapon if it corresponds to the current weapon category
		if ((*it)->category == categoryStr)
		{
			// Add weapon name
			CEGUI::ListboxTextItem *entry = createListBoxItem((*it)->name, id);
			list->addItem(entry);

			// Keep the t_weapon pointer to display description
			_currentWeaponList.push_back(*it);

			// Pre select weapon if already selected before
			if ((_weapons.first == (*it) && slot == GUI_WEAPON_SLOT_1) ||
				(_weapons.second == (*it) && slot == GUI_WEAPON_SLOT_2))
			{
				entry->setSelected(true);
			}

			++id;
		}
		++it;
	}
}

/////////////////////////////////////////////////////////////////////
/////	Select a weapon id
/////////////////////////////////////////////////////////////////////

void WeaponSelection::selectWeapon(int slot)
{
	// Update the weapon description
	CEGUI::MultiLineEditbox *description =
		dynamic_cast<CEGUI::MultiLineEditbox*>(get("WeaponAttributes"));

	if (description == NULL)
		return;

	if (slot < 0 && _autoselect == false)
	{
		description->setText(CEGUI::String(
			std::string("=== ARME ACTUELLE ===\n\n") +
			"S�lectionner une arme parmis la liste ci-dessous\n\n" +
			"=== DESCRIPTION ===\n\n" +
			"S�lectionner une arme parmi la liste ci-dessous"));

		setGuiElementAlpha("WeaponAttributes", WEAPON_MIDDLE_ALPHA);
		setGuiElementAlpha("WeaponAttributesLabel", WEAPON_MIDDLE_ALPHA);

		return;
	}

	// Get the id
	unsigned int id = slot;

	// Autoselection ?
	if (_autoselect == true)
	{
		// Get the description box
		CEGUI::Listbox *weaponList =
			dynamic_cast<CEGUI::Listbox*>(get("WeaponList"));

		if (weaponList != NULL)
		{
			// Get the item selected
			CEGUI::ListboxItem *entry = weaponList->getFirstSelectedItem();
			if (entry != NULL)
				id = entry->getID();
		}
	}

	// I don't know if it's possible, but we never know :D
	if (id > _currentWeaponList.size())
		return;

	// Save the weapon pointer for GUI management
	if (_current == &_weapon1)
		_weapons.first = _currentWeaponList[id];
	else  if (_current == &_weapon2)
		_weapons.second = _currentWeaponList[id];

	// Get the weapon description
	const std::string &descriptionStr = _currentWeaponList[id]->desc;
	const std::string &nameStr = _currentWeaponList[id]->name;

	description->setText(CEGUI::String("=== ARME ACTUELLE ===\n\n" +  nameStr + "\n\n" +
		"=== DESCRIPTION ===\n\n" + descriptionStr));

	setGuiElementAlpha("WeaponAttributes", WEAPON_FULL_ALPHA);
	setGuiElementAlpha("WeaponAttributesLabel", WEAPON_FULL_ALPHA);

	// Update the slot button
	if (_current == &_weapon1)
		get("Weapon1Button")->setText(CEGUI::String(_currentWeaponList[id]->name));
	else if (_current == &_weapon2)
		get("Weapon2Button")->setText(CEGUI::String(_currentWeaponList[id]->name));
}

/////////////////////////////////////////////////////////////////////
/////	Set the alphas
/////////////////////////////////////////////////////////////////////

void WeaponSelection::setWeaponSlotAlpha(float alpha)
{
	setGuiElementAlpha("Weapon1Button", alpha);
	setGuiElementAlpha("Weapon2Button", alpha);
}

void WeaponSelection::setWeaponCategoryAlpha(float alpha)
{
	setGuiElementAlpha("BulletButton", alpha);
	setGuiElementAlpha("BombButton", alpha);
	setGuiElementAlpha("GravityButton", alpha);
	setGuiElementAlpha("ShieldButton", alpha);
	setGuiElementAlpha("TurretButton", alpha);
}

void WeaponSelection::setWeaponListDescAlpha(float alpha)
{
	setGuiElementAlpha("WeaponAttributes", alpha);
	setGuiElementAlpha("WeaponList", alpha);
	setGuiElementAlpha("WeaponAttributesLabel", alpha);
	setGuiElementAlpha("WeaponListLabel", alpha);
}

/////////////////////////////////////////////////////////////////////
/////	Utils
/////////////////////////////////////////////////////////////////////

void WeaponSelection::setGuiElementAlpha(const std::string &buttonName, float alpha)
{
	if (buttonName.empty() == false)
		get(buttonName)->setAlpha(alpha);
}

CEGUI::ListboxTextItem* WeaponSelection::createListBoxItem(const std::string &text,
	int id)
{
	CEGUI::ListboxTextItem *entry = new CEGUI::ListboxTextItem(text, id);

	entry->setSelectionBrushImage("GlossySerpentFHD/MultiListSelectionBrush");
	entry->setSelectionColours(CEGUI::Colour(0, 1, 0, 1));

	return entry;
}

std::string WeaponSelection::categoryToString(eWeaponCategoryGUI category)
{
	switch (category)
	{
	case GUI_WEAPON_BULLET: return "BULLET";
	case GUI_WEAPON_BOMB: return "BOMB";
	case GUI_WEAPON_GRAVITY: return "GRAVITY";
	case GUI_WEAPON_SHIELD: return "SHIELD";
	case GUI_WEAPON_TURRET: return "TURRET";
	case GUI_WEAPON_CATEGORY_NONE: return "";
	default: return "";
	}
}

eWeaponCategoryGUI WeaponSelection::stringToCategory(const std::string &str)
{
	if (str == "BULLET")
		return GUI_WEAPON_BULLET;
	if (str == "BOMB")
		return GUI_WEAPON_BOMB;
	if (str == "GRAVITY")
		return GUI_WEAPON_GRAVITY;
	if (str == "SHIELD")
		return GUI_WEAPON_SHIELD;
	if (str == "TURRET")
		return GUI_WEAPON_TURRET;

	return GUI_WEAPON_CATEGORY_NONE;
}

/////////////////////////////////////////////////////////////////////
/////	Add button and bind callbacks
/////////////////////////////////////////////////////////////////////

CEGUI::Window* WeaponSelection::addButton(const std::string &name, bool hasOver)
{
	CEGUI::Window *w = addWindow(name);

	// Mouse hovered
	if (hasOver == true)
	{
		w->subscribeEvent(
			CEGUI::Window::EventMouseEntersArea,
			CEGUI::Event::Subscriber(&WeaponSelection::onEnterArea, this));

		w->subscribeEvent(
			CEGUI::Window::EventMouseLeavesArea,
			CEGUI::Event::Subscriber(&WeaponSelection::onLeaveArea, this));
	}

	// Mouse click
	w->subscribeEvent(
		CEGUI::PushButton::EventMouseClick,
		CEGUI::Event::Subscriber(&WeaponSelection::onClick, this));

	return w;
}

/////////////////////////////////////////////////////////////////////
/////	Stop
/////////////////////////////////////////////////////////////////////

void WeaponSelection::stop(void)
{
	GUIEntity::stop();
}

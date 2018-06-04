/********************************************************************
**
**              WeaponSelection.hpp
**              Created by : Joris VIal
**
********************************************************************/

#ifndef WEAPON_SELECTION
#define WEAPON_SELECTION

#include "GUIEntity.hpp"
#include "ConfigParser.hpp"

/////// Define sates ///////

enum eWeaponSlot
{
	GUI_WEAPON_SLOT_NONE,
	GUI_WEAPON_SLOT_1,
	GUI_WEAPON_SLOT_2,
};

enum eWeaponCategoryGUI
{
	GUI_WEAPON_CATEGORY_NONE,
	GUI_WEAPON_BULLET,
	GUI_WEAPON_BOMB,
	GUI_WEAPON_GRAVITY,
	GUI_WEAPON_SHIELD,
	GUI_WEAPON_TURRET,
};

struct PropertySelected
{
	eWeaponSlot weaponSlot;
	eWeaponCategoryGUI category;
	int idWeapon;
	bool active;
};

class WeaponSelection : public GUIEntity
{
public:
	WeaponSelection(void);
	virtual ~WeaponSelection(void);

	virtual void start(CEGUI::Window *root);
	virtual void update(float deltatime);
	virtual void stop(void);

	// Status management
	virtual void setActive(bool active, bool autofocus = true); // overrided

private:
	// Add button and bind callbacks(could be templated ? )
	CEGUI::Window* addButton(const std::string &name, bool hasHover = false);

	// Handle button selection (slot or category)
	bool handleSelection(CEGUI::Window *w);

	// Reset the selection
	void resetSelection(int weapon);
	void resetSelectionGUI(void);

	// Update the entire GUI
	void updateGUI(void);

	// Update slot
	void setWeaponSlotAlpha(float alpha);
	bool selectWeaponSlot(eWeaponSlot slot);

	// Update category
	void setWeaponCategoryAlpha(float alpha);
	bool selectWeaponCategory(eWeaponCategoryGUI category, eWeaponSlot slot = GUI_WEAPON_SLOT_NONE);
	void selectWeaponCategoryGUI(const std::string &buttonName);

	// Update weapon list and description
	void updateWeaponList(eWeaponCategoryGUI category, eWeaponSlot slot);
	void updateWeaponDescription(int idWeapon);
	void setWeaponListDescAlpha(float alpha);
	void selectWeapon(int idWeapon);

	// Utils
	void setGuiElementAlpha(const std::string &buttonName, float alpha);
	std::string categoryToString(eWeaponCategoryGUI category);
	eWeaponCategoryGUI stringToCategory(const std::string &category);
	CEGUI::ListboxTextItem* createListBoxItem(const std::string &text,
		int id);

private:
	bool _firstUpdate;

	// Check if fonction are called by autoselection or not
	bool _autoselect;

	// Callbacks
	bool onClick(const CEGUI::EventArgs &eIN);
	bool onEnterArea(const CEGUI::EventArgs &eIN);
	bool onLeaveArea(const CEGUI::EventArgs &eIN);
	bool onWeaponSelectionChanged(const CEGUI::EventArgs &eIN);

	// Current state
	PropertySelected *_current;
	PropertySelected _weapon1;
	PropertySelected _weapon2;

	// Keep pointer on weapon category selection
	std::map<std::string, CEGUI::Window*> _categories;

	// Current selected weapon
	std::pair<t_weapon *, t_weapon*> _weapons;

	// Keep weapon pointer to display proper description
	std::vector<t_weapon*> _currentWeaponList;
};

#endif

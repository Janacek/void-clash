/********************************************************************
**
**		ModeAdmin.hpp
**		Created by : Joris VIal
**
********************************************************************/

#ifndef MODE_ADMIN
#define MODE_ADMIN

#include "GUIEntity.hpp"
#include "Map.hpp"

class ModeAdmin : public GUIEntity
{
public:
	ModeAdmin(void);
	virtual ~ModeAdmin(void);

	virtual void start(CEGUI::Window *root);
	virtual void update(float deltatime);
	virtual void stop(void);

private:
	// Add button and bind callbacks(could be templated ? )
	void addButton(const std::string &name, bool hasHover = false);

	// Fill mode list box
	void setupModeList(void);
	void addMode(CEGUI::Listbox *list, const std::string &name, eMapMode mode);

	// Change the mode
	void changeMode(void);

	// Utils
	CEGUI::ListboxTextItem* createEntry(const std::string &name, int index);

private:
	// Callbacks
	bool onClick(const CEGUI::EventArgs &eIN);
	bool onEnterArea(const CEGUI::EventArgs &eIN);
	bool onLeaveArea(const CEGUI::EventArgs &eIN);

private:
	eMapMode _currentMode;

};

#endif
/********************************************************************
**
**		GraphicOptions.hpp
**		Created by : Joris VIal
**
********************************************************************/

#ifndef GRAPHIC_OPTIONS_GUI
#define GRAPHIC_OPTIONS_GUI

#include "GUIEntity.hpp"

class GraphicOptions : public GUIEntity
{
public:
	GraphicOptions(void);
	virtual ~GraphicOptions(void);

	virtual void start(CEGUI::Window *root);
	virtual void update(float deltatime);
	virtual void stop(void);

	virtual void setActive(bool active); // Override

private:
	// Add button and bind callbacks(could be templated ? )
	void addButton(const std::string &name, bool hasHover = false);

	// Initialize graphic options
	void initGraphicOptions(void);
	void initToogles(void);
	void initComboBox(void);
	void initEditBox(void);

	// Save graphic options
	void saveGraphicOptions(void);
	void saveToogles(void);
	void saveComboBox(void);
	void saveEditBox(void);

	// Utils
	CEGUI::ToggleButton* getToogle(const std::string &name);
	CEGUI::Combobox* getComboBox(const std::string &name);
	CEGUI::Editbox* getEditBox(const std::string &name);
	CEGUI::ListboxTextItem* createListBoxItem(const std::string &text, int id);
	
private:
	// Callbacks
	bool onClick(const CEGUI::EventArgs &eIN);
	bool onEnterArea(const CEGUI::EventArgs &eIN);
	bool onLeaveArea(const CEGUI::EventArgs &eIN);
};

#endif
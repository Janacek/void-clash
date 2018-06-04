/********************************************************************
**
**		GraphicOptions.cpp
**		Created by : Joris VIal
**
********************************************************************/

#include "GUIManager.hpp"
#include "GraphicOptions.hpp"
#include "GraphicConfiguration.hpp"

/////////////////////////////////////////////////////////////////////
/////	Ctor/Dtor
/////////////////////////////////////////////////////////////////////

GraphicOptions::GraphicOptions(void)
{
	_type = GUI_OPTIONS_GRAPHIC;
}

GraphicOptions::~GraphicOptions(void)
{

}

/////////////////////////////////////////////////////////////////////
/////	Start
/////////////////////////////////////////////////////////////////////

void GraphicOptions::start(CEGUI::Window *root)
{
	_root = root;

	// Load content and subscribe to events
	addButton("GraphicOptionsWindow");
	addButton("ValidateGraphicOptionsButton", true);

	// Toogles
	addButton("DisplayCursorCheckbox");
	addButton("VSyncCheckbox");
	addButton("FXAACheckbox");
	addButton("BloomCheckbox");
	addButton("DisplayAimCheckbox");
	addButton("DisplayBackgroundCheckbox");
	addButton("MotionBlurCheckbox");

	// Combo box
	addButton("DisplayTypeListbox");
	addButton("ResolutionListbox");

	// Edit box
	addButton("BloomCoeffEditbox");

	// Initialize options
	initGraphicOptions();
}

/////////////////////////////////////////////////////////////////////
/////	Update
/////////////////////////////////////////////////////////////////////

void GraphicOptions::update(float deltatime)
{
	(void)deltatime;
}

void GraphicOptions::setActive(bool active)
{
	GUIEntity::setActive(active);

	// Initialize graphic options
	if (active == true)
		initGraphicOptions();
}

/////////////////////////////////////////////////////////////////////
/////	Initialize graphic options
/////////////////////////////////////////////////////////////////////

void GraphicOptions::initGraphicOptions(void)
{
	// Toogles
	initToogles();

	// Combo box
	initComboBox();

	// Edit box
	initEditBox();
}

/////////////////////////////////////////////////////////////////////
/////	Initialize edit box
/////////////////////////////////////////////////////////////////////

void GraphicOptions::initEditBox(void)
{
	CEGUI::Editbox *bloomCoeff = getEditBox("BloomCoeffEditbox");
	bloomCoeff->setText(std::to_string((int)(GCONFIG_TMP.bloomCoeff)));
}

/////////////////////////////////////////////////////////////////////
/////	Initialize toogles
/////////////////////////////////////////////////////////////////////

void GraphicOptions::initToogles(void)
{
	// Cursor
	CEGUI::ToggleButton *showCursor = getToogle("DisplayCursorCheckbox");
	showCursor->setSelected(GCONFIG_TMP.displayCursor);

	// Vsync
	CEGUI::ToggleButton *vSync = getToogle("VSyncCheckbox");
	vSync->setSelected(GCONFIG_TMP.vsync);

	// Antialiasing
	CEGUI::ToggleButton *FXAA = getToogle("FXAACheckbox");
	FXAA->setSelected(GCONFIG_TMP.fxaa);

	// Glow
	CEGUI::ToggleButton *bloom = getToogle("BloomCheckbox");
	bloom->setSelected(GCONFIG_TMP.bloom);

	// Aim
	CEGUI::ToggleButton *displayAim = getToogle("DisplayAimCheckbox");
	displayAim->setSelected(GCONFIG_TMP.displayaim);

	// Background
	CEGUI::ToggleButton *displayBackground = getToogle("DisplayBackgroundCheckbox");
	displayBackground->setSelected(GCONFIG_TMP.displayBackground);

	// Motion blur
	CEGUI::ToggleButton *motionBlur = getToogle("MotionBlurCheckbox");
	motionBlur->setSelected(false); // Not implemented
	motionBlur->setVisible(false); // Not implemented
};

/////////////////////////////////////////////////////////////////////
/////	Initialize combo box
/////////////////////////////////////////////////////////////////////

void GraphicOptions::initComboBox(void)
{
	/////// Fullscreen - Borderless - Windowed ///////

	CEGUI::Combobox *displayTypeBox = getComboBox("DisplayTypeListbox");

	displayTypeBox->resetList();

	// Create entries - // TODO - Default selection is not working

	CEGUI::ListboxTextItem *entry1 = createListBoxItem("Fen�tr�", 0);
	displayTypeBox->addItem(entry1);
	entry1->setSelected(true);

	CEGUI::ListboxTextItem *entry3 = createListBoxItem("Plein �cran fen�tr�", 2);
	displayTypeBox->addItem(entry3);

	if (GCONFIG_TMP.borderless == true)
		entry3->setSelected(true);

	CEGUI::ListboxTextItem *entry2 = createListBoxItem("Plein �cran", 1);
	displayTypeBox->addItem(entry2);

	if (GCONFIG_TMP.fullscreen == true)
		entry2->setSelected(true);

	/////// Resolution ///////

	CEGUI::Combobox *resolutionBox = getComboBox("ResolutionListbox");

	resolutionBox->resetList();

	// Get possible resolutions
	auto res = S_Config->getResolutions();

	int i = 0;
	for (auto it = res.begin(); it != res.end(); ++it)
	{
		// Create new entry
		CEGUI::ListboxTextItem *entry = new CEGUI::ListboxTextItem(
			std::to_string((*it).first) +
			" * " +
			std::to_string((*it).second), i);

		entry->setSelectionBrushImage("GlossySerpentFHD/MultiListSelectionBrush");
		resolutionBox->addItem(entry);

		// Set current resolution to selected
		if (GCONFIG_TMP.winX == (int)((*it).first) &&
			GCONFIG_TMP.winY == (int)((*it).second))
		{
			entry->setSelected(true);
			resolutionBox->setText(std::to_string((*it).first) + " * " + std::to_string((*it).second));
		}

		++i;
	}
}

/////////////////////////////////////////////////////////////////////
/////	Save the current graphic configuration
/////////////////////////////////////////////////////////////////////

void GraphicOptions::saveGraphicOptions(void)
{
	// Toogles
	saveToogles();

	// Combo box
	saveComboBox();

	// Edit box
	saveEditBox();
}

/////////////////////////////////////////////////////////////////////
/////	Initialize combo box
/////////////////////////////////////////////////////////////////////

void GraphicOptions::saveToogles(void)
{
	// Cursor
	CEGUI::ToggleButton *showCursor = getToogle("DisplayCursorCheckbox");
	GCONFIG_TMP.displayCursor = showCursor->isSelected();

	// Vsync
	CEGUI::ToggleButton *vSync = getToogle("VSyncCheckbox");
	GCONFIG_TMP.vsync = vSync->isSelected();

	// Antialiasing
	CEGUI::ToggleButton *FXAA = getToogle("FXAACheckbox");
	GCONFIG_TMP.fxaa = FXAA->isSelected();

	// Glow
	CEGUI::ToggleButton *bloom = getToogle("BloomCheckbox");
	GCONFIG_TMP.bloom = bloom->isSelected();

	// Aim
	CEGUI::ToggleButton *displayAim = getToogle("DisplayAimCheckbox");
	GCONFIG_TMP.displayaim = displayAim->isSelected();

	// Background
	CEGUI::ToggleButton *displayBackground = getToogle("DisplayBackgroundCheckbox");
	GCONFIG_TMP.displayBackground = displayBackground->isSelected();

	// Not implemented
	//CEGUI::ToggleButton *motionBlur = getToogle("MotionBlurCheckbox");
	//GCONFIG_TMP.motionBlur = motionBlur->isSelected();
}

/////////////////////////////////////////////////////////////////////
/////	Initialize combo box
/////////////////////////////////////////////////////////////////////

void GraphicOptions::saveComboBox(void)
{
	/////// Fullscreen - Borderless - Windowed ///////

	CEGUI::ListboxItem *displayTypeItem =
		getComboBox("DisplayTypeListbox")->getSelectedItem();

	if (displayTypeItem != NULL)
	{
		if (displayTypeItem->getID() == 0)
		{
			GCONFIG_TMP.fullscreen = false;
			GCONFIG_TMP.borderless = false;
		}
		else if (displayTypeItem->getID() == 1)
		{
			GCONFIG_TMP.fullscreen = true;
			GCONFIG_TMP.borderless = false;
		}
		else
		{
			GCONFIG_TMP.borderless = true;
			GCONFIG_TMP.fullscreen = false;
		}
	}

	/////// Resolution  ///////

	CEGUI::ListboxItem *resolutionTypeBox =
		getComboBox("ResolutionListbox")->getSelectedItem();

	if (resolutionTypeBox != NULL)
	{
		auto res = S_Config->getResolutions().at(resolutionTypeBox->getID());

		GCONFIG_TMP.winX = res.first;
		GCONFIG_TMP.winY = res.second;

		S_Config->saveCurrentConfiguration();
		//S_GUI->addDelayedEvent(ev_REBOOT_GRAPHIC, s_event()); // Bugged

		// Fix me - GUI is fucked after reboot
	}
}

/////////////////////////////////////////////////////////////////////
/////	Initialize combo box
/////////////////////////////////////////////////////////////////////

void GraphicOptions::saveEditBox(void)
{

}

/////////////////////////////////////////////////////////////////////
/////	Callbacks
/////////////////////////////////////////////////////////////////////

bool GraphicOptions::onClick(const CEGUI::EventArgs &eIN)
{
	// Get the window which fired the event <3
	const CEGUI::WindowEventArgs& e =
		static_cast<const CEGUI::WindowEventArgs&>(eIN);

	VC_INFO_CRITICAL("GraphicOptions::onClick, trigger: " + getWindowName(e.window));

	// "Ok" button
	if (e.window == get("ValidateGraphicOptionsButton"))
	{
		saveGraphicOptions();
		S_GUI->disableTopModule();
	}

	return true;
}

bool GraphicOptions::onEnterArea(const CEGUI::EventArgs &eIN)
{
	(void)eIN;
	return true;
}

bool GraphicOptions::onLeaveArea(const CEGUI::EventArgs &eIN)
{
	(void)eIN;
	return true;
}

/////////////////////////////////////////////////////////////////////
/////	Add button and bind callbacks (could be templated?)
/////////////////////////////////////////////////////////////////////

void GraphicOptions::addButton(const std::string &name, bool hasOver)
{
	CEGUI::Window *w = addWindow(name);

	// Mouse hovered
	if (hasOver == true)
	{
		w->subscribeEvent(
			CEGUI::Window::EventMouseEntersArea,
			CEGUI::Event::Subscriber(&GraphicOptions::onEnterArea, this));

		w->subscribeEvent(
			CEGUI::Window::EventMouseLeavesArea,
			CEGUI::Event::Subscriber(&GraphicOptions::onLeaveArea, this));
	}

	// Mouse click
	w->subscribeEvent(
		CEGUI::PushButton::EventMouseClick,
		CEGUI::Event::Subscriber(&GraphicOptions::onClick, this));
}

/////////////////////////////////////////////////////////////////////
/////	Utils
/////////////////////////////////////////////////////////////////////

CEGUI::ToggleButton* GraphicOptions::getToogle(const std::string &name)
{
	CEGUI::ToggleButton *toogle =
		static_cast<CEGUI::ToggleButton *>(get(name));

	return toogle;
}

CEGUI::Combobox* GraphicOptions::getComboBox(const std::string &name)
{
	CEGUI::Combobox* comboBox =
		static_cast<CEGUI::Combobox *>(get(name));

	return comboBox;
}

CEGUI::Editbox* GraphicOptions::getEditBox(const std::string &name)
{
	CEGUI::Editbox* comboBox =
		static_cast<CEGUI::Editbox *>(get(name));

	return comboBox;
}

CEGUI::ListboxTextItem* GraphicOptions::createListBoxItem(const std::string &text,
	int id)
{
	CEGUI::ListboxTextItem *entry = new CEGUI::ListboxTextItem(text, id);

	entry->setSelectionBrushImage("GlossySerpentFHD/MultiListSelectionBrush");

	return entry;
}


/////////////////////////////////////////////////////////////////////
/////	Stop
/////////////////////////////////////////////////////////////////////

void GraphicOptions::stop()
{
	GUIEntity::stop();
}

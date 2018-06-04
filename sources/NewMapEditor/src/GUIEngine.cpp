#include <iostream>
#include "GUIEngine.h"
#include	"MapDatabase.hpp"
#include "MapSaver.h"

#include "SpeedField.hpp"
#include "Flag.hpp"
#include "Capture.hpp"

#include "Map.hpp"
#include "Event.hpp"

#include "GraphicHelper.hpp"
#include "HudRessources.hpp"

// Globals
extern t_config *_conf;
extern std::string g_ip;
extern int G_id;
extern bool G_isMapLoaded;

GUIEngine::GUIEngine()
{
}

GUIEngine::~GUIEngine()
{
}

void GUIEngine::start(void)
{
	m_mapData = NULL;
	mapToLoad = "";
	hoverMenu = false;
	G_isMapLoaded = false;

	m_window = &S_Context->getWindow();

	_objectToCreate = eObjectType::NONE;
	_objectCreated = NULL;

	std::cout << sf::VideoMode::getDesktopMode().height << std::endl;
	std::cout << sf::VideoMode::getDesktopMode().width << std::endl;

	m_renderer = &CEGUI::OpenGLRenderer::bootstrapSystem();

	sf::RenderWindow &win(S_Context->getWindow());
	m_renderer->setDisplaySize(CEGUI::Sizef(win.getSize().x, win.getSize().y));


	CEGUI::DefaultResourceProvider *rp = static_cast<CEGUI::DefaultResourceProvider *>(CEGUI::System::getSingleton().getResourceProvider());

	rp->setResourceGroupDirectory("schemes", "../datafiles/schemes/");
	rp->setResourceGroupDirectory("imagesets", "../datafiles/imagesets/");
	rp->setResourceGroupDirectory("fonts", "../datafiles/fonts/");
	rp->setResourceGroupDirectory("layouts", "../datafiles/layouts/");
	rp->setResourceGroupDirectory("looknfeels", "../datafiles/looknfeel/");
	rp->setResourceGroupDirectory("lua_scripts", "../datafiles/lua_scripts/");

	rp->setResourceGroupDirectory("schemas", "../datafiles/xml_schemas/");

	// set the default resource groups to be used
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");

		// create (load) the GlossySerpentFHD scheme file
	// (this auto-loads the GlossySerpentFHD looknfeel and imageset files)
	CEGUI::SchemeManager::getSingleton().createFromFile("GlossySerpentFHD.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	// create (load) a font.
	// The first font loaded automatically becomes the default font, but note
	// that the scheme might have already loaded a font, so there may already
	// be a default set - if we want the "DejaVuSans-10" font to definitely
	// be the default, we should set the default explicitly afterwards.
	CEGUI::FontManager::getSingleton().createFromFile( "DejaVuSans-10.font");

	// Load the imageSet of the Icons

	//CEGUI::ImageManager::getSingleton().loadImageset("Icons.imageset", "imagesets");

	CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont( "DejaVuSans-10" );

	//CEGUI::System::getSingleton().getDefaultGUIContext().
    //getMouseCursor().setDefaultImage( "GlossySerpentFHD/MouseArrow" );

	//CEGUI::System::getSingleton().getDefaultGUIContext().
    //setDefaultTooltipType( "GlossySerpentFHD/Tooltip" );

	myRoot = CEGUI::WindowManager::getSingleton().loadLayoutFromFile( "VoidClashMapEditor.layout" );
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow( myRoot );

	myRoot->setMousePassThroughEnabled(true);


	CEGUI::Window *leftWindow = myRoot->getChildRecursive("LeftWindow");
	CEGUI::Window *topWindow = myRoot->getChildRecursive("TopWindow");
	CEGUI::Window *bottomWindow = myRoot->getChildRecursive("BottomWindow");
	CEGUI::Window *rightWindow = myRoot->getChildRecursive("RightWindow");


	CEGUI::Window *button = myRoot->getChildRecursive("X");

	CEGUI::Window *openButton = myRoot->getChildRecursive("OpenButton");
	CEGUI::Window *openLoadButton = myRoot->getChildRecursive("RefuseSaveMapLoad");
	CEGUI::Window *saveAsButton = myRoot->getChildRecursive("SaveAsButton");
	saveAsButton->setDisabled(true);
	CEGUI::Window *newButton = myRoot->getChildRecursive("NewButton");
	CEGUI::Window *cancelButton = myRoot->getChildRecursive("CancelSaveMapLoad");
	CEGUI::Window *saveOnLoadButton = myRoot->getChildRecursive("ConfirmSaveMapLoad");
	CEGUI::Window *saveButton = myRoot->getChildRecursive("SaveButton");
	saveButton->setDisabled(true);

	CEGUI::Window *playButton = myRoot->getChildRecursive("PlayButton");
	playButton->setDisabled(true);

	CEGUI::Window *confirmMapLoad = myRoot->getChildRecursive("LoadMapButton");

	CEGUI::Window *saveMapButton = myRoot->getChildRecursive("MapNameButton");

	CEGUI::Window *deleteButton = myRoot->getChildRecursive("deleteObjectButton");


	CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("MapItems"));
	CEGUI::Listbox *mapBox = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("LoadMapListbox"));
	CEGUI::Combobox *availableItems = static_cast<CEGUI::Combobox *>(myRoot->getChildRecursive("ItemsAvailableCombobox"));


	CEGUI::Window *XButton = myRoot->getChildRecursive("X");
	CEGUI::Window *YButton = myRoot->getChildRecursive("Y");
	CEGUI::Window *XEndButton = myRoot->getChildRecursive("endX");
	CEGUI::Window *YEndButton = myRoot->getChildRecursive("endY");
	CEGUI::Window *XDirButton = myRoot->getChildRecursive("dirX");
	CEGUI::Window *YDirButton = myRoot->getChildRecursive("dirY");
	CEGUI::Window *WidthButton = myRoot->getChildRecursive("width");
	CEGUI::Window *HeightButton = myRoot->getChildRecursive("height");
	CEGUI::Window *PowerButton = myRoot->getChildRecursive("power");
	CEGUI::Window *SizeButton = myRoot->getChildRecursive("size");
	CEGUI::Window *TeamButton = myRoot->getChildRecursive("team");
	
	CEGUI::Window *WallCreationButton = myRoot->getChildRecursive("WallCreationButton");
	
	XButton->subscribeEvent(CEGUI::Editbox::EventTextChanged, CEGUI::Event::Subscriber(&GUICallbacks::XEditBoxChanged, &m_guiCallbacks));
	YButton->subscribeEvent(CEGUI::Editbox::EventTextChanged, CEGUI::Event::Subscriber(&GUICallbacks::YEditBoxChanged, &m_guiCallbacks));
	XEndButton->subscribeEvent(CEGUI::Editbox::EventTextChanged, CEGUI::Event::Subscriber(&GUICallbacks::XEndEditBoxChanged, &m_guiCallbacks));
	YEndButton->subscribeEvent(CEGUI::Editbox::EventTextChanged, CEGUI::Event::Subscriber(&GUICallbacks::YEndEditBoxChanged, &m_guiCallbacks));
	XDirButton->subscribeEvent(CEGUI::Editbox::EventTextChanged, CEGUI::Event::Subscriber(&GUICallbacks::XDirEditBoxChanged, &m_guiCallbacks));
	YDirButton->subscribeEvent(CEGUI::Editbox::EventTextChanged, CEGUI::Event::Subscriber(&GUICallbacks::YDirEditBoxChanged, &m_guiCallbacks));
	WidthButton->subscribeEvent(CEGUI::Editbox::EventTextChanged, CEGUI::Event::Subscriber(&GUICallbacks::WidthEditBoxChanged, &m_guiCallbacks));
	HeightButton->subscribeEvent(CEGUI::Editbox::EventTextChanged, CEGUI::Event::Subscriber(&GUICallbacks::HeightEditBoxChanged, &m_guiCallbacks));
	PowerButton->subscribeEvent(CEGUI::Editbox::EventTextChanged, CEGUI::Event::Subscriber(&GUICallbacks::PowerEditBoxChanged, &m_guiCallbacks));
	SizeButton->subscribeEvent(CEGUI::Editbox::EventTextChanged, CEGUI::Event::Subscriber(&GUICallbacks::SizeEditBoxChanged, &m_guiCallbacks));
	TeamButton->subscribeEvent(CEGUI::Editbox::EventTextChanged, CEGUI::Event::Subscriber(&GUICallbacks::TeamEditBoxChanged, &m_guiCallbacks));

	leftWindow->subscribeEvent(CEGUI::Window::EventMouseEntersArea, CEGUI::Event::Subscriber(&GUICallbacks::HoverMenu, &m_guiCallbacks));
	leftWindow->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, CEGUI::Event::Subscriber(&GUICallbacks::NotHoverMenu, &m_guiCallbacks));

	mapItems = box;
	mapItems->subscribeEvent(CEGUI::Listbox::EventSelectionChanged, CEGUI::Event::Subscriber(&GUICallbacks::ItemSelectionChanged, &m_guiCallbacks));

	cancelButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUICallbacks::CancelLoad, &m_guiCallbacks));
	openButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUICallbacks::OpenMap, &m_guiCallbacks));
	openLoadButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUICallbacks::OpenMapLoad, &m_guiCallbacks));
	saveAsButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUICallbacks::SaveAsMap, &m_guiCallbacks));
	newButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUICallbacks::NewMap, &m_guiCallbacks));
	saveButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUICallbacks::SaveMap, &m_guiCallbacks));
	saveOnLoadButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUICallbacks::SaveMap, &m_guiCallbacks));

	saveMapButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUICallbacks::SaveAsMap1, &m_guiCallbacks));

	confirmMapLoad->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUICallbacks::ConfirmMapToLoad, &m_guiCallbacks));
	mapBox->subscribeEvent(CEGUI::PushButton::EventMouseDoubleClick, CEGUI::Event::Subscriber(&GUICallbacks::ConfirmMapToLoad, &m_guiCallbacks));
	availableItems->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&GUICallbacks::NewItemSelected, &m_guiCallbacks));

	
	button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUICallbacks::PrintHello, &m_guiCallbacks));
	deleteButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUICallbacks::DeleteObject, &m_guiCallbacks));

	WallCreationButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUICallbacks::WallCreationSelection, &m_guiCallbacks));

	m_guiCallbacks.setApp(this);

	InitLoadMaps();
	InitAvailableItems();

	std::cout << "GUI Engine started." << std::endl;
}

void GUIEngine::InitAvailableItems()
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	CEGUI::Combobox *box = static_cast<CEGUI::Combobox *>(myRoot->getChildRecursive("ItemsAvailableCombobox"));

	box->addItem(new CEGUI::ListboxTextItem("Wall"));
	box->addItem(new CEGUI::ListboxTextItem("Spawn"));
	box->addItem(new CEGUI::ListboxTextItem("Flag"));
	box->addItem(new CEGUI::ListboxTextItem("Speed Field"));
	box->addItem(new CEGUI::ListboxTextItem("Capture Zone"));
}

void GUIEngine::InitLoadMaps()
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("LoadMapListbox"));

	box->resetList();


	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;

	if ((hFind = FindFirstFile("./../maps/*.json", &FindFileData)) != INVALID_HANDLE_VALUE){
		do{

			CEGUI::ListboxTextItem *entry = new CEGUI::ListboxTextItem(FindFileData.cFileName);
			entry->setSelectionBrushImage("GlossySerpentFHD/MultiListSelectionBrush");
			box->addItem(entry);

		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}
}

#include "Respawn.hpp"

void GUIEngine::CreateNewObject(const std::string &objectName)
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
	CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("MapItems"));
	AObject *newObject;

	if (objectName == "Spawn")
	{
		newObject = new Respawn();
	}
	else if (objectName == "Wall")
	{
		newObject = new Wall();
	}
	else if (objectName == "Flag")
	{
		newObject = new Flag();
	}
	else if (objectName == "Speed Field")
	{
		newObject = new SpeedField();
	}
	else if (objectName == "Capture Zone")
	{
		newObject = new Capture();
	}
	newObject->pushInMap();

	CustomListBoxItem *item = new CustomListBoxItem(objectName, NULL, newObject->getId());
	item->setSelectionBrushImage("GlossySerpentFHD/MultiListSelectionBrush");
	getMapData()->setModifications(true);
	box->addItem(item);
}

void GUIEngine::ConfirmMapToLoad()
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
	CEGUI::Window *loadMapWindow = myRoot->getChildRecursive("LoadMapWindow");
	CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("LoadMapListbox"));

	if (box->getFirstSelectedItem() == NULL)
	{
		return;
	}
	std::string mapToLoad = box->getFirstSelectedItem()->getText().c_str();

	loadMapWindow->setVisible(false);

	std::string prefix = ".\\..\\maps\\";
	
	OpenMap(prefix + mapToLoad);
}

void GUIEngine::OpenMapWindow()
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
	CEGUI::Window *loadMapWindow = myRoot->getChildRecursive("LoadMapWindow");
	CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("LoadMapListbox"));

	box->resetList();


	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;

	if ((hFind = FindFirstFile("./../maps/*.json", &FindFileData)) != INVALID_HANDLE_VALUE){
		do{

			CEGUI::ListboxTextItem *entry = new CEGUI::ListboxTextItem(FindFileData.cFileName);
			entry->setSelectionBrushImage("GlossySerpentFHD/MultiListSelectionBrush");
			box->addItem(entry);

		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}
	loadMapWindow->setVisible(true);
}

void GUIEngine::SelectObjectById(int id)
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("MapItems"));

	box->clearAllSelections();

	for (unsigned int i = 0; i < box->getItemCount(); ++i)
	{
		if (box->getListboxItemFromIndex(i)->getID() == id)
		{
			box->getListboxItemFromIndex(i)->setSelected(true);
			return;
		}
	}

}

eGameState GUIEngine::update(const sf::Time &)
{
	return RUN;
}

void GUIEngine::createWall(float aimmX, float aimmY, CEGUI::Listbox *box)
{
	_objectCreated = new Wall(aimmX, aimmY, aimmX, aimmY);
	_objectCreated->pushInMap();

	std::cout << _objectCreated->getId() << " - - " << std::endl;

	CustomListBoxItem *item = new CustomListBoxItem("Wall", NULL, _objectCreated->getId());
	item->setSelectionBrushImage("GlossySerpentFHD/MultiListSelectionBrush");
	getMapData()->setModifications(true);
	box->addItem(item);
}

void GUIEngine::createRespawn(float aimmX, float aimmY, CEGUI::Listbox *box)
{
	_objectCreated = new Respawn(aimmX, aimmY, aimmX, aimmY, aimmX, aimmY, 1);
	_objectCreated->pushInMap();

	std::cout << _objectCreated->getId() << " - - " << std::endl;

	CustomListBoxItem *item = new CustomListBoxItem("Respawn", NULL, _objectCreated->getId());
	item->setSelectionBrushImage("GlossySerpentFHD/MultiListSelectionBrush");
	getMapData()->setModifications(true);
	box->addItem(item);
}

void GUIEngine::createCapture(float aimmX, float aimmY, CEGUI::Listbox *box)
{
	_objectCreated = new Capture(aimmX, aimmY, aimmX, aimmY, aimmX, aimmY);
	_objectCreated->pushInMap();

	std::cout << _objectCreated->getId() << " - - " << std::endl;

	CustomListBoxItem *item = new CustomListBoxItem("Capture", NULL, _objectCreated->getId());
	item->setSelectionBrushImage("GlossySerpentFHD/MultiListSelectionBrush");
	getMapData()->setModifications(true);
	box->addItem(item);
}

void GUIEngine::createFlag(float aimmX, float aimmY, CEGUI::Listbox *box)
{
	_objectCreated = new Flag(aimmX, aimmY, aimmX, aimmY, 1);
	_objectCreated->pushInMap();

	std::cout << _objectCreated->getId() << " - - " << std::endl;

	CustomListBoxItem *item = new CustomListBoxItem("Flag", NULL, _objectCreated->getId());
	item->setSelectionBrushImage("GlossySerpentFHD/MultiListSelectionBrush");
	getMapData()->setModifications(true);
	box->addItem(item);
}

eGameState GUIEngine::update(const sf::Time &deltaTime, bool firtPass)
{
	//static int countdown = 15;

	//if (G_isMapLoaded == true && countdown <= 0)
	//{
	//	LoadMapGUI();
	//	G_isMapLoaded = false;
	//	countdown = 15;
	//}

	//--countdown;

	if (Event::getEventByType(ev_MAP_LOADED) != NULL)
	{
		LoadMapGUI();
	}

	if (firtPass == false)
		return RUN;

	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("MapItems"));

	std::list<std::pair<eventType, s_event>> *objectToCreate = Event::getEventByType(ev_OBJECT_CREATION);
	const sf::Vector2i &mouse(HudRessources::getInstance()->getMousePosition());
	float aimX = 0; float aimY = 0;
	screenToWorld(mouse.x, mouse.y, aimX, aimY);

	if (objectToCreate && _objectCreated == NULL)
	{
		std::pair<float, float> *positions = (std::pair<float, float> *)(objectToCreate->begin()->second.data);

		float aimmX = 0; float aimmY = 0;
		screenToWorld(mouse.x, mouse.y, aimmX, aimmY);

		if (_objectToCreate == WALL)
		{
			createWall(aimmX, aimmY, box);
		}
		if (_objectToCreate == RESPAWN)
		{
			createRespawn(aimmX, aimmY, box);
		}
		if (_objectToCreate == CAPTURE)
		{
			createCapture(aimmX, aimmY, box);
		}
		if (_objectToCreate == FLAG)
		{
			createFlag(aimmX, aimmY, box);
		}
	}

	if (_objectCreated)
	{
		_objectCreated->damage(
			_objectCreated->getX(),
			_objectCreated->getY(),
			aimX,
			aimY,
			aimX - _objectCreated->getX(),
			aimY - _objectCreated->getY(),
			_objectCreated->getTeam()
			);
	}
	//if (_objectCreated)
	//{
	//	_objectCreated->damage(
	//		_objectCreated->getX(),
	//		_objectCreated->getY(),
	//		_objectCreated->getDirX(),
	//		_objectCreated->getDirY(),
	//		aimX,
	//		aimY,
	//		//_objectCreated->getWidth(),
	//		//_objectCreated->getHeight(),
	//		_objectCreated->getTeam()
	//		);
	//}

	std::list<std::pair<eventType, s_event>> *objectCreated = Event::getEventByType(ev_OBJECT_CREATED);

	if (objectCreated)
	{
		std::cout << "Object created" << std::endl;
		_objectCreated = NULL;
	}

	std::list<std::pair<eventType, s_event>> *list = Event::getEventByType(ev_OBJECT_DESELECTED);

	if (list)
	{


		if (box->getFirstSelectedItem())
		{
			S_Map->findObjectWithID(box->getFirstSelectedItem()->getID())->setSelected(false);
		}

		box->clearAllSelections();
		m_mapData->setVisibleParameters("00000000000");
	}

	list = Event::getEventByType(ev_OBJECT_SELECTED);

	if (list && list->empty() == false)
	{

		// Deselect de l'ancien object

		if (box->getFirstSelectedItem())
		{
			S_Map->findObjectWithID(box->getFirstSelectedItem()->getID())->setSelected(false);
		}

		std::list<std::pair<eventType, s_event>>::iterator it = list->begin();

		AObject *obj = it->second.trigger.get();
		obj->setSelected(true);
		SelectObjectById(obj->getId());
		m_mapData->setObjectParameters(obj->getId());
	}

	return eGameState::RUN;
}

void GUIEngine::stop(void)
{
	//return EXIT;
}

MapData *GUIEngine::getMapData()
{
	return m_mapData;
}

void GUIEngine::ApplyNewValues()
{
	CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("MapItems"));

	if (box->getSelectedCount() == 1)
	{
		m_mapData->applyNewValues();
	}
}

void GUIEngine::NewMap()
{
	// 1 - check if a map is already opened
	// 2 - if map -> ask if user realy wants to create a new map
	// 3 - if user wants to create a new map -> check if modifications to save
	//if (m_mapData)
	//{
	//}
	//else
	{
		m_mapData = new MapData();
		m_window->setTitle(m_mapData->getName());

		myRoot->getChildRecursive("SaveButton")->setDisabled(true);
		myRoot->getChildRecursive("SaveAsButton")->setDisabled(true);
		myRoot->getChildRecursive("PlayButton")->setDisabled(true);

		m_mapData->setVisibleParameters("00000000000");
		CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("MapItems"));

		box->resetList();

		S_Map->clearMap();
	}
}

void GUIEngine::SaveMap()
{
	// 1 - Check if the map exists
	// 2 - If no -> Save As.
	// 3 - If yes -> Save

	myRoot->getChildRecursive("UnsavedMapLoad")->setVisible(false);

	if (m_mapData == NULL)
	{
		// Raise PopUp
		ShowWarning("Saving failed : No map in edition");

		return;
	}

	std::cout << "Map name : " << m_mapData->getFullPath() << std::endl;

	if (m_mapData->getFullPath() == "")
	{
		m_guiCallbacks.SaveAsMap();
	}

	MapSaver mapSave;

	if (!m_mapData->_map)
	{
		return;
	}
	mapSave.saveMap(m_mapData->_map, m_mapData->getName(), m_mapData->getFullPath());
	m_mapData->setModifications(false);

	m_window->setTitle(m_mapData->getName());
}

void GUIEngine::LoadMapGUI()
{
	m_mapData->LoadMapGUI();
}

void GUIEngine::OpenMap(const std::string &mapPath)
{
	if (m_mapData == NULL)
		m_mapData = new MapData();
	if (m_mapData->getModifications() == true)
	{
		myRoot->getChildRecursive("UnsavedMapLoad")->setVisible(true);
		mapToLoad = mapPath;
	}
	else
	{
		myRoot->getChildRecursive("UnsavedMapLoad")->setVisible(false);

		int i = mapPath.size();

		while (mapPath[i] != '\\')
		{
			--i;
		}
		++i;
		char name[256];
		for (int a = 0; a < 256; ++a)
			name[a] = 0;

		int j = 0;
		while (mapPath[i] != '.')
		{
			name[j] = mapPath[i];
			++i;
			++j;
		}

		m_mapData->setFullPath(mapPath);
		m_mapData->setName(name);

		m_mapData->LoadMap();
		
		m_window->setTitle(name);
		CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("MapItems"));
		if (box->getItemCount() > 0)
		{
			myRoot->getChildRecursive("PlayButton")->setDisabled(false);
		}
	}
}

void GUIEngine::CancelOpen()
{
	myRoot->getChildRecursive("UnsavedMapLoad")->setVisible(false);
}

void GUIEngine::ShowWarning(const std::string &warningText)
{
	CEGUI::Window *warningWindow = myRoot->getChildRecursive("WarningWindow");

	warningWindow->getChild("WarningWindowText")->setText(warningText);
	warningWindow->setVisible(true);
}
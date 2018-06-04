#include "Application.h"

//#include "GUICallbacks.h"

#include <iostream>
#include <fstream>

Application::Application(sf::Window *window)
{
}

Application::~Application()
{
}

MapData *Application::getMapData()
{
	return m_mapData;
}

#include "MapSaver.h"

void Application::Init()
{
	m_mapData = NULL;
	mapToLoad = "";




	std::cout << sf::VideoMode::getDesktopMode().height << std::endl;
	std::cout << sf::VideoMode::getDesktopMode().width << std::endl;

	m_renderer = &CEGUI::OpenGLRenderer::bootstrapSystem();

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
	CEGUI::SchemeManager::getSingleton().createFromFile( "TaharezLook.scheme");
	// create (load) a font.
	// The first font loaded automatically becomes the default font, but note
	// that the scheme might have already loaded a font, so there may already
	// be a default set - if we want the "DejaVuSans-10" font to definitely
	// be the default, we should set the default explicitly afterwards.
	CEGUI::FontManager::getSingleton().createFromFile( "DejaVuSans-10.font");

	// Load the imageSet of the Icons

	//CEGUI::ImageManager::getSingleton().loadImageset("Icons.imageset", "imagesets");

	CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont( "DejaVuSans-10" );

	CEGUI::System::getSingleton().getDefaultGUIContext().
    getMouseCursor().setDefaultImage( "TaharezLook/MouseArrow" );

	CEGUI::System::getSingleton().getDefaultGUIContext().
    setDefaultTooltipType( "TaharezLook/Tooltip" );

	myRoot = CEGUI::WindowManager::getSingleton().loadLayoutFromFile( "test.layout" );
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow( myRoot );

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

	CEGUI::Listbox *box =  static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("MapItems"));


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


	mapItems = box;
	mapItems->subscribeEvent(CEGUI::Listbox::EventSelectionChanged, CEGUI::Event::Subscriber(&GUICallbacks::ItemSelectionChanged, &m_guiCallbacks));

	cancelButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUICallbacks::CancelLoad, &m_guiCallbacks));
	openButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUICallbacks::OpenMap, &m_guiCallbacks));
	openLoadButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUICallbacks::OpenMapLoad, &m_guiCallbacks));
	saveAsButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUICallbacks::SaveAsMap, &m_guiCallbacks));
	newButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUICallbacks::NewMap, &m_guiCallbacks));
	saveButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUICallbacks::SaveMap, &m_guiCallbacks));
	saveOnLoadButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUICallbacks::SaveMap, &m_guiCallbacks));
	button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUICallbacks::PrintHello, &m_guiCallbacks));

	m_guiCallbacks.setApp(this);
}

void Application::Run()
{
	bool running = true;

	this->Init();
	while (running)
    {
        sf::Event event;
        CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
		
		context.setMouseClickEventGenerationEnabled(false);

		// Sets the multiselection of items on the listBox
		//mapItems->setMultiselectEnabled(m_guiCallbacks.multiSelectionEnabled);
		while (m_window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                running = false;
            }
            if (event.type == sf::Event::Resized)
            {
                glViewport(0, 0, event.size.width, event.size.height);
				CEGUI::Sizef newSize = CEGUI::Sizef((float)(event.size.width), (float)(event.size.height));
				
				CEGUI::System::getSingleton().notifyDisplaySizeChanged(newSize);
            }
			if (event.type == sf::Event::MouseMoved)
			{
				context.injectMousePosition((float)(sf::Mouse::getPosition(*m_window).x), (float)(sf::Mouse::getPosition(*m_window).y));
			}
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				if (m_guiCallbacks.multiSelectionEnabled)
					mapItems->setMultiselectEnabled(m_guiCallbacks.multiSelectionEnabled);
				context.injectMouseButtonDown(CEGUI::MouseButton::LeftButton);
			}
			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
			{
				context.injectMouseButtonUp(CEGUI::MouseButton::LeftButton);
			}
			if (event.type == sf::Event::MouseWheelMoved)
			{
				context.injectMouseWheelChange(event.mouseWheel.delta);
			}
			if (event.type == sf::Event::TextEntered)
			{
				context.injectChar(event.text.unicode);
				ApplyNewValues();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				context.injectKeyDown(m_guiCallbacks.toCEGUIkeys[event.key.code]);
				if (event.key.code == sf::Keyboard::LControl || event.key.code == sf::Keyboard::RControl)
					m_guiCallbacks.multiSelectionEnabled = true;
			}
			if (event.type == sf::Event::KeyReleased)
			{
				context.injectKeyUp(m_guiCallbacks.toCEGUIkeys[event.key.code]);
				if (event.key.code == sf::Keyboard::LControl || event.key.code == sf::Keyboard::RControl)
					m_guiCallbacks.multiSelectionEnabled = false;
			}


        }

		this->Update();
		this->Draw();
    }
}

void Application::Update()
{
}

void Application::Draw()
{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
		CEGUI::System::getSingleton().renderAllGUIContexts();

		m_window->display();
}

void Application::ApplyNewValues()
{
	CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("MapItems"));

	if (box->getSelectedCount() == 1)
	{
		m_mapData->applyNewValues();
	}
}

void Application::NewMap()
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
	}
}

void Application::SaveMap()
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

	//std::ofstream myfile;
	//myfile.open(m_mapData->getFullPath(), std::ios::in | std::ios::trunc);
	//myfile << "Writing this to a file.\n";
	//myfile.close();

	m_window->setTitle(m_mapData->getName());
}

void Application::OpenMap(const std::string &mapPath)
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

void Application::CancelOpen()
{
	myRoot->getChildRecursive("UnsavedMapLoad")->setVisible(false);
}

void Application::ShowWarning(const std::string &warningText)
{
	CEGUI::Window *warningWindow = myRoot->getChildRecursive("WarningWindow");

	warningWindow->getChild("WarningWindowText")->setText(warningText);
	warningWindow->setVisible(true);
}
#include <iostream>
#include "GUIEngine.h"
#include	"MapManager.hpp"
#include "MapSaver.h"


// Globals
extern t_config *_conf;
extern std::string g_ip;

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
	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	// create (load) a font.
	// The first font loaded automatically becomes the default font, but note
	// that the scheme might have already loaded a font, so there may already
	// be a default set - if we want the "DejaVuSans-10" font to definitely
	// be the default, we should set the default explicitly afterwards.
	CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-10.font");

	// Load the imageSet of the Icons

	//CEGUI::ImageManager::getSingleton().loadImageset("Icons.imageset", "imagesets");

	CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont("DejaVuSans-10");

	CEGUI::System::getSingleton().getDefaultGUIContext().
		getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

	CEGUI::System::getSingleton().getDefaultGUIContext().
		setDefaultTooltipType("TaharezLook/Tooltip");

	myRoot = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("test.layout");
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(myRoot);

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

	CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("MapItems"));


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

eGameState GUIEngine::update(const sf::Time &deltaTime)
{
	return eGameState::RUN;
}

void GUIEngine::stop(void)
{

}

MapData *GUIEngine::getMapData()
{
	return m_mapData;
}


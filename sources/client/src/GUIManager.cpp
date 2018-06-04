/********************************************************************
**
**		GUIManager.cpp
**		Created by : Joris Vial
**
********************************************************************/

#include "GUIManager.hpp"
#include "GraphicConfiguration.hpp"
#include "GraphicHelper.hpp"

// Main menu
#include "MainMenu.hpp"

// Team selection
#include "TeamSelection.hpp"

// Server admin
#include "ServerAdmin.hpp"
#include "MapAdmin.hpp"
#include "ModeAdmin.hpp"

// Options
#include "Options.hpp"
#include "GraphicOptions.hpp"
#include "SoundPreferences.hpp"
#include "KeyPreferences.hpp"

// Weapon selection
#include "WeaponSelection.hpp"

// Chat
#include "Chat.hpp"

// Scoreboard
#include "Scoreboard.hpp"

// Disable CEGUI log
 #include "CEGUINoLogger.hpp"
 NoLogger nL;

/////////////////////////////////////////////////////////////////////
/////	Ctor/Dtor
/////////////////////////////////////////////////////////////////////

CGUIManager::CGUIManager(void)
{
	_started = false;
	_background = NULL;
}

CGUIManager::~CGUIManager(void)
{

}

/////////////////////////////////////////////////////////////////////
/////	Start
/////////////////////////////////////////////////////////////////////

void CGUIManager::start(void)
{
	// Create background
	_background = new ogl::Quad();
	_background->create();
	_background->resetTransform();
	_background->scale(2.0, 2.0);

	// Set color (transparent black)
	_shader = S_Shader->addShader("ColoredBackground");
	_shader->setParameter("Color", 0.0f, 0.0f, 0.0f, 0.75f);
	_shader->setParameter("Model", _background->getTransform());

	// EV_REBOOT_GRAPHIC ?
	if (_started == true)
		return;

	/////// Initial setup ///////

	setupRenderer();
	setupProvider();
	setupLooks();

	CEGUI_SYSTEM.getDefaultGUIContext().setRootWindow(_root);

	/////// Load views ///////

	addModule(new MainMenu());

	addModule(new TeamSelection());

	addModule(new ServerAdmin());
	addModule(new MapAdmin());
	addModule(new ModeAdmin());

	addModule(new Options());
	addModule(new GraphicOptions());
	addModule(new KeyPreferences());
	addModule(new SoundPreferences());

	addModule(new WeaponSelection());

	// Chat is handled separatly (it doesn't have a stack behavior)
	_chat = new Chat();
	_chat->start(_root);
	_chat->setActive(false);

	// Scoreboard is handled speratly too
	_scoreboard = new Scoreboard();
	_scoreboard->start(_root);
	_scoreboard->setActive(false);

	// Setup initial states

	disableAllModules();
	_stack.push(GUI_NONE_MENU);

	_started = true;
}

/////////////////////////////////////////////////////////////////////
/////	Setup renderer
/////////////////////////////////////////////////////////////////////

void CGUIManager::setupRenderer(void)
{
	// Get renderer
	_renderer = &(CEGUI::OpenGL3Renderer::bootstrapSystem());
	_renderer->setDisplaySize(CEGUI::Sizef(GCONFIG.winX, GCONFIG.winY));
}

/////////////////////////////////////////////////////////////////////
/////	Setup provider
/////////////////////////////////////////////////////////////////////

void CGUIManager::setupProvider(void)
{
	// Get default provider
	_provider = static_cast<CEGUI::DefaultResourceProvider *>
		(CEGUI_SYSTEM.getResourceProvider());

	// Set directories
	_provider->setResourceGroupDirectory("schemes", "../datafiles/schemes/");
	_provider->setResourceGroupDirectory("imagesets", "../datafiles/imagesets/");
	_provider->setResourceGroupDirectory("fonts", "../datafiles/fonts/");
	_provider->setResourceGroupDirectory("layouts", "../datafiles/layouts/");
	_provider->setResourceGroupDirectory("looknfeels", "../datafiles/looknfeel/");
	_provider->setResourceGroupDirectory("lua_scripts", "../datafiles/lua_scripts/");
	_provider->setResourceGroupDirectory("schemas", "../datafiles/xml_schemas/");

	// Set the default resource groups to be used
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
}

/////////////////////////////////////////////////////////////////////
/////	Load schemes / fonts / layouts
/////////////////////////////////////////////////////////////////////

void CGUIManager::setupLooks(void)
{
	// Schemes
	CEGUI_SHEME.createFromFile("GlossySerpentFHD.scheme");
	CEGUI_SHEME.createFromFile("TaharezLook.scheme");
	CEGUI_SHEME.createFromFile("OgreTray.scheme");

	// Fonts
	CEGUI_FONT.createFromFile("Jura-7.font");
	CEGUI_FONT.createFromFile("Jura-8.font");
	CEGUI_FONT.createFromFile("Jura-9.font");
	CEGUI_FONT.createFromFile("Jura-10.font");
	CEGUI_FONT.createFromFile("Jura-13.font");
	CEGUI_FONT.createFromFile("Jura-25.font");
	CEGUI_SYSTEM.getDefaultGUIContext().setDefaultFont("Jura-10");

	// Layout
	_root = CEGUI_WINDOW.loadLayoutFromFile("VoidClashClientGUI.layout");
	_root->subscribeEvent(
			CEGUI::Window::EventMouseClick,
			CEGUI::Event::Subscriber(&CGUIManager::onClick, this));

	// Images
	CEGUI_IMAGES.loadImageset("ClientIcons.imageset");
}

/////////////////////////////////////////////////////////////////////
/////	Update
/////////////////////////////////////////////////////////////////////

void CGUIManager::update(float deltatime)
{
	// Ignore so fucked deltatimes
	if (deltatime > 0.1f) deltatime = 0.1f;

	/////// Check events ///////

	if (Event::getEventByType(ev_WELCOME) != NULL)
		enableModule(GUI_TEAM_SELECTION_MENU);

	/////// Update modules ///////

	for (auto it = _modules.begin(); it != _modules.end(); ++it)
		(*it)->update(deltatime);

	// Separate handling for chat, scoreboard
	_chat->update(deltatime);
	_scoreboard->update(deltatime);
}

/////////////////////////////////////////////////////////////////////
/////	Draw
/////////////////////////////////////////////////////////////////////

void CGUIManager::draw(void)
{
	// Display background
	if (isDisplayed() == true || _scoreboard->isActive() == true)
		displayBackground();

	// Display GUI
	CEGUI_SYSTEM.renderAllGUIContexts();
}

/////////////////////////////////////////////////////////////////////
/////	Add a new module
/////////////////////////////////////////////////////////////////////

GUIEntity* CGUIManager::addModule(GUIEntity *e)
{
	e->start(_root);
	_modules.push_back(e);

	return e;
}

/////////////////////////////////////////////////////////////////////
/////	Enable a module (disable all other)
/////////////////////////////////////////////////////////////////////

void CGUIManager::enableModule(eGuiComponents view)
{
	// If we try to enable an already enabled module, ignore
	if (alreadyEnabled(view) == true)
		return;

	// Get the module reference to enable
	auto toEnable = _modules.begin();
	while (toEnable != _modules.end() && (*toEnable)->getType() != view)
		++toEnable;

	// The module doesn't exist, ignore
	if (toEnable == _modules.end())
		return;

	// Disable other module if they are not allowed
	// to be displayed at the same time
	for (auto it = _modules.begin(); it != _modules.end(); ++it)
	{
		if (// Disable active modules which are not allowed to be
			// displayed at the same time that view
			((*it)->isActive() == true && it != toEnable && (*toEnable)->allows((*it)->getType()) == false))
		{
			(*it)->setActive(false);
		}
	}

	// The module exists, enable it

	if (_chat->isInputEnabled() == true)
		// Don't autofocus
		(*toEnable)->setActive(true, false);
	else
		// Autofocus
		(*toEnable)->setActive(true, true);

	// Push it on the stack
	_stack.push(view);
	VC_INFO_CRITICAL("CGUIManager::enable " + std::to_string(view));
}

/////////////////////////////////////////////////////////////////////
/////	Disable the top module (the current one)
/////////////////////////////////////////////////////////////////////

void CGUIManager::disableTopModule(void)
{
	eGuiComponents view = _stack.top();
	VC_INFO_CRITICAL("CGUIManager::disable " + std::to_string(view));

	// No module enabled, ignore
	if (view == GUI_NONE_MENU)
		return;

	// Pop the top module
	_stack.pop();

	// Get the module reference to disable
	auto toDisable = _modules.begin();
	while (toDisable != _modules.end() && (*toDisable)->getType() != view)
		++toDisable;

	// The module doesn't exist, ignore
	if (toDisable == _modules.end())
		return;

	(*toDisable)->setActive(false);

	// Re enable the previous view
	view = _stack.top();
	VC_INFO_CRITICAL("CGUIManager::enable " + std::to_string(view));

	// NOthing more to do
	if (view == GUI_NONE_MENU)
		return;

	// Get the module reference to disable
	auto toEnable = _modules.begin();
	while (toEnable != _modules.end() && (*toEnable)->getType() != view)
		++toEnable;

	// The module doesn't exist, ignore
	if (toEnable == _modules.end())
		return;

	if (// Any modules but chat
		(*toEnable)->getType() != GUI_CHAT_MENU ||
		// Specific case for chat
		((*toEnable)->getType() == GUI_CHAT_MENU && _chat->isInputEnabled() == true))
	{
		(*toEnable)->setActive(true);
	}
}

/////////////////////////////////////////////////////////////////////
/////	Disable all modules
/////////////////////////////////////////////////////////////////////

void CGUIManager::disableAllModules(void)
{
	for (auto it = _modules.begin(); it != _modules.end(); ++it)
		(*it)->setActive(false);
}

/////////////////////////////////////////////////////////////////////
/////	Check if a module is already enable
////	(It should be better to check in the stack)
/////////////////////////////////////////////////////////////////////

bool CGUIManager::alreadyEnabled(eGuiComponents view)
{
	auto it = _modules.begin();
	auto end = _modules.end();

	while (it != end)
	{
		// If the module type corresponds to the view we want to activated
		// and is already activated, ignore the 'enableModule(view)' request
		if ((*it)->getType() == view && (*it)->isActive() == true)
			return true;
		++it;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////
/////	Delete all modules
/////////////////////////////////////////////////////////////////////

void CGUIManager::clearModules(void)
{
	// Delete all sub modules
	for (auto it = _modules.begin(); it != _modules.end(); ++it)
	{
		(*it)->stop();
		delete (*it);
	}
	_modules.clear();
}

/////////////////////////////////////////////////////////////////////
/////	Delayed events management
/////////////////////////////////////////////////////////////////////

void CGUIManager::addDelayedEvent(eventType type, s_event event)
{
	_delayedEvents.push_back(std::pair<eventType, s_event>(type, event));
}

void CGUIManager::flushDelayedEvent(void)
{
	for (auto it = _delayedEvents.begin(); it != _delayedEvents.end(); ++it)
		ADD_EVENT(it->first, it->second);

	_delayedEvents.clear();
}

/////////////////////////////////////////////////////////////////////
/////	Can we open a new module ?
/////////////////////////////////////////////////////////////////////

bool CGUIManager::canOpenNewModule(void)
{
	// The chat is opened and has input focus
	if (_chat->isActive() == true && _chat->isInputEnabled() == true)
		return false;

	// No menu opened
	if (_stack.top() == GUI_NONE_MENU)
		return true;

	return false;
}

/////////////////////////////////////////////////////////////////////
/////	Modules activation shortcuts
/////	Also define chat behavior
/////////////////////////////////////////////////////////////////////

void CGUIManager::injectModuleActivationKey(CEGUI::Key::Scan k)
{
	switch (k)
	{
		// Escape
	case CEGUI::Key::Escape:
	{
		// Chat is opened and has input focus
		if (_chat->isActive() == true && _chat->isInputEnabled() == true)
		{
			// Disable chat input
			_chat->setChatInputFocus(false);
			break;
		}

		// No current menu, open the main menu
		if (canOpenNewModule() == true)
			enableModule(GUI_MAIN_MENU);

		// A menu is already active, remove it
		else if (_stack.top() != GUI_NONE_MENU)
			disableTopModule();

		break;
	}

		// Team selection
	case CEGUI::Key::N:
		if (canOpenNewModule() == true)
			enableModule(GUI_TEAM_SELECTION_MENU);
		break;

		// Weapon selection
	case CEGUI::Key::B:
		if (canOpenNewModule() == true)
			enableModule(GUI_WEAPON_SELECTION_MENU);
		break;

		// Chat
	case CEGUI::Key::Return:
	{
		// Enable chat
		if (_chat->isActive() == false)
			_chat->setActive(true);

		// Re focus the chat
		else if (_chat->isActive() == true && _chat->isInputEnabled() == false)
			_chat->setChatInputFocus(true);

		break;
	}

		// Unknown
	default: break;
	}
}

/////////////////////////////////////////////////////////////////////
/////	Handle the scoreboard display
/////////////////////////////////////////////////////////////////////

void CGUIManager::displayScoreboard(bool displayed)
{
	_scoreboard->setActive(displayed);
}

bool CGUIManager::scoreboardDisplayed(void)
{
	return _scoreboard->isActive();
}

/////////////////////////////////////////////////////////////////////
/////	Are some GUI displayed (ignoring chat)
/////////////////////////////////////////////////////////////////////

bool CGUIManager::isDisplayed(void)
{
	eGuiComponents current = _stack.top();

	// Some menu are opened or the chat is opended
	if (current != GUI_NONE_MENU && current != GUI_CHAT_MENU)
		return true;

	// Nothing else is opened
	return false;
}

/////////////////////////////////////////////////////////////////////
/////	Does the chat input is focused
/////////////////////////////////////////////////////////////////////

bool CGUIManager::chatInputEnabled(void)
{
	return _chat->isInputEnabled();
}

/////////////////////////////////////////////////////////////////////
/////	Fade the game to focus on menus
/////////////////////////////////////////////////////////////////////

void CGUIManager::displayBackground(void)
{
	// Bind shader
	_shader->bind();

	// Draw background
	_background->draw();

	// Clear OpenGL
	_shader->unbind();

	GL_CHECK;
}

/////////////////////////////////////////////////////////////////////
/////	Click outside any menus
/////////////////////////////////////////////////////////////////////

bool CGUIManager::onClick(const CEGUI::EventArgs &eIN)
{
	(void)eIN;

	while (_stack.top() != GUI_NONE_MENU)
	{
		disableTopModule();
	}

	return true;
}

/////////////////////////////////////////////////////////////////////
/////	Stop
/////////////////////////////////////////////////////////////////////

void CGUIManager::stop(void)
{
	disableAllModules();
	for (auto it = _modules.begin(); it != _modules.end(); ++it)
	{
		(*it)->stop();
		delete (*it);
	}
	_modules.clear();

	_scoreboard->setActive(false);
	_scoreboard->stop();
	delete _scoreboard;

	_chat->setActive(false);
	_chat->stop();
	delete _chat;

	if (_background != NULL)
	{
		_background->destroy();
		delete _background;
	}

	// Slow down a lot the poweroff
	//CEGUI::OpenGL3Renderer::destroySystem();
}

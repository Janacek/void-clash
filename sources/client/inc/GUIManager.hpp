/********************************************************************
**
**              GUIManager.hpp
**              Created by : Joris Vial
**
********************************************************************/

#ifndef VOID_CLASH_GUI_MANAGER
#define VOID_CLASH_GUI_MANAGER

#include <list>
#include "VoidClash_OpenGL.h"
#include "Context.hpp"
#include "SingletonBase.hpp"
#include "GUIEntity.hpp"
#include "Event.hpp"
#include "ShaderManager.hpp"
#include "CEGUI/RendererModules/OpenGL/GL3Renderer.h"
#include "Quad.hpp"

// Standalone module
class Chat;
class Scoreboard;

class CGUIManager
{
public:
	CGUIManager(void);
	virtual ~CGUIManager(void);

	void start(void);
	void update(float deltatime);
	void draw(void);
	void stop(void);

	// Event management
	// GLFW triggers events when we refresh the screen aka at the
	// end of frame. So we need to keep events to send until the
	// beggining of the next frame. This way, other engines will
	// be abble to handle them
	void addDelayedEvent(eventType type, s_event event = s_event());
	void flushDelayedEvent(void);

	// Getters
	bool isDisplayed(void);
	bool chatInputEnabled(void);
	bool scoreboardDisplayed(void);

	// Module management
	void enableModule(eGuiComponents view);
	void disableTopModule(void);

	// Scoreboard is handle separatly (like chat)
	void displayScoreboard(bool displayed);

	// Modules activation shortcuts
	void injectModuleActivationKey(CEGUI::Key::Scan k);

private:
	// Callback
	bool onClick(const CEGUI::EventArgs &eIN);

private:
	// Initial setup
	void setupRenderer(void);
	void setupProvider(void);
	void setupLooks(void);

	// Display the background (transparent black)
	void displayBackground();

	// Module management
	GUIEntity* addModule(GUIEntity *e);
	void clearModules(void);
	void disableAllModules(void);

	// Get module states
	bool alreadyEnabled(eGuiComponents view);
	bool canOpenNewModule(void);

private:
	bool _started;

	// CEGUI mains
	CEGUI::OpenGL3Renderer *_renderer;
	CEGUI::DefaultResourceProvider *_provider;

	// Main window
	CEGUI::Window *_root;

	// Current view stack
	std::stack<eGuiComponents> _stack;

	// Sub modules
	std::list<GUIEntity*> _modules;

	// Delayed events
	std::list<std::pair<eventType, s_event>> _delayedEvents;

	// Module which doesn't have a stack behavior
	Chat *_chat;
	Scoreboard *_scoreboard;

	// Background
	ogl::Quad *_background;
	ogl::Shader *_shader;
};

typedef SingletonBase<CGUIManager> GUIManager;
#define S_GUI GUIManager::self()

#endif /* end of include guard: VOID_CLASH_GUI_MANAGER */

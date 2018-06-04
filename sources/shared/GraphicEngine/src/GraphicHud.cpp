/********************************************************************
**
**              GraphicHud.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "GraphicHud.hpp"
#include "Log.hpp"
#include "GraphicConfiguration.hpp"
#include "OpenGLContext.hpp"

// Sub modules
#include "GraphicMinimap.hpp"
#include "GraphicHudTop.hpp"
#include "GraphicInGameMessage.hpp"
#include "GraphicKillReport.hpp"
#include "GraphicHudWarmup.hpp"
#include "GraphicWeaponSelection.hpp"
#include "GraphicRespawnCooldown.hpp"
#include "GraphicAim.hpp"

namespace VoidClashGraphics
{
	/////////////////////////////////////////////////////////////////////
	/////	Constructor/ Destructor
	/////////////////////////////////////////////////////////////////////

	GraphicHud::GraphicHud(void)
	{

	}

	GraphicHud::~GraphicHud(void)
	{

	}

	/////////////////////////////////////////////////////////////////////
	/////	Start
	/////////////////////////////////////////////////////////////////////

	void GraphicHud::start(OpenGLContext *context)
	{
		VC_INFO("GraphicHud::starting");

		_isExiting = false;
		_context = context;

		// Create camera

		_camera = new ogl::Camera();
		_camera->setupOrthographic(GCONFIG.winX, GCONFIG.winY);
		_camera->update();

		// Create sub-modules

		createModule(new GraphicMinimap(context));
		createModule(new GraphicHudTop(context));
		createModule(new GraphicInGameMessage(context));
		createModule(new GraphicKillReport(context));
		createModule(new GraphicHudWarmup(context));
		createModule(new GraphicWeaponSelection(context));
		createModule(new GraphicRespawnCooldown(context));
		createModule(new GraphicAim(context));

		// Cursor
		_cursor = new GraphicCursor(context);
		_cursor->start(_camera);

		// FpsCounter
		_fps = new ogl::FpsCounter();
		_fps->start();

		VC_INFO("GraphicHud::started");
	}

	/////////////////////////////////////////////////////////////////////
	/////	Update
	/////////////////////////////////////////////////////////////////////

	bool GraphicHud::update(float deltatime)
	{
		auto it = _modules.begin();
		auto end = _modules.end();

		// Save current framebuffer
		_context->saveCurrentFramebuffer();

		// Update all modules
		while (it != end)
		{
			(*it)->update(deltatime);
			++it;
		}

		// Fps counter
		_fps->update(deltatime);

		return true;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Display the cursor on top of everything
	/////////////////////////////////////////////////////////////////////

	void GraphicHud::displayCursor(void)
	{
		_cursor->update(0.0f);
	}

	/////////////////////////////////////////////////////////////////////
	/////	Create a module and start it
	/////////////////////////////////////////////////////////////////////

	void GraphicHud::createModule(GraphicHudEntity *module)
	{
		if (module == NULL)
			return;

		// Start the module
		module->start(_camera);

		// Add it into the module container
		_modules.push_back(module);
	}

	/////////////////////////////////////////////////////////////////////
	/////	Stop
	/////////////////////////////////////////////////////////////////////

	void GraphicHud::stop(void)
	{
		VC_INFO("GraphicHud::stopping");

		delete _camera;

		// Stop sub-modules

		auto it = _modules.begin();
		auto end = _modules.end();

		while (it != end)
		{
			// Release memory
			(*it)->stop();
			delete (*it);
			++it;
		}

		// Remove modules
		_modules.clear();

		// Delete cursor
		_cursor->stop();
		delete _cursor;

		VC_INFO("GraphicHud::stopped");
	}
}

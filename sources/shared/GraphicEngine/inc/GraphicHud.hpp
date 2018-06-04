/********************************************************************
**
**              GraphicHud.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_GRAPHIC_HUD
#define VOID_CLASH_GRAPHIC_HUD

#include <list>
#include "GraphicHudEntity.hpp"
#include "GraphicCursor.hpp"
#include "FpsCounter.hpp"

namespace VoidClashGraphics
{
	class GraphicHud
	{
	public:
		GraphicHud(void);
		virtual ~GraphicHud(void);

		void start(OpenGLContext *context);
		bool update(float deltatime);
		void stop(void);

		// Check if the game is going to exit (network/version issues)
		bool isExiting(void) { return _isExiting; }

		// Display cursor
		void displayCursor(void);

	private:
		// Create a module and start it
		void createModule(GraphicHudEntity *module);

	private:
		bool _isExiting;

		// context
		OpenGLContext *_context;

		// Hud camera
		ogl::Camera *_camera;

		// Sub-module
		std::list<GraphicHudEntity*> _modules;

		// Cursor (specific case since we want to be sure it will be draw
		// on top of everything)
		GraphicCursor *_cursor;

		// Fps counter
		ogl::FpsCounter *_fps;
	};
}

#endif /* end of include guard: VOID_CLASH_GRAPHIC_HUD */

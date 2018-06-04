/********************************************************************
**
**		GraphicCursor.hpp
**		Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_GRAPHIC_CURSOR
#define VOID_CLASH_GRAPHIC_CURSOR

#include "GraphicHudEntity.hpp"

namespace VoidClashGraphics
{
	// Cursor type
	enum eCursorType
	{
		CURSOR_GAME,
		CURSOR_MENU
	};

	class GraphicCursor : public GraphicHudEntity
	{
	public:
		GraphicCursor(OpenGLContext *context);
		virtual ~GraphicCursor(void);

		virtual void start(ogl::Camera *camera);
		virtual void update(float deltatime);
		virtual void stop(void);

	private:
		virtual void clearOpenGLStates(void);

		// Load cursors
		void loadCursors(void);

		// Set current cursor
		void updateCurrentCursor(void);

		// Display cursor
		void displayCursor(void);

	private:
		/////// Cursors maps ///////

		// Type - Texture name
		std::map<eCursorType, std::string> _cursors;
		// Type - scale correction
		std::map<eCursorType, float> _scales;

		// Quad to display cursor
		ogl::Quad *_quad;

		// Shader
		ogl::Shader *_shader;

		// Camera
		ogl::Camera *_camera;

		// Current cursor
		eCursorType _currentCursor;
	};
}

#endif /* end of include guard: VOID_CLASH_GRAPHIC_CURSOR */

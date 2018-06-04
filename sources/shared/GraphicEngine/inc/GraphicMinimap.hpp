/********************************************************************
**
**              GraphicMinimap.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_GRAPHIC_MINIMAP
#define VOID_CLASH_GRAPHIC_MINIMAP

#include "GraphicHudEntity.hpp"
#include "Framebuffer.hpp"
#include "AObject.hpp"

namespace VoidClashGraphics
{
	class GraphicMinimap : public GraphicHudEntity
	{
	public:
		GraphicMinimap(OpenGLContext *context = NULL);
		virtual ~GraphicMinimap(void);

		virtual void start(ogl::Camera *camera);
		virtual void update(float deltatime);
		virtual void stop(void);

	private:
		virtual void clearOpenGLStates(void);

		// Final rendering
		void drawOnScreen(void);

		// Global design
		void drawBackground(void);
		void drawForeground(void);

		// Entities
		void drawEntities(void);
		void updateGameCamera(void);
		void drawWall(AObject *e);
		void drawPlayer(AObject *e);
		void drawFlag(AObject *e);
		void drawRespawn(AObject *e);
		void drawCapture(AObject *e);
		void drawSpeedfield(AObject *e);

	private:
		// Total time
		float _totalTime;

		// Rendering elements
		ogl::Quad *_quad;
		ogl::Framebuffer *_render;
		ogl::Camera *_camera;

		// Global design
		ogl::Shader *_shader;
		ogl::Texture *_background;
		ogl::Texture *_foreground;

		// Render to screen
		ogl::Shader *_shaderFinal;
		ogl::Shader *_shader2;

		// Entites (Geometry + Color shader)
		ogl::Shader *_entityShader;
		ogl::Camera *_gameCamera;

		// Entites textures
		ogl::Texture *_wall;
		ogl::Texture *_player;
		ogl::Texture *_flag;
		ogl::Texture *_respawn;
		ogl::Texture *_capture;
		ogl::Texture *_speedfield;
	};
}

#endif /* end of include guard: VOID_CLASH_GRAPHIC_MINIMAP */

/********************************************************************
**
**              GraphicHudTop.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_GRAPHIC_HUD_TOP
#define VOID_CLASH_GRAPHIC_HUD_TOP

#include "GraphicHudEntity.hpp"
#include "Framebuffer.hpp"

namespace VoidClashGraphics
{
	enum e_hudTopDisplayMode
	{
		HUD_TOP_SOLO,
		HUD_TOP_TEAM,
	};

	enum e_hudTopMiddlePart
	{
		HUD_TOP_TIME,
		HUD_TOP_ROUND,
	};

	class GraphicHudTop : public GraphicHudEntity
	{
	public:
		GraphicHudTop(OpenGLContext *context);
		virtual ~GraphicHudTop(void);

		virtual void start(ogl::Camera *camera);
		virtual void update(float deltaTime);
		virtual void stop(void);

	private:
		virtual void clearOpenGLStates(void);

		// Final rendering
		void drawOnScreen(void);

		// Draw background
		void drawBackground(void);

		// Draw time/round left
		e_hudTopMiddlePart getMiddlePartType(void);
		void drawRoundLeft(void);
		void drawTimeLeft(void);
		void drawTimeRoundLeft(void);

		// Draw score
		void drawScore(void);

		// Draw players alive
		void drawPlayersAlive(void);

		// Utils
		void updateMode(void);
		void bindTexture();

	private:
		e_hudTopDisplayMode _mode;

		// Rendering elements
		ogl::Framebuffer *_render;
		ogl::Camera *_camera;
		ogl::Camera *_localCamera;
		ogl::Quad *_quad;
		ogl::Shader *_shader;
		ogl::Shader *_shaderFinal;

		// Textures
		ogl::Texture *_team;
		ogl::Texture *_solo;

		// Text
		ogl::Text *_timeRoundLeft;
		ogl::Text *_timeRoundLeftLabel;

		ogl::Text *_score1;
		ogl::Text *_score2;

		ogl::Text *_aliveLabel;
		ogl::Text *_alive1;
		ogl::Text *_alive2;
	};
}

#endif /* end of include guard: VOID_CLASH_GRAPHIC_HUD_TOP */

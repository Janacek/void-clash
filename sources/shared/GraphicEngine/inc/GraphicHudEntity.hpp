/********************************************************************
**
**              GraphicHudEntity.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef GRAPHIC_HUD_ENTITY
#define GRAPHIC_HUD_ENTITY

#include "Context.hpp"
#include "Quad.hpp"
#include "Camera.hpp"
#include "ShaderManager.hpp"
#include "TextureManager.hpp"
#include "FontManager.hpp"

namespace VoidClashGraphics
{
	class OpenGLContext;

	class GraphicHudEntity
	{
	public:
		GraphicHudEntity(OpenGLContext *context) { _context = context; }
		virtual ~GraphicHudEntity(void) {}

		virtual void start(ogl::Camera *camera) = 0;
		virtual void update(float deltatime) = 0;
		virtual void stop(void) = 0;

	protected:
		virtual void clearOpenGLStates(void) = 0;

	protected:
		OpenGLContext *_context;
	};
}

#endif

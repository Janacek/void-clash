/********************************************************************
**
**              GraphicHudWarmup.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_GRAPHIC_HUD_WARMUP
#define VOID_CLASH_GRAPHIC_HUD_WARMUP

#include "GraphicHudEntity.hpp"

namespace VoidClashGraphics
{
	class GraphicHudWarmup : public GraphicHudEntity
	{
	public:
		GraphicHudWarmup(OpenGLContext *context);
		virtual ~GraphicHudWarmup(void);

		virtual void start(ogl::Camera *camera);
		virtual void update(float deltatime);
		virtual void stop(void);

	private:
		virtual void clearOpenGLStates(void);

	private:
		ogl::Text *_text;
	};
}

#endif /* end of include guard: VOID_CLASH_GRAPHIC_HUD_WARMUP */

/********************************************************************
**
**              GraphicRespawnCooldown.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_GRAPHIC_RESPAWN_COOLDOWN
#define VOID_CLASH_GRAPHIC_RESPAWN_COOLDOWN

#include "GraphicHudEntity.hpp"

namespace VoidClashGraphics
{
	class GraphicRespawnCooldown : public GraphicHudEntity
	{
	public:
		GraphicRespawnCooldown(OpenGLContext *context);
		virtual ~GraphicRespawnCooldown(void);

		virtual void start(ogl::Camera *camera);
		virtual void update(float deltatime);
		virtual void stop(void);

	private:
		virtual void clearOpenGLStates(void);

		// Display the text according the the respawn time left
		void displayLabel();
		void displayProgression();
		std::string prepareText(void);

		// Display the background (transparent black)
		void displayBackground();

		// Utils
		void calculateAlpha(void);
		void calculateColor(void);
		void calculateRatio(void);

	private:
		ogl::Text *_text;
		ogl::Text *_label;
		ogl::Quad *_background;
		ogl::Shader *_shader;

		// Smooth fade in/out
		glm::vec4 _color;
		float _alpha;
		float _ratio;
	};
}

#endif /* end of include guard: VOID_CLASH_GRAPHIC_RESPAWN_COOLDOWN */

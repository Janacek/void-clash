/********************************************************************
**
**              GraphicAim.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_GRAPHIC_AIM
#define VOID_CLASH_GRAPHIC_AIM

#include "GraphicHudEntity.hpp"

namespace VoidClashGraphics
{
	class GraphicAim : public GraphicHudEntity
	{
	public:
		GraphicAim(OpenGLContext *context);
		virtual ~GraphicAim(void);

		virtual void start(ogl::Camera *camera);
		virtual void update(float deltatime);
		virtual void stop(void);

	private:
		virtual void clearOpenGLStates(void);

		// Update the aim
		void updateAim(void);
		void displayAim(float deltatime);

	private:
		// Camera
		ogl::Camera *_camera;

		// Shader
		ogl::Shader *_shader;

		// Quad
		ogl::Quad *_aim;

		// Keep datas about aim
		glm::vec2 _playerPos;
		float _scale;
		float _totalTime;
	};
}

#endif /* end of include guard: VOID_CLASH_GRAPHIC_AIM */

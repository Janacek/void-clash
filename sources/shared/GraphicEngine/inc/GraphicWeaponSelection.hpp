/********************************************************************
**
**              GraphicWeaponSelection.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_GRAPHIC_WEAPON_SELECTION
#define VOID_CLASH_GRAPHIC_WEAPON_SELECTION

#include "GraphicHudEntity.hpp"
#include "Framebuffer.hpp"

namespace VoidClashGraphics
{
	class GraphicWeaponSelection : public GraphicHudEntity
	{
	public:
		GraphicWeaponSelection(OpenGLContext *context);
		virtual ~GraphicWeaponSelection(void);

		virtual void start(ogl::Camera *camera);
		virtual void update(float deltatime);
		virtual void stop(void);

	private:
		virtual void clearOpenGLStates(void);

		// Final rendering
		void drawOnScreen(bool primary);

		// Draw weapon selection into renderer
		void drawIcon(bool primary);
		void bindTexture(const std::string &weapon);
		void drawCooldown(bool primary);
		void drawLabel(bool primary);

		// Key
		void drawKey(void);

	private:
		// Rendering
		ogl::Quad *_quad;
		ogl::Shader *_final;
		ogl::Framebuffer *_render1;
		ogl::Framebuffer *_render2;
		ogl::Camera *_camera;
		ogl::Camera *_localCamera;

		// Weapon picture
		ogl::Shader *_weaponShader;

		// Cooldown
		ogl::Shader *_cooldownShader;
		ogl::Quad *_cooldown;

		// Weapons name
		ogl::Text *_label1;
		ogl::Text *_label2;

		// Key picture
		ogl::Quad *_key;
	};
}

#endif /* end of include guard: VOID_CLASH_GRAPHIC_WEAPON_SELECTION */

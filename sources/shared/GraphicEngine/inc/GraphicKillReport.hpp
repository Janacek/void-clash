/********************************************************************
**
**              GraphicKillReport.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_GRAPHIC_KILL_REPORT
#define VOID_CLASH_GRAPHIC_KILL_REPORT

#include "GraphicHudEntity.hpp"
#include "HudRessources.hpp"

namespace VoidClashGraphics
{
	// Describe a kill entry
	struct killEntry
	{
		// OpenGL ressources
		ogl::Text *killer;
		ogl::Text *killed;
		ogl::Quad *quad;

		// Entry property
		Player *pKiller;
		Player *pKilled;
		eObjectType weapon;
		float time;
	};

	class GraphicKillReport : public GraphicHudEntity
	{
	public:
		GraphicKillReport(OpenGLContext *context);
		virtual ~GraphicKillReport(void);

		virtual void start(ogl::Camera *camera);
		virtual void update(float deltatime);
		virtual void stop(void);

	private:
		virtual void clearOpenGLStates(void);

		// Kills management
		void getNewKillEntry(void);
		killEntry* createNewEntry(t_killHandler *kill);
		void updateEntries(float deltatime);

		// Display
		void drawEntries(void);
		void bindTexture(eObjectType type);

		// Utils
		glm::vec4 getEntryColor(Player *player, float currentTime);
		ogl::Font* getEntryFont(Player *player);

	private:
		std::list<killEntry*> _kills;

		// OpenGL
		ogl::Shader *_shader;
		ogl::Camera *_camera;
};
}

#endif /* end of include guard: VOID_CLASH_GRAPHIC_KILL_REPORT */

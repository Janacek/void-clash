/********************************************************************
**
**              GraphicInGameMessage.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_GRAPHIC_INGAME_MESSAGES
#define VOID_CLASH_GRAPHIC_INGAME_MESSAGES

#include "GraphicHudEntity.hpp"
#include "Event.hpp"

namespace VoidClashGraphics
{
	// Describe a message
	struct inGameMessage
	{
		float time;
		bool animated;
		ogl::Text *text;
	};

	class GraphicInGameMessage : public GraphicHudEntity
	{
	public:
		GraphicInGameMessage(OpenGLContext *context);
		virtual ~GraphicInGameMessage(void);

		virtual void start(ogl::Camera *camera);
		virtual void update(float deltatime);
		virtual void stop(void);

	private:
		virtual void clearOpenGLStates(void);

		// Messages management
		void addMessage(const std::string &message);
		void updateMessages(float deltatime);
		void checkEvents(void);

		// Display
		void drawMessages(void);

		// Message factory
		bool createKillStreakMessage(std::pair<eventType, s_event> &ev);
		bool createGameStartMessage(std::pair<eventType, s_event> &ev);
		bool createCaptureZoneMessage(std::pair<eventType, s_event> &ev);
		bool createFlagMessage(std::pair<eventType, s_event> &ev);

	private:
		// Rendering
		ogl::Camera *_camera;

		// Messages
		std::list<inGameMessage*> _messages;
		bool _blockNewMessages;
	};
}

#endif /* end of include guard: VOID_CLASH_GRAPHIC_INGAME_MESSAGES */

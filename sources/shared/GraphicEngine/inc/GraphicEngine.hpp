/********************************************************************
**
**              GraphicEngine.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOIDCLASH_GRAPHIC_ENGINE
#define VOIDCLASH_GRAPHIC_ENGINE

#include <list>
#include "AEngine.hpp"

namespace VoidClashGraphics
{
    class OpenGLContext;
    class LoadingScreen;
    class GraphicHud;
    class Renderer;
    class Scene;

    class GraphicEngine : public AEngine
    {
    public:
        GraphicEngine(void);
        virtual ~GraphicEngine(void);

        virtual void start(void);
        virtual eGameState update(const sf::Time &deltaTime);
        virtual void stop(void);
        void reboot(void);

    private:
        // Loading
        void continueStart(void);
        bool waitForServerConfiguration(float deltaTime);

        // Scene related
        void addChild(Scene *scene);
        void updateChilds(float deltatime);

        // Check new events
        void checkEvents(void);
        void checkDeletion(void);
        void checkAddition(void);

    private:
        // Wait for the client to recieve configuration files
        bool _fullyStarted;
		bool _configAlreadyRecieved;

        // Window and OpenGL context
        OpenGLContext *_context;

        // Renderer
        Renderer *_renderer;

        // Loading screen
        LoadingScreen *_loading;

        // Game HUD
        GraphicHud *_hud;

        // Scenes
        std::list<Scene*> _childs;
    };
}

#endif /* end of include guard: VOIDCLASH_GRAPHIC_ENGINE */

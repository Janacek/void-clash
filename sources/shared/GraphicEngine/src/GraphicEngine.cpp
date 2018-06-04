/********************************************************************
**
**              GraphicEngine.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include <chrono>
#include <thread>
#include "GraphicEngine.hpp"
#include "OpenGLContext.hpp"

// Configuration
#include "GraphicConfiguration.hpp"

// Sub modules
#include "LoadingScreen.hpp"
#include "GraphicHud.hpp"
#include "GUIManager.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"

// Managers
#include "TextureManager.hpp"
#include "ShaderManager.hpp"
#include "FontManager.hpp"
#include "ModelManager.hpp"

// Helpers
#include "GraphicHelper.hpp"

// Log and dev paths
#include "Log.hpp"
#include "AssetPath.h"

#include "ParticleSystem.hpp"
#include "ParticleSystemFactory.hpp"

// Globals
extern bool G_isOffline;

namespace VoidClashGraphics
{
    #define GRAPHIC_PASS_NUMBER 2

    /////////////////////////////////////////////////////////////////////
    /////	Ctor/Dtor
    /////////////////////////////////////////////////////////////////////

    GraphicEngine::GraphicEngine(void)
    {
		_configAlreadyRecieved = false;
    }

    GraphicEngine::~GraphicEngine(void)
    {

    }

    /////////////////////////////////////////////////////////////////////
    /////	Start
    /////////////////////////////////////////////////////////////////////

    void GraphicEngine::start(void)
    {
		VC_INFO_CRITICAL("GraphicEngine::Starting");

        _fullyStarted = false;

        // Initialize external library
        S_Context->initGLFW();

        // Load the graphic configuration
        S_Config->setConfigFilePath(ASSETS_PATH + "GraphicConfig.xml");
        S_Config->loadConfig();
        S_Config->dumpConfig();

        // Create the window and OpenGL context
        _context = new OpenGLContext();
        _context->start();

        // Create loading screen
        _loading = new LoadingScreen();
        _loading->start();
        _loading->update(20);

        // Start managers
        S_Shader->setRootDirectory(ASSETS_PATH + "assets/shaders/");
        S_Shader->start();
        _loading->update(30);

        S_Texture->setRootDirectory(ASSETS_PATH + "assets/img/");
        S_Texture->start();
        _loading->update(40);

        ogl::Text::initializeFreetype();
        S_Font->setRootDirectory(ASSETS_PATH + "assets/fonts/");
        S_Font->start();
        _loading->update(50);

        S_Model->setRootDirectory(ASSETS_PATH + "assets/models/");
        S_Model->start();
        _loading->update(60);

        // Create Renderer
        _renderer = new Renderer();
        _renderer->start();

        // Create HUD
        _hud = new GraphicHud();
        _hud->start(_context);

		// Start CEGUI
		S_GUI->start();
		_loading->update(70);

		VC_INFO_CRITICAL("GraphicEngine::Started");
    }

    /////////////////////////////////////////////////////////////////////
    /////	Continue loading when the client recieved
    /////   configuration file from the server
    /////////////////////////////////////////////////////////////////////

    void GraphicEngine::continueStart(void)
    {
		VC_INFO_CRITICAL("GraphicEngine::Continuing loading");

        // Create scenes
        Scene *scene = NULL;

        // Environment
        scene = new Scene("Environment_Scene");
        scene->addModule(G_ENVIRONMENT);
        scene->disableCameraAutoUpdate();
        addChild(scene);

        // Particle systems
        scene = new Scene("Particle_System_Scene");
        scene->addModule(G_PARTICLE_SYSTEM);
        scene->disableCameraAutoUpdate();
        addChild(scene);

        // Lights
        scene = new Scene("Light_Scene");
        scene->addModule(G_LIGHT);
        addChild(scene);

		// Static elements
        scene = new Scene("Static_Scene");
        scene->addModule(G_RESPAWN);
        scene->addModule(G_SPEEDFIELD);
        scene->addModule(G_CAPTURE);
        scene->addModule(G_GRAVITYFIELD);
        addChild(scene);

        // GravityField
        scene = new Scene("Gravity_Scene");
        scene->addModule(G_GRAVITYFIELD);
        addChild(scene);

        // Walls
        scene = new Scene("Wall_Scene");
        scene->addModule(G_WALL);
        addChild(scene);

        // Bullets and Bombs
        scene = new Scene("Bomb_Bullet_Scene");
        scene->addModule(G_BOMB);
        scene->addModule(G_BULLET);
        addChild(scene);

        // Flags
        scene = new Scene("Flag_Scene");
        scene->addModule(G_FLAG);
        addChild(scene);

		// Flash effet
		scene = new Scene("Flash_Scene");
		scene->addModule(G_FLASH);
		addChild(scene);

		// Players
        scene = new Scene("Player_Scene");
        scene->addModule(G_PLAYER);
        addChild(scene);

        // Loading done
        _fullyStarted = true;

		VC_INFO_CRITICAL("GraphicEngine::Ready");
    }

    /////////////////////////////////////////////////////////////////////
    /////	Stop
    /////////////////////////////////////////////////////////////////////

    void GraphicEngine::stop(void)
    {
		VC_INFO_CRITICAL("GraphicEngine::Stopping");

		// Flush the graphic queue
		_context->update();

		// Delete childs
        for (auto it = _childs.begin(); it != _childs.end(); ++it)
        {
            (*it)->stop();
            delete *it;
        }
        _childs.clear();

		// Stop managers
		S_Shader->stop();
		S_Texture->stop();
		S_Font->stop();
		S_Model->stop();

        // Delete loading screen
        _loading->stop();
        safeDelete(_loading);

        // Delete HUD
        _hud->stop();
        safeDelete(_hud);

        // Delete Renderer
        _renderer->stop();
        safeDelete(_renderer);

		// Stop GUI stuff
		S_GUI->stop();

		// Delete the window and the OpenGL context
		_context->stop();
		safeDelete(_context);

		// Wait a bit for everything to be done
		VC_INFO_CRITICAL("GraphicEngine::Waiting a bit ...");
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		_fullyStarted = false;

		VC_INFO_CRITICAL("GraphicEngine::Stopped");
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update
    /////////////////////////////////////////////////////////////////////

    eGameState GraphicEngine::update(const sf::Time &deltaTime)
    {
		// Restore intial opengl states
        _context->restoreDefaultOpenGLStates();

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Wait for server configuration
        if (waitForServerConfiguration(deltaTime.asSeconds()) == true)
            return RUN;

        // Update the global camera
        updateGlobalCamera();

        // Enabled renderer
        _renderer->bindMainTarget();

        // Update scenes
        updateChilds(deltaTime.asSeconds());

		// Apply Post-processing and display result on screen
		_renderer->postProcess();

		// Display non post processed stuff
        updateChilds(deltaTime.asSeconds());

        // Display game HUD
        _hud->update(deltaTime.asSeconds());

        // Display GUI
        S_GUI->draw();

        // Display cursor on top of everything
        _hud->displayCursor();

		// Refresh the screen
        _context->update();

        // Clear renderer targets (Mandatory???)
        // _renderer->clearTargets();

		return RUN;
    }

    void GraphicEngine::updateChilds(float deltatime)
    {
        static int renderPass = 0;

        // Update childs during the first pass
        if (renderPass == 0)
        {
            for (auto it = _childs.begin(); it != _childs.end(); ++it)
            {
                (*it)->update(deltatime, renderPass);
            }
            ++renderPass;
        }

        // Call post rendering functions
        else
        {
            while (renderPass != GRAPHIC_PASS_NUMBER)
            {
                for (auto it = _childs.begin(); it != _childs.end(); ++it)
                {
                    (*it)->update(deltatime, renderPass);
                }
                ++renderPass;
            }
        }

        // Reset the rendering pass
        if (renderPass >= GRAPHIC_PASS_NUMBER)
        {
            renderPass = 0;
        }
    }

    /////////////////////////////////////////////////////////////////////
    /////	Check if we recieved the server configuration file
    /////////////////////////////////////////////////////////////////////

    bool GraphicEngine::waitForServerConfiguration(float deltaTime)
    {
        (void)deltaTime;

		if (_fullyStarted == true)
		{
			return false;
		}

        bool ret = true;
        auto events = Event::getEventByType(ev_CONFIG_RECEIVED);

        // Configuration recieved
		if (events != NULL || G_isOffline == true || _configAlreadyRecieved == true)
		{
            _fullyStarted = true;
			_configAlreadyRecieved = true;

			// Continue the loading
			continueStart();

			// Display loading screen
            _loading->update(90);

            ret = false;
        }
        // Still waiting for configuration
        else
        {
            _loading->update(80);
        }

        // Refresh the screen
        _context->update();

        return ret;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Restart the graphic engine
    /////////////////////////////////////////////////////////////////////

    void GraphicEngine::reboot(void)
    {
    }

    /////////////////////////////////////////////////////////////////////
    /////	Add a new child
    /////////////////////////////////////////////////////////////////////

    void GraphicEngine::addChild(Scene *scene)
    {
        scene->start();
        _childs.push_back(scene);
    }
}

/********************************************************************
**
**              Scene.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "Scene.hpp"
#include "GraphicConfiguration.hpp"
#include "Log.hpp"
#include "Map.hpp"
#include "GraphicDefines.hpp"

namespace VoidClashGraphics
{
    /////////////////////////////////////////////////////////////////////
    /////	Ctor/Dtor
    /////////////////////////////////////////////////////////////////////

    Scene::Scene(const std::string &name)
    {
        _name = name;
        _active = false;
        _camera = new ogl::Camera();
        _autoUpdateCamera = true;
    }

    Scene::~Scene(void)
    {
        if (_camera != NULL)
        {
            safeDelete(_camera);
        }
    }

    /////////////////////////////////////////////////////////////////////
    /////	Start
    /////////////////////////////////////////////////////////////////////

    void Scene::start(void)
    {
        VC_INFO("Scene::Enable " + _name);

        // Create the camera
        if (_camera != NULL)
        {
            _camera->setupPerspective(
                45.0f,
            	(float)GCONFIG.winX / (float)GCONFIG.winY,
            	100.0f,
            	50000.0f
            );
            _camera->update();
        }

        _active = true;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Stop
    /////////////////////////////////////////////////////////////////////

    void Scene::stop(void)
    {
        VC_INFO("Scene::Disable " + _name);

        // Stop modules
        auto it = _childs.begin();
        auto end = _childs.end();

        while (it != end)
        {
            (*it)->stop();
            delete *it;
            it = _childs.erase(it);
        }

        _active = false;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update
    /////////////////////////////////////////////////////////////////////

    void Scene::update(float deltatime, int renderPass)
    {
        if (_active == false)
        {
            return;
        }

        auto it = _childs.begin();
        auto end = _childs.end();

        // First pass, render post-processed stuff and update entities
        if (renderPass == 0)
        {
            // Update the Camera
            if (_autoUpdateCamera == true)
            {
                updateCamera(deltatime);
            }

            while (it != end)
            {
                (*it)->update(deltatime);
                ++it;
            }
        }

        // Other pass, render non processed stuff
        else
        {
            while (it != end)
            {
                (*it)->postRendering(deltatime, renderPass);
                ++it;
            }
        }
    }

	/////////////////////////////////////////////////////////////////////
    /////	Update the camera
    /////////////////////////////////////////////////////////////////////

    void Scene::updateCamera(float deltatime)
    {
        (void)deltatime;

        // Get player to follow
        std::shared_ptr<Player> player = S_Map->getPlayerFollowed();

        // Camera position
        glm::vec2 pos(0.0f, 0.0f);

        if (player != NULL)
        {
            pos.x = player->getX();
            pos.y = -player->getY();
        }

        // Update the camera
        _camera->lookAt(
            glm::vec3(pos.x, pos.y, S_Map->getZoom() * COEFF_ZOOM_MULT),
            glm::vec3(pos.x, pos.y, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

	     _camera->update();
    }

    /////////////////////////////////////////////////////////////////////
    /////	Add/Remove a module to the scene
    /////////////////////////////////////////////////////////////////////

    void Scene::addModule(gObjectType type)
    {
        // Create the module
        AModule *module = S_ModuleFactory->createModule(type);

        // Add the module
        if (module != NULL)
        {
            module->setCamera(_camera);
            module->start();
            _childs.push_back(module);
        }
    }

    /////////////////////////////////////////////////////////////////////
    /////	Setters
    /////////////////////////////////////////////////////////////////////

    void Scene::setCamera(ogl::Camera *camera)
    {
        if (_camera != NULL)
        {
            safeDelete(_camera);
        }

        _camera = camera;
    }

    void Scene::disableCameraAutoUpdate(void)
    {
        _autoUpdateCamera = false;
    }
}

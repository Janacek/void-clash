/********************************************************************
**
**              Environment.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "Environment.hpp"
#include "Cubemap.hpp"
#include "Model.hpp"
#include "ModelManager.hpp"
#include "ShaderManager.hpp"
#include "Log.hpp"
#include "AssetPath.h"
#include "GraphicHelper.hpp"
#include "GraphicConfiguration.hpp"

namespace VoidClashGraphics
{
    /////////////////////////////////////////////////////////////////////
    /////	Ctor/Dtor
    /////////////////////////////////////////////////////////////////////

    Environment::Environment(void) : AModule()
    {
        _active = false;
    }

    Environment::~Environment(void)
    {

    }

    /////////////////////////////////////////////////////////////////////
    /////	Start
    /////////////////////////////////////////////////////////////////////

    void Environment::start(void)
    {
        VC_INFO("Environment::Starting");

        // ---------- Skybox ---------- //

        // Load the skybox Model
        _skybox = S_Model->addModel("Cube/Cube.obj", true);

        // Load the cubemap Texture
        _cubemap = new ogl::Cubemap();
        _cubemap->create(ASSETS_PATH + "assets/img/Skybox");

        // Load the Shader
        _skyboxShader = S_Shader->addShader("Model_Skybox");

        _angle = std::rand() % 720 - 360;
        _active = true;

        // Setup the camera during the first frame
        _refreshCamera = true;

        VC_INFO("Environment::Started");
    }

    /////////////////////////////////////////////////////////////////////
    /////	Stop
    /////////////////////////////////////////////////////////////////////

    void Environment::stop(void)
    {
        VC_INFO("Environment::Stopping");

        _active = false;

        _skybox = NULL;
        _skyboxShader = NULL;

        safeDelete(_cubemap);

        VC_INFO("Environment::Stopped");
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update
    /////////////////////////////////////////////////////////////////////

    void Environment::update(float deltatime)
    {
        static float offset = std::rand() % 720 - 360;
        float skyboxSize = 4.0;

        // ---------- Skybox ---------- //

        // Refresh the camera if needed
        if (_refreshCamera == true)
        {
            updateCamera();
            _refreshCamera = false;
        }

        _angle += 0.25 * deltatime;

        // Calculate angles
        float angleSkybox1 = std::fmod(_angle, 360.0f);
        float angleSkybox2 = std::fmod(-_angle * 0.75 + offset, 360.0f);

        // Send camera matrix on GPU
        _skyboxShader->bind();
        _skyboxShader->setParameter("Camera", _camera->getFinal());

        // Bind cubemap texture and disable depth mask
        _cubemap->bind();
        glDepthMask(GL_FALSE);

        // Draw first skybox
        _skybox->resetTransform();
        _skybox->scale(skyboxSize, skyboxSize, skyboxSize);
        _skybox->rotate(angleSkybox1, ogl::Xaxis);
        _skybox->rotate(angleSkybox1, ogl::Yaxis);
        _skybox->rotate(angleSkybox1, ogl::Zaxis);
        _skyboxShader->setParameter("Model", _skybox->getTransform());
        _skyboxShader->setParameter("Color", 1.0, 0.5, 0.5, 0.75);
        _skybox->draw();

        // Draw second skybox
        _skybox->resetTransform();
        _skybox->scale(skyboxSize, skyboxSize, skyboxSize);
        _skybox->rotate(angleSkybox2, ogl::Xaxis);
        _skybox->rotate(angleSkybox2, ogl::Yaxis);
        _skybox->rotate(angleSkybox2, ogl::Zaxis);
        _skyboxShader->setParameter("Camera", _camera->getFinal());
        _skyboxShader->setParameter("Model", _skybox->getTransform());
        _skyboxShader->setParameter("Color", 0.5, 1.0, 1.0, 0.75);
        // _skybox->draw();
        //
        // // Reset OpenGL States
        glDepthMask(GL_TRUE);
        _cubemap->unbind();
        _skyboxShader->unbind();

        // ---------- Other? :-) ---------- //
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update the camera
    /////////////////////////////////////////////////////////////////////

    void Environment::updateCamera(void)
    {
        // Setup camera
        _camera->setupPerspective(
            45.0f,
            (float)GCONFIG.winX / (float)GCONFIG.winY,
            1.0f,
            50.0f
        );

        _camera->lookAt(
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
        _camera->update();
    }

    /////////////////////////////////////////////////////////////////////
    /////	Render non post processed stuff
    /////////////////////////////////////////////////////////////////////

    void Environment::postRendering(float deltatime, int renderPass)
    {
        (void)deltatime;
        (void)renderPass;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Set the module type
    /////////////////////////////////////////////////////////////////////

    void Environment::addType(gObjectType type)
    {
        _types.push_back(type);
    }

    /////////////////////////////////////////////////////////////////////
    /////	Enable/Disable the module
    /////////////////////////////////////////////////////////////////////

    void Environment::setActive(bool active)
    {
        _active = active;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Return the module type
    /////////////////////////////////////////////////////////////////////

    std::vector<gObjectType>& Environment::getTypeChild(void)
    {
        return _types;
    }
}

/********************************************************************
**
**              LoadingScreen.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "LoadingScreen.hpp"
#include "Log.hpp"
#include "Context.hpp"
#include "AssetPath.h"
#include "GraphicHelper.hpp"

namespace VoidClashGraphics
{
    /////////////////////////////////////////////////////////////////////
    /////	Ctor/Dtor
    /////////////////////////////////////////////////////////////////////

    LoadingScreen::LoadingScreen(void)
    {
        _quad = NULL;
        _shader = NULL;
        _texture = NULL;
    }

    LoadingScreen::~LoadingScreen(void)
    {

    }

    /////////////////////////////////////////////////////////////////////
    /////	Start
    /////////////////////////////////////////////////////////////////////

    void LoadingScreen::start(void)
    {
        // Create Quad
        _quad = new ogl::Quad();
        _quad->create();
        _quad->resetTransform();
        _quad->scale(2.0f, 2.0f);

        // Get Shader
        _shader = new ogl::Shader();
        _shader->loadFromFile(ASSETS_PATH + "assets/shaders/Loading_screen.vert",
            ASSETS_PATH + "assets/shaders/Loading_screen.frag");
        _shader->finalize("LoadingScreen.shader");
        _shader->bind();
        _shader->setParameter("Model", _quad->getTransform());
        _shader->unbind();

        // Get Texture
        _texture = new ogl::Texture();
        _texture->create(ASSETS_PATH + "assets/img/Loading_screen.png");

        GL_CHECK;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update
    /////////////////////////////////////////////////////////////////////

    void LoadingScreen::update(float progress)
    {
        VC_INFO("GraphicEngine::LoadingScreen " + std::to_string((int)progress) + "%");

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind texture
        _texture->bind();

        // Update progress
        _shader->bind();
        _shader->setParameter("Progress", progress * 0.01f);

        // Draw
        glDisable(GL_DEPTH_TEST);
        _quad->draw();
        glEnable(GL_DEPTH_TEST);

        // Clear
        _texture->unbind();
        _shader->unbind();

        // Refresh the screen
        S_Context->update();

        GL_CHECK;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Stop
    /////////////////////////////////////////////////////////////////////

    void LoadingScreen::stop(void)
    {
        safeDelete(_shader);
        safeDelete(_texture);

        _quad->destroy();
        safeDelete(_quad);
    }
}

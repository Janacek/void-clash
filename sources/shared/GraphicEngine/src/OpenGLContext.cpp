/********************************************************************
**
**              OpenGLContext.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include <cstring>
#include "OpenGLContext.hpp"
#include "GraphicConfiguration.hpp"
#include "GraphicHelper.hpp"

#include "Defines.h"
#include "Log.hpp"

namespace VoidClashGraphics
{
    /////////////////////////////////////////////////////////////////////
    /////	Ctor/Dtor
    /////////////////////////////////////////////////////////////////////

    OpenGLContext::OpenGLContext(void)
    {
        std::memset(&_currentState, 0, sizeof(OpenGLState));
        std::memset(&_previousState, 0, sizeof(OpenGLState));
        std::memset(&_defaultState, 0, sizeof(OpenGLState));

        // Setup default OpenGL states
        _defaultState.blend_enabled = true;
        _defaultState.blend_srcAlpha = GL_SRC_ALPHA;
        _defaultState.blend_destAlpha = GL_ONE_MINUS_SRC_ALPHA;
        _defaultState.depth_enabled = true;
        _defaultState.clearColor = glm::vec4(0, 0, 0, 1);
    }

    OpenGLContext::~OpenGLContext(void)
    {

    }

    /////////////////////////////////////////////////////////////////////
    /////	Start
    /////////////////////////////////////////////////////////////////////

    void OpenGLContext::start(void)
    {
        VC_INFO("OpenGLContext::Creating Window and OpenGLContext");

        // Get original video mode
        const GLFWvidmode *originalMode = NULL;
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        if (monitor != NULL)
        originalMode = glfwGetVideoMode(monitor);

        // Resolution
        GLFWvidmode mode;
        mode.width = GCONFIG.winX;
        mode.height = GCONFIG.winY;
        if (originalMode != NULL)
        {
            mode.redBits = originalMode->redBits;
            mode.greenBits = originalMode->greenBits;
            mode.blueBits = originalMode->blueBits;
        }

        // Disable resizable window
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        // Set the refreshRate limite if setted
        if (GCONFIG.fpslimit > 0)
            glfwWindowHint(GLFW_REFRESH_RATE, GCONFIG.fpslimit);

        // Remove the border if setted
        if (GCONFIG.borderless == true)
            glfwWindowHint(GLFW_DECORATED, GL_FALSE);

                    // Create window
        std::string title("VoidClash v" + std::to_string(VOID_CLASH_VERSION));
        S_Context->createWindow(mode, title, GCONFIG.fullscreen);

        // VSync
        if (GCONFIG.vsync)
            glfwWindowHint(GLFW_DOUBLEBUFFER, true);

        // Disable the default cursor
        glfwSetInputMode(&(S_Context->getWindow()), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        // Set default OpenGL states
        restoreDefaultOpenGLStates();

        // Dump the OpenGL drivers property
        printDriverProperty();

        VC_INFO("OpenGLContext::Window and OpenGLContext has been created");
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update
    /////////////////////////////////////////////////////////////////////

    void OpenGLContext::update(void)
    {
        S_Context->update();
    }

    /////////////////////////////////////////////////////////////////////
    /////	Stop
    /////////////////////////////////////////////////////////////////////

    void OpenGLContext::stop(void)
    {
		S_Context->destroyRessources();
    }

    /////////////////////////////////////////////////////////////////////
    /////	Save current opengl states
    /////////////////////////////////////////////////////////////////////

    void OpenGLContext::saveCurrentOpenGLState(void)
    {

    }

    /////////////////////////////////////////////////////////////////////
    /////	Restore previous opengl states
    /////////////////////////////////////////////////////////////////////

    void OpenGLContext::restorePreviousOpenGLStates(void)
    {

    }

    /////////////////////////////////////////////////////////////////////
    /////	Restore default opengl states
    /////////////////////////////////////////////////////////////////////

    void OpenGLContext::restoreDefaultOpenGLStates(void)
    {
        // Blending
        if (_defaultState.blend_enabled == true)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        else
        {
            glDisable(GL_BLEND);
        }

        // Depth test
        if (_defaultState.depth_enabled == true)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }

        // Clear color
        glm::vec4 &color(_defaultState.clearColor);
        glClearColor(color.r, color.g, color.b, color.a);

        // Enable screen framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    /////////////////////////////////////////////////////////////////////
    /////	Save current framebuffer
    /////////////////////////////////////////////////////////////////////

    void OpenGLContext::saveCurrentFramebuffer(void)
    {
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &_previousState.fbo);
    }

    /////////////////////////////////////////////////////////////////////
    /////	Restore previous framebuffer
    /////////////////////////////////////////////////////////////////////*

    void OpenGLContext::restorePreviousFramebuffer(void)
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _previousState.fbo);
    }
}

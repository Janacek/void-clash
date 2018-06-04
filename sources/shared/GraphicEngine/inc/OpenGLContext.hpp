/********************************************************************
**
**              OpenGLContext.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOIDCLASH_OPENGL_CONTEXT
#define VOIDCLASH_OPENGL_CONTEXT

#include "Context.hpp"

namespace VoidClashGraphics
{
    struct OpenGLState
    {
        // Blend
        bool blend_enabled;
        GLint blend_srcAlpha;
        GLint blend_destAlpha;

        // Depth
        bool depth_enabled;

        // Framebuffer
        GLint fbo;

        // Clear color
        glm::vec4 clearColor;
    };

    class OpenGLContext
    {
    public:
        OpenGLContext(void);
        virtual ~OpenGLContext(void);

        void start(void);
        void update(void);
        void stop(void);

        // Framebuffers handling
        void saveCurrentFramebuffer(void);
        void restorePreviousFramebuffer(void);

        // Basic OpenGL states
        void saveCurrentOpenGLState(void);
        void restorePreviousOpenGLStates(void);
        void restoreDefaultOpenGLStates(void);

    private:
        // Handle the OpenGL states
        OpenGLState _currentState;
        OpenGLState _previousState;
        OpenGLState _defaultState;
    };
}

#endif /* end of include guard: VOIDCLASH_OPENGL_CONTEXT */

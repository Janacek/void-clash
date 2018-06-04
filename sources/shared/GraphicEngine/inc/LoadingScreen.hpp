/********************************************************************
**
**              LoadingScreen.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_LOADING_SCREEN
#define VOID_CLASH_LOADING_SCREEN

#include "VoidClash_OpenGL.h"
#include "Quad.hpp"
#include "ShaderManager.hpp"
#include "TextureManager.hpp"

namespace VoidClashGraphics
{
    class LoadingScreen
    {
    public:
        LoadingScreen(void);
        ~LoadingScreen(void);

        void start(void);
        void update(float progress);
        void stop(void);

    private:
        ogl::Quad *_quad;
        ogl::Shader *_shader;
        ogl::Texture *_texture;
    };
}

#endif /* end of include guard: VOID_CLASH_LOADING_SCREEN */

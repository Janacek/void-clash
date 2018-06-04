/********************************************************************
**
**              Renderer.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_RENDERER
#define VOID_CLASH_RENDERER

#include "Framebuffer.hpp"
#include "Quad.hpp"
#include "Shader.hpp"

namespace VoidClashGraphics
{
    class Renderer
    {
    public:
        Renderer(void);
        ~Renderer(void);

        void start(void);
        void stop(void);

        void bindMainTarget(void);
        void postProcess(void);
        void clearTargets(void);

    private:
        ogl::Framebuffer* processFxaa(ogl::Framebuffer* current);
        ogl::Framebuffer* processBloom(ogl::Framebuffer* current);
        void display(ogl::Framebuffer* scene1, ogl::Framebuffer* scene2);

    private:
        bool _ready;

        // Display
        ogl::Quad *_quad;

        // Main Framebuffer
        ogl::Framebuffer *_main;
        ogl::Framebuffer *_multiSampled;

        // Fxaa
        ogl::Framebuffer *_fxaaBuffer;

        // ----- Bloom ----- //

        // Downsample
        ogl::Framebuffer *_halfBuffer;

        // Gaussian blur
    	ogl::Framebuffer *_quarterBuffer1;
    	ogl::Framebuffer *_quarterBuffer2;
    };
}

#endif /* end of include guard: VOID_CLASH_RENDERER */

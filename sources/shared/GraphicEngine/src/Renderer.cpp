/********************************************************************
**
**              Renderer.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "Renderer.hpp"
#include "GraphicConfiguration.hpp"
#include "GraphicHelper.hpp"
#include "Log.hpp"
#include "ShaderManager.hpp"
#include "TextureManager.hpp"
#include "Context.hpp"

namespace VoidClashGraphics
{
    /////////////////////////////////////////////////////////////////////
    /////	Ctor/Dtor
    /////////////////////////////////////////////////////////////////////

    Renderer::Renderer(void)
    {
        _quad = NULL;
        _main = NULL;
        _fxaaBuffer = NULL;
        _halfBuffer = NULL;
        _quarterBuffer1 = NULL;
        _quarterBuffer2 = NULL;
        _ready = false;
    }

    Renderer::~Renderer(void)
    {

    }

    /////////////////////////////////////////////////////////////////////
    /////	Start
    /////////////////////////////////////////////////////////////////////

    void Renderer::start(void)
    {
        VC_INFO("Renderer::Initialize post-processing utils");

        ogl::Shader *s = NULL;

        // Display
        _quad = new ogl::Quad();
        _quad->create();
        _quad->scale(2.0f, 2.0f);

        // Create Framebuffers
        _main = new ogl::Framebuffer();
        _fxaaBuffer = new ogl::Framebuffer();
        _halfBuffer = new ogl::Framebuffer();
        _quarterBuffer1 = new ogl::Framebuffer();
        _quarterBuffer2 = new ogl::Framebuffer();

        // ----- Multisampled -----//

        _multiSampled = new ogl::Framebuffer();
        _multiSampled->create(GCONFIG.winX, GCONFIG.winY, true, GL_CLAMP_TO_EDGE, 1, 8);

        // ----- Main target -----//

        _main->create(GCONFIG.winX, GCONFIG.winY, false);

        // ----- Fxaa -----//

        _fxaaBuffer->create(GCONFIG.winX, GCONFIG.winY, false);
        s = S_Shader->addShader("FXAA");
        s->setParameter("TextureSize", 1.0f / (float)GCONFIG.winX, 1.0f / (float)GCONFIG.winY);
        s->setParameter("Model", _quad->getTransform());
        s->setParameter("Texture", 0);

        GL_CHECK;

        // ----- Bloom -----//

        _halfBuffer->create(GCONFIG.winX * 0.5f, GCONFIG.winY * 0.5, false);
        _quarterBuffer1->create(GCONFIG.winX * 0.25f, GCONFIG.winY * 0.25, false);
        _quarterBuffer2->create(GCONFIG.winX * 0.25f, GCONFIG.winY * 0.25, false);

        s = S_Shader->addShader("Downsample");
        s->setParameter("Model", _quad->getTransform());

        s = S_Shader->addShader("HBlur");
        s->setParameter("Model", _quad->getTransform());
        s->setParameter("PixelSize", 1.0f / (GCONFIG.winX * 0.25f),
            1.0f / (GCONFIG.winY * 0.25f));

        s = S_Shader->addShader("VBlur");
        s->setParameter("Model", _quad->getTransform());
        s->setParameter("PixelSize", 1.0f / (GCONFIG.winX * 0.25f),
            1.0f / (GCONFIG.winY * 0.25f));

        GL_CHECK;

        // ----- Shaders -----//

        s = S_Shader->addShader("MapQuadCombine");
        s->setParameter("Model", _quad->getTransform());
        s->setParameter("Texture1", 0);
        s->setParameter("Texture2", 1);

        s = S_Shader->addShader("MapQuad");
        s->setParameter("Model", _quad->getTransform());
        s->setParameter("Texture", 0);

        GL_CHECK;

        // Reset current binded target
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        _ready = true;

        VC_INFO("Renderer::Post-processing utils initialized");
    }

    /////////////////////////////////////////////////////////////////////
    /////	Stop
    /////////////////////////////////////////////////////////////////////

    void Renderer::stop(void)
    {
        if (_ready == false)
            return;

        _main->destroy();
        _fxaaBuffer->destroy();
        _quad->destroy();
        _halfBuffer->destroy();
        _quarterBuffer1->destroy();
        _quarterBuffer2->destroy();

        delete _quad;
        delete _main;
        delete _fxaaBuffer;
        delete _halfBuffer;
        delete _quarterBuffer1;
        delete _quarterBuffer2;

        _multiSampled->destroy();
        delete _multiSampled;

        _ready = false;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Clear
    /////////////////////////////////////////////////////////////////////

    void Renderer::clearTargets(void)
    {
        _main->bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // FXAA
        if (GCONFIG.fxaa)
        {
            _fxaaBuffer->bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        // Bloom
        if (GCONFIG.bloom)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            _halfBuffer->bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            _quarterBuffer1->bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            _quarterBuffer2->bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        GL_CHECK;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Apply post-processing
    /////////////////////////////////////////////////////////////////////

    void Renderer::postProcess(void)
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Copy the multisampled buffer into the singled buffered
        // main Framebuffer
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _multiSampled->getId());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _main->getId());
		glBlitFramebuffer(0, 0, GCONFIG.winX, GCONFIG.winY,
			0, 0, GCONFIG.winX, GCONFIG.winY, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        // Apply post-processing
        ogl::Framebuffer *fxaa = NULL;
    	ogl::Framebuffer *bloom = NULL;
    	ogl::Framebuffer *current = _main;

    	if (GCONFIG.fxaa == true)
    	{
    		fxaa = processFxaa(current);
    		current = fxaa;
    	}

    	if (GCONFIG.bloom == true)
    	{
    		bloom = processBloom(current);
    		current = bloom;
    	}

        // All disabled
    	if (fxaa == NULL && bloom == NULL)
    		display(current, NULL);

    	// Fxaa + bloom
    	else if (fxaa != NULL && bloom != NULL)
    		display(fxaa, bloom);

    	// Only bloom
    	else if (fxaa == NULL && bloom != NULL)
    		display(bloom, _main);

    	// Only fxaa
    	else if (bloom == NULL)
    		display(fxaa, NULL);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glActiveTexture(GL_TEXTURE0);
    }

    /////////////////////////////////////////////////////////////////////
    /////	Display
    /////////////////////////////////////////////////////////////////////

    void Renderer::display(ogl::Framebuffer *scene1, ogl::Framebuffer *scene2)
    {
    	// Bind shader
    	if (scene2 == NULL)
    		S_Shader->bindShader("MapQuad");
    	else
    		S_Shader->bindShader("MapQuadCombine");

        // Bind scene texture
    	scene1->bindTexture(0);
    	if (scene2 != NULL)
    		scene2->bindTexture(1);

    	// Bind screen buffer
    	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    	glViewport(0, 0, S_Context->getWindowSize().x, S_Context->getWindowSize().y);

    	// Display scene
    	_quad->draw();

        // Clear
        S_Shader->unbindShader();
        S_Texture->unbindTexture();
    }

    /////////////////////////////////////////////////////////////////////
    /////	FXAA
    /////////////////////////////////////////////////////////////////////

    ogl::Framebuffer* Renderer::processFxaa(ogl::Framebuffer* current)
    {
        // Bind shader
    	S_Shader->bindShader("FXAA");

    	// Bind framebuffer
    	_fxaaBuffer->bind();

    	// Bind scene texture
    	current->bindTexture(0);

    	// Apply fxaa
    	_quad->draw();

    	// Clean OpenGL states
    	_fxaaBuffer->unbind();
    	S_Shader->unbindShader();

    	return _fxaaBuffer;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Bloom
    /////////////////////////////////////////////////////////////////////

    ogl::Framebuffer* Renderer::processBloom(ogl::Framebuffer* current)
    {
        // Downsample half
    	ogl::Shader *s = S_Shader->bindShader("Downsample");
    	s->setParameter("PixelSize", 1.0f / (GCONFIG.winX * 0.5f),
    		1.0f / (GCONFIG.winY * 0.5f));
    	s->setParameter("BloomPower", GCONFIG.bloomCoeff * 0.1f);

    	_halfBuffer->bind();
    	current->bindTexture(0);
    	glViewport(0, 0, GCONFIG.winX * 0.5f, GCONFIG.winY * 0.5f);

    	_quad->draw();

    	// Downsample quarter
    	s->setParameter("PixelSize", 1.0f / (GCONFIG.winX * 0.25f),
    		1.0f / (GCONFIG.winY * 0.25f));

    	_quarterBuffer1->bind();
    	_halfBuffer->bindTexture(0);
    	glViewport(0, 0, GCONFIG.winX * 0.25f, GCONFIG.winY * 0.25f);

    	_quad->draw();

    	// Horizontal blur
    	s = S_Shader->bindShader("HBlur");
    	_quarterBuffer2->bind();
    	_quarterBuffer1->bindTexture(0);
    	_quad->draw();

    	// Verticle blur
    	s = S_Shader->bindShader("VBlur");
    	_quarterBuffer1->bind();
    	_quarterBuffer2->bindTexture(0);
    	_quad->draw();

    	// Clear OpenGL states
    	_quarterBuffer1->unbind();
    	s->unbind();
    	glViewport(0, 0, GCONFIG.winX, GCONFIG.winY);

    	return _quarterBuffer1;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Bind the main buffer
    /////////////////////////////////////////////////////////////////////

    void Renderer::bindMainTarget(void)
    {
        // Bind the main Framebuffer
        _multiSampled->bind();

        // And clear it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set size drawing size
        glViewport(0, 0, S_Context->getWindowSize().x, S_Context->getWindowSize().y);
    }
}

/********************************************************************
**
**              EntityCapture.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "EntityCapture.hpp"
#include "Capture.hpp"
#include "GraphicHelper.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "GraphicDefines.hpp"

namespace VoidClashGraphics
{
    /////////////////////////////////////////////////////////////////////
    /////	Ctor/Dtor
    /////////////////////////////////////////////////////////////////////

    EntityCapture::EntityCapture(void) : AEntity()
    {
        _progress = NULL;
    }

    EntityCapture::~EntityCapture(void)
    {

    }

    /////////////////////////////////////////////////////////////////////
    /////	Copy entity into this
    /////////////////////////////////////////////////////////////////////

    void EntityCapture::set(AEntity *entity)
    {
        AEntity::set(entity);

        // Dynamic cast
        _capture = std::dynamic_pointer_cast<Capture>(_gameObject);

        if (_capture == NULL)
        {
            _good = false;
            return;
        }

        // Re-create the progress bar
        _progress = new ogl::Quad();
        _progress->create();
    }

    /////////////////////////////////////////////////////////////////////
    /////	Start
    /////////////////////////////////////////////////////////////////////

    void EntityCapture::start(void)
    {
        // Check if the parent still exists
        if (checkParent() == false)
        {
            return;
        }

        // Dynamic cast
        _capture = std::dynamic_pointer_cast<Capture>(_gameObject);

        if (_capture == NULL)
        {
            _good = false;
            return;
        }

        // Create the progress bar
        _progress = new ogl::Quad();
        _progress->create();

        // Everything is fine
        _good = true;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Stop
    /////////////////////////////////////////////////////////////////////

    void EntityCapture::stop(void)
    {
        AEntity::stop();
        _capture = NULL;

        if (_progress != NULL)
        {
            _progress->destroy();
            safeDelete(_progress);
        }
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update
    /////////////////////////////////////////////////////////////////////

    void EntityCapture::update(float deltatime)
    {
        AEntity::update(deltatime);

        // Check if the parent still exists
        if (checkParent() == false)
        {
            return;
        }

        // Double check
        if (!S_Map->getMode()->getProperty()->capture)
        {
            _skipThisFrame = true;
            return;
        }

        // Update geometry
        resetTransform();
        scale(_capture->getWidth(), _capture->getHeight());
        translate(_capture->getX()  + _capture->getWidth() * 0.5,
        	-(_capture->getY()  + _capture->getHeight() * 0.5), Z_CAPTURE);

        // Update attributes
        updateColor();
        _matrix = getTransform();
        _vec2.x = (float)_capture->getWidth() / _capture->getHeight();
        _vec2.y = _gameObject->getTimeSinceCreation() * 0.000001f;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update the color
    /////////////////////////////////////////////////////////////////////

    void EntityCapture::updateColor(void)
    {
        getTeamColor(_vec4, _capture->getTeamControlling());
    }

    /////////////////////////////////////////////////////////////////////
    /////	Render non post-processed stuff
    /////////////////////////////////////////////////////////////////////

    void EntityCapture::postRendering(float deltatime, float renderPass)
    {
        (void)renderPass;
        (void)deltatime;

        // Check if the parent still exists
        if (checkParent() == false)
        {
            return;
        }

        // Double check
    	if (!S_Map->getMode()->getProperty()->capture)
        {
    		return;
        }

    	_progress->resetTransform();
    	_progress->scale(_capture->getWidth() * 0.5, 50);
    	_progress->translate(_capture->getX() + _capture->getWidth() * 0.5, -_capture->getY() + 50);

    	ogl::Shader *s = S_Shader->bindShader("CaptureZoneProgress");
        s->setParameter("Camera", _camera->getFinal());
        s->setParameter("Model", _progress->getTransform());
    	s->setParameter("ControlMax", (float)CONTROL_MAX);
    	s->setParameter("TimeToCapture", (float)TIME_TO_CAPTURE);
    	s->setParameter("ControlBar", _capture->getControledStatus());
    	s->setParameter("ScaleCoeff", _vec2.x);

        glDisable(GL_DEPTH_TEST);
        _progress->draw();
        glEnable(GL_DEPTH_TEST);

        s->unbind();
    }
}

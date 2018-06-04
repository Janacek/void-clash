/********************************************************************
**
**              EntitySpeedfield.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "EntitySpeedfield.hpp"
#include "SpeedField.hpp"
#include "GraphicDefines.hpp"

namespace VoidClashGraphics
{
    /////////////////////////////////////////////////////////////////////
    /////	Ctor/Dtor
    /////////////////////////////////////////////////////////////////////

    EntitySpeedfield::EntitySpeedfield(void) : AEntity()
    {

    }

    EntitySpeedfield::~EntitySpeedfield(void)
    {

    }

    /////////////////////////////////////////////////////////////////////
    /////	Copy entity into this
    /////////////////////////////////////////////////////////////////////

    void EntitySpeedfield::set(AEntity *entity)
    {
        AEntity::set(entity);

        // Dynamic cast into Speedfield
        _speedfield = std::dynamic_pointer_cast<SpeedField>(_gameObject);

        if (_speedfield == NULL)
        {
            _good = false;
        }
    }

    /////////////////////////////////////////////////////////////////////
    /////	Start
    /////////////////////////////////////////////////////////////////////

    void EntitySpeedfield::start(void)
    {
        // Check if the parent still exists
        if (checkParent() == false)
        {
            return;
        }

        // Dynamic cast into Bullet
        _speedfield = std::dynamic_pointer_cast<SpeedField>(_gameObject);

        // If the owner of the bullet is unknown, ignore
        if (_speedfield == NULL)
        {
            _good = false;
            return;
        }

        // Time for instance
        _float = 0.0f;

        // Everything is fine
        _good = true;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Stop
    /////////////////////////////////////////////////////////////////////

    void EntitySpeedfield::stop(void)
    {
        AEntity::stop();
        _speedfield = NULL;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update
    /////////////////////////////////////////////////////////////////////

    void EntitySpeedfield::update(float deltatime)
    {
        AEntity::update(deltatime);

        // Check if the parent still exists
        if (checkParent() == false)
        {
            return;
        }

        // Update the Geometry
        resetTransform();
        scale(_speedfield->getWidth(), _speedfield->getHeight());
        translate(_speedfield->getX()  + _speedfield->getWidth() * 0.5,
            -(_speedfield->getY()  + _speedfield->getHeight() * 0.5), Z_SPEEDFIELD);

        // Update attributes
        updateColor();
        _matrix = getTransform();
        _float += deltatime;

        // Calculate texture coefficient
        _vec2.x = 0.0130f * _speedfield->getWidth();
        _vec2.y = 0.0137f * _speedfield->getHeight();
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update the color
    /////////////////////////////////////////////////////////////////////

    void EntitySpeedfield::updateColor(void)
    {
        _vec4.r = 0.0f;
        _vec4.g = 0.4f;
        _vec4.b = 0.4f;
        _vec4.a = 1.0f;
    }
}

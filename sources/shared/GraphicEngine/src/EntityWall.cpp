/********************************************************************
**
**              EntityWall.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "EntityWall.hpp"
#include "Wall.hpp"
#include "GraphicDefines.hpp"

namespace VoidClashGraphics
{
    /////////////////////////////////////////////////////////////////////
    /////	Ctor/Dtor
    /////////////////////////////////////////////////////////////////////

    EntityWall::EntityWall(void) : AEntity()
    {

    }

    EntityWall::~EntityWall(void)
    {

    }

    /////////////////////////////////////////////////////////////////////
    /////	Copy entity into this
    /////////////////////////////////////////////////////////////////////

    void EntityWall::set(AEntity *entity)
    {
        AEntity::set(entity);

        // Dynamic cast into Bullet
        _wall = std::dynamic_pointer_cast<Wall>(_gameObject);

        if (_wall == NULL)
        {
            _good = false;
        }
    }

    /////////////////////////////////////////////////////////////////////
    /////	Start
    /////////////////////////////////////////////////////////////////////

    void EntityWall::start(void)
    {
        // Check if the parent still exists
        if (checkParent() == false)
        {
            return;
        }

        // Dynamic cast into Bullet
        _wall = std::dynamic_pointer_cast<Wall>(_gameObject);

        if (_wall == NULL)
        {
            _good = false;
            return;
        }

        // Everything is fine
        _good = true;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Stop
    /////////////////////////////////////////////////////////////////////

    void EntityWall::stop(void)
    {
        AEntity::stop();
        _wall = NULL;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update
    /////////////////////////////////////////////////////////////////////

    void EntityWall::update(float deltatime)
    {
        AEntity::update(deltatime);

        // Check if the parent still exists
        if (checkParent() == false)
        {
            return;
        }

        // Update the geometry
        resetTransform();
        scale(_wall->getLen(), _wall->getRadius() * 2.0f, WALL_SIZE);
        translate(_wall->getLen() * 0.5f, 0);
        rotate(-_wall->getAngle(), ogl::Zaxis);
        translate(_wall->getX(), -_wall->getY(), Z_WALL);

        // Update attributes
        updateColor();
        _matrix = getTransform();
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update the color
    /////////////////////////////////////////////////////////////////////

    void EntityWall::updateColor(void)
    {
        _vec4.r = 0;
        _vec4.g = 1;
        _vec4.b = 1;
        _vec4.a = 1;
    }
}

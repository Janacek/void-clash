/********************************************************************
**
**              EntityBullet.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "EntityBullet.hpp"
#include "Bullet.hpp"
#include "GraphicHelper.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "GraphicDefines.hpp"

namespace VoidClashGraphics
{
    /////////////////////////////////////////////////////////////////////
    /////	Ctor/Dtor
    /////////////////////////////////////////////////////////////////////

    EntityBullet::EntityBullet(void) : AEntity()
    {

    }

    EntityBullet::~EntityBullet(void)
    {

    }

    /////////////////////////////////////////////////////////////////////
    /////	Copy entity into this
    /////////////////////////////////////////////////////////////////////

    void EntityBullet::set(AEntity *entity)
    {
        AEntity::set(entity);

        // Dynamic cast
        _bullet = std::dynamic_pointer_cast<Bullet>(_gameObject);

        if (_bullet == NULL || entity == NULL)
        {
            _good = false;
            return;
        }

        _float = entity->_float;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Start
    /////////////////////////////////////////////////////////////////////

    void EntityBullet::start(void)
    {
        // Check if the parent still exists
        if (checkParent() == false || checkOwner() == false)
        {
            return;
        }

        // Dynamic cast into Bullet
        _bullet = std::dynamic_pointer_cast<Bullet>(_gameObject);

        // If the owner is unknown, ignore
        if (_bullet == NULL)
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

    void EntityBullet::stop(void)
    {
        AEntity::stop();
        _bullet = NULL;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update
    /////////////////////////////////////////////////////////////////////

    void EntityBullet::update(float deltatime)
    {
        AEntity::update(deltatime);

        // Check if the parent still exists
        if (checkParent() == false || checkOwner() == false)
        {
            return;
        }

        // Update the Geometry
        float rotation =
            std::atan2(_gameObject->getDirX(), _gameObject->getDirY()) / 3.1415f * 180.0f + 180.0f;

        resetTransform();
        rotate(rotation, ogl::Zaxis);
        scale(_gameObject->getRadius() * 2.0, _gameObject->getRadius() * 2.0);
        translate(_gameObject->getX(), -_gameObject->getY(), Z_BULLET);

        // Update attributes
        updateColor();
        _matrix = getTransform();
        _float += deltatime;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update the color
    /////////////////////////////////////////////////////////////////////

    void EntityBullet::updateColor(void)
    {
        if (_gameObject->getOwner()->getTeam() == 0)
        {
            std::memset(&_vec4, 0, sizeof(glm::vec4));
            return;
        }

        // Get the main player
        std::shared_ptr<Player> mainPlayer = S_Map->getCurrentPlayer();

        // Get the upated color
        getTeamColor(_vec4, _bullet->getOwner()->getTeam(),
            _gameObject->getOwner() == mainPlayer);
    }
}

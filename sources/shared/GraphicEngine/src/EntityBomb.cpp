/********************************************************************
**
**              EntityBomb.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "EntityBomb.hpp"
#include "Bomb.hpp"
#include "GraphicHelper.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "GraphicDefines.hpp"

namespace VoidClashGraphics
{
    /////////////////////////////////////////////////////////////////////
    /////	Ctor/Dtor
    /////////////////////////////////////////////////////////////////////

    EntityBomb::EntityBomb(void) : AEntity()
    {

    }

    EntityBomb::~EntityBomb(void)
    {

    }

    /////////////////////////////////////////////////////////////////////
    /////	Copy entity into this
    /////////////////////////////////////////////////////////////////////

    void EntityBomb::set(AEntity *entity)
    {
        AEntity::set(entity);

        // Dynamic cast
        _bomb = std::dynamic_pointer_cast<Bomb>(_gameObject);

        if (_bomb == NULL)
        {
            _good = false;
        }
    }

    /////////////////////////////////////////////////////////////////////
    /////	Start
    /////////////////////////////////////////////////////////////////////

    void EntityBomb::start(void)
    {
        // Check if the parent still exists
        if (checkParent() == false || checkOwner() == false)
        {
            return;
        }

        // Dynamic cast
        _bomb = std::dynamic_pointer_cast<Bomb>(_gameObject);

        if (_bomb == NULL)
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

    void EntityBomb::stop(void)
    {
        AEntity::stop();
        _bomb = NULL;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update
    /////////////////////////////////////////////////////////////////////

    void EntityBomb::update(float deltatime)
    {
        AEntity::update(deltatime);

        // Check if the parent still exists
        if (checkParent() == false || checkOwner() == false)
        {
            return;
        }

        // Update the Geometry
        resetTransform();
        scale(_gameObject->getRadius() * 2.0, _gameObject->getRadius() * 2.0);
        translate(_gameObject->getX(), -_gameObject->getY(), Z_BOMB);

        // Update attributes
        updateColor();
        _matrix = getTransform();
        _float = _gameObject->getTimeSinceCreation() * 0.000001f;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update the color
    /////////////////////////////////////////////////////////////////////

    void EntityBomb::updateColor(void)
    {
        if (_gameObject->getOwner()->getTeam() == 0)
        {
            std::memset(&_vec4, 0, sizeof(glm::vec4));
            return;
        }

        // Get the main player
        std::shared_ptr<Player> mainPlayer = S_Map->getCurrentPlayer();

        // Get the upated color
        getTeamColor(_vec4, _bomb->getOwner()->getTeam(),
            _gameObject->getOwner() == mainPlayer);
    }
}

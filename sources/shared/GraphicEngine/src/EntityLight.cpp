/********************************************************************
**
**              EntityLight.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "EntityLight.hpp"
#include "GraphicHelper.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "GraphicDefines.hpp"

namespace VoidClashGraphics
{
    /////////////////////////////////////////////////////////////////////
    /////	Ctor/Dtor
    /////////////////////////////////////////////////////////////////////

    EntityLight::EntityLight(void) : AEntity()
    {

    }

    EntityLight::~EntityLight(void)
    {

    }

    /////////////////////////////////////////////////////////////////////
    /////	Copy entity into this
    /////////////////////////////////////////////////////////////////////

    void EntityLight::set(AEntity *entity)
    {
        AEntity::set(entity);
    }

    /////////////////////////////////////////////////////////////////////
    /////	Start
    /////////////////////////////////////////////////////////////////////

    void EntityLight::start(void)
    {
        // Check if the parent still exists
        if (checkParent() == false)
        {
            return;
        }

        // Check owner for some game object
        eObjectType type = _gameObject->getType();
        if (type == BULLET || type == BOMB || type == GRAVITY_FIELD)
        {
            if (_gameObject->getOwner() == NULL)
            {
                _good = false;
                return;
            }
        }

        // Everything is fine
        _good = true;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Stop
    /////////////////////////////////////////////////////////////////////

    void EntityLight::stop(void)
    {
        AEntity::stop();
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update
    /////////////////////////////////////////////////////////////////////

    void EntityLight::update(float deltatime)
    {
        AEntity::update(deltatime);

        // Check if the parent still exists
        if (checkParent() == false)
        {
            return;
        }

        // Check owner for some game object
        eObjectType type = _gameObject->getType();
        if (type == BULLET || type == BOMB || type == GRAVITY_FIELD)
        {
            if (_gameObject->getOwner() == NULL)
            {
                _good = false;
                return;
            }
        }
		else if (type == PLAYER)
		{
			// Get player
			std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(_gameObject);
			if (player != NULL && player->getLife() <= 0)
			{
				_skipThisFrame = true;
				return;
			}
		}

        // Update the Geometry
        resetTransform();
        scale(_gameObject->getRadius() * 10.0, _gameObject->getRadius() * 10.0);
        translate(_gameObject->getX(), -_gameObject->getY(), Z_HALO);

        // Update attributes
        updateColor();
        _matrix = getTransform();
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update the color
    /////////////////////////////////////////////////////////////////////

    void EntityLight::updateColor(void)
    {
        std::shared_ptr<AObject> toCheck = _gameObject;

        // Check owner for some game object
        eObjectType type = _gameObject->getType();
        if (type == BULLET || type == BOMB || type == GRAVITY_FIELD)
        {
            toCheck = _gameObject->getOwner();
        }

        if (toCheck->getTeam() == 0)
        {
            std::memset(&_vec4, 0, sizeof(glm::vec4));
            return;
        }

        // Get the main player
        std::shared_ptr<Player> mainPlayer = S_Map->getCurrentPlayer();

        // Get the upated color
        getTeamColor(_vec4, toCheck->getTeam(),
            toCheck == mainPlayer);
    }
}

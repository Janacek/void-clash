/********************************************************************
**
**              EntityGravity.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "EntityGravity.hpp"
#include "GravityField.hpp"
#include "Player.hpp"
#include "GraphicDefines.hpp"
#include "GraphicHelper.hpp"
#include "Map.hpp"

namespace VoidClashGraphics
{
    /////////////////////////////////////////////////////////////////////
    /////	Ctor/Dtor
    /////////////////////////////////////////////////////////////////////

    EntityGravity::EntityGravity(void) : AEntity()
    {

    }

    EntityGravity::~EntityGravity(void)
    {

    }

    /////////////////////////////////////////////////////////////////////
    /////	Copy the content of the AEntity passed in parameter to this
    /////////////////////////////////////////////////////////////////////

    void EntityGravity::set(AEntity *entity)
    {
        AEntity::set(entity);

        // Dynamic cast into GravityField
        _gravity = std::dynamic_pointer_cast<GravityField>(_gameObject);

        if (_gravity == NULL)
        {
            _good = false;
        }
    }

    /////////////////////////////////////////////////////////////////////
    /////	Start
    /////////////////////////////////////////////////////////////////////

    void EntityGravity::start(void)
    {
        // Check if the parent still exists
        if (checkParent() == false || checkOwner() == false)
        {
            return;
        }

        // Dynamic cast into GravityField
        _gravity = std::dynamic_pointer_cast<GravityField>(_gameObject);

        // If the owner is unknown, ignore
        if (_gravity == NULL)
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

    void EntityGravity::stop(void)
    {
        AEntity::stop();
        _gravity = NULL;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update
    /////////////////////////////////////////////////////////////////////

    void EntityGravity::update(float deltatime)
    {
        AEntity::update(deltatime);

        // Check if the parent and owner still exists
        if (checkParent() == false || checkOwner() == false)
        {
            return;
        }

        // Calculate rotation direction and speed
        float speed = _gravity->getProperty()->pushback_other / 600.0f;
        speed = std::max(-1.0f, speed);
        speed = std::min(1.0f, speed);

        if (speed >= 0.0f && speed < 0.10f)
        {
            speed += 0.25f;
        }
        else if (speed > -0.10f && speed < 0.0f)
        {
            speed -= 0.25f;
        }

        speed = _gameObject->getTimeSinceCreation() * 0.001f * speed;

        // Update the Geometry
        resetTransform();
        rotate(speed, ogl::Zaxis);
        scale(_gameObject->getRadius() * 2.0f, _gameObject->getRadius() * 2.0f);
        translate(_gameObject->getX(), -_gameObject->getY(), Z_GRAVITYFIELD);

        // Update attributes
        updateColor();
        _matrix = getTransform();
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update the color
    /////////////////////////////////////////////////////////////////////

    void EntityGravity::updateColor(void)
    {
        if (_gameObject->getOwner()->getTeam() == 0)
        {
            std::memset(&_vec4, 0, sizeof(glm::vec4));
            return;
        }

        // Get the main player
        std::shared_ptr<Player> mainPlayer = S_Map->getCurrentPlayer();

        // Get the upated color
        getTeamColor(_vec4, _gameObject->getOwner()->getTeam(),
            _gameObject->getOwner() == mainPlayer);
    }

    /////////////////////////////////////////////////////////////////////
    /////	Return the assetId to use
    /////////////////////////////////////////////////////////////////////

    unsigned int EntityGravity::getAssetId(void)
    {
        return COMMON_ASSET_ID;
    }
}

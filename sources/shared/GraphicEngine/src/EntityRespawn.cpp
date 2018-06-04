/********************************************************************
**
**              EntityRespawn.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "EntityRespawn.hpp"
#include "Respawn.hpp"
#include "GraphicHelper.hpp"
#include "Map.hpp"
#include "GraphicDefines.hpp"

namespace VoidClashGraphics
{
    /////////////////////////////////////////////////////////////////////
    /////	Ctor/Dtor
    /////////////////////////////////////////////////////////////////////

    EntityRespawn::EntityRespawn(void) : AEntity()
    {

    }

    EntityRespawn::~EntityRespawn(void)
    {

    }

    /////////////////////////////////////////////////////////////////////
    /////	Copy entity into this
    /////////////////////////////////////////////////////////////////////

    void EntityRespawn::set(AEntity *entity)
    {
        AEntity::set(entity);

        // Dynamic cast into Respawn
        _respawn = std::dynamic_pointer_cast<Respawn>(_gameObject);

        if (_respawn == NULL)
        {
            _good = false;
        }
    }

    /////////////////////////////////////////////////////////////////////
    /////	Start
    /////////////////////////////////////////////////////////////////////

    void EntityRespawn::start(void)
    {
        // Check if the parent still exists
        if (checkParent() == false)
        {
            return;
        }

        // Dynamic cast into Respawn
        _respawn = std::dynamic_pointer_cast<Respawn>(_gameObject);

        if (_respawn == NULL)
        {
            _good = false;
            return;
        }

        // Time for instance = 0;
        _float = 0.0f;

        // Everything is fine
        _good = true;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Stop
    /////////////////////////////////////////////////////////////////////

    void EntityRespawn::stop(void)
    {
        AEntity::stop();
        _respawn = NULL;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update
    /////////////////////////////////////////////////////////////////////

    void EntityRespawn::update(float deltatime)
    {
        AEntity::update(deltatime);

        // Check if the parent still exists
        if (checkParent() == false)
        {
            return;
        }

        // Check If we have to display respawn
        if (_respawn->getTeam() == 0 || !S_Map->getMode()->getProperty()->team)
        {
            _skipThisFrame = true;
            return;
        }

        // Update the Geometry
        resetTransform();
        scale(_respawn->getWidth(), _respawn->getHeight());
        translate(_respawn->getX()  + _respawn->getWidth() * 0.5,
            -(_respawn->getY() + _respawn->getHeight() * 0.5), Z_RESPAWN);

        // Update attributes
        updateColor();
        _float = (float)_respawn->getWidth() / _respawn->getHeight();
        _matrix = getTransform();
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update the color
    /////////////////////////////////////////////////////////////////////

    void EntityRespawn::updateColor(void)
    {
        getTeamColor(_vec4, _respawn->getTeam());
    }

    /////////////////////////////////////////////////////////////////////
    /////	Return the assetId to use
    /////////////////////////////////////////////////////////////////////

    unsigned int EntityRespawn::getAssetId(void)
    {
        if (_gameObject != NULL)
        {
            return _gameObject->getTeam();
        }

        return 0;
    }
}

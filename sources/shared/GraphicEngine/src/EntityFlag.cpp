/********************************************************************
**
**              EntityFlag.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "EntityFlag.hpp"
#include "Flag.hpp"
#include "GraphicHelper.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "GraphicDefines.hpp"

namespace VoidClashGraphics
{
    #define FLAG_DELAY 0.10

    /////////////////////////////////////////////////////////////////////
    /////	Ctor/Dtor
    /////////////////////////////////////////////////////////////////////

    EntityFlag::EntityFlag(void) : AEntity()
    {

    }

    EntityFlag::~EntityFlag(void)
    {

    }

    /////////////////////////////////////////////////////////////////////
    /////	Copy entity into this
    /////////////////////////////////////////////////////////////////////

    void EntityFlag::set(AEntity *entity)
    {
        AEntity::set(entity);

        // Dynamic cast into Flag
        _flag = std::dynamic_pointer_cast<Flag>(_gameObject);
        // Dynamic cast into EntityFlag
        EntityFlag *eFlag = dynamic_cast<EntityFlag*>(entity);

        if (_flag == NULL || eFlag == NULL)
        {
            _good = false;
            return;
        }

        _gotOwnerLastFrame = eFlag->_gotOwnerLastFrame;
        _timer = eFlag->_timer;
        _initialPosition = eFlag->_initialPosition;

        _ownerPosition.clear();
        for (auto it = eFlag->_ownerPosition.begin();
            it != eFlag->_ownerPosition.end(); ++it)
        {
            _ownerPosition.push_back(*it);
        }
    }

    /////////////////////////////////////////////////////////////////////
    /////	Start
    /////////////////////////////////////////////////////////////////////

    void EntityFlag::start(void)
    {
        // Check if the parent still exists
        if (checkParent() == false)
        {
            return;
        }

        // Dynamic cast into Bullet
        _flag = std::dynamic_pointer_cast<Flag>(_gameObject);

        if (_flag == NULL)
        {
            _good = false;
            return;
        }

        _gotOwnerLastFrame = false;
        _timer = 0;

        // Everything is fine
        _good = true;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Stop
    /////////////////////////////////////////////////////////////////////

    void EntityFlag::stop(void)
    {
        AEntity::stop();
        _flag = NULL;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update
    /////////////////////////////////////////////////////////////////////

    void EntityFlag::update(float deltatime)
    {
        AEntity::update(deltatime);

        // Check if the parent still exists
        if (checkParent() == false)
        {
            return;
        }

        // Update the Geometry
        float rotation = _gameObject->getTimeSinceCreation() * 0.0005f;

        resetTransform();
        rotate(rotation, ogl::Zaxis);
        scale(_gameObject->getRadius() * 2.0, _gameObject->getRadius() * 2.0);

        // ---------- Handle the flag state ---------- //

        // The flag has been taken, clear position buffer
        if (_gotOwnerLastFrame == false && _flag->getOwner() != NULL)
        {
            _gotOwnerLastFrame = true;
            _ownerPosition.clear();

            // Save initial position
            _initialPosition = glm::vec2(_flag->getX(), -_flag->getY());
        }
        // The flag is not carried
        else if (_flag->getOwner() == NULL)
        {
            _gotOwnerLastFrame = false;
            _timer = 0;
        }

        // ---------- Handle the flag position ---------- //

        // Delay the flag move a bit to make it follow the player
        if (_flag->getOwner() != NULL)
        {
            _timer += deltatime;
            _ownerPosition.push_back(
                glm::vec2(_flag->getOwner()->getX(), -_flag->getOwner()->getY())
            );

            // We've wait enought, move the flag now
            if (_timer > FLAG_DELAY)
            {
                if (_ownerPosition.size() != 0)
                {
                    translate(
                        _ownerPosition.begin()->x,
                        _ownerPosition.begin()->y,
                        Z_FLAG
                    );
                    _ownerPosition.erase(_ownerPosition.begin());
                }
            }
            // The delay is not reached
            else
            {
                translate(_initialPosition.x, _initialPosition.y, Z_FLAG);
            }
        }
        // The flag is not carried
        else
        {
            translate(_gameObject->getX(), -_gameObject->getY(), Z_FLAG);
        }

        // Update attributes
        updateColor();
        _matrix = getTransform();
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update the color
    /////////////////////////////////////////////////////////////////////

    void EntityFlag::updateColor(void)
    {
        if (_gameObject->getTeam() == 0)
        {
            std::memset(&_vec4, 0, sizeof(glm::vec4));
            return;
        }

        // Get the main player
        std::shared_ptr<Player> mainPlayer = S_Map->getCurrentPlayer();

        // Get the upated color
        getTeamColor(_vec4, _flag->getTeam(),
            _gameObject->getOwner() == mainPlayer);
    }
}

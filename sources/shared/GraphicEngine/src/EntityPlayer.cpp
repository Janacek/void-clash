/********************************************************************
**
**              EntityPlayer.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "EntityPlayer.hpp"
#include "Player.hpp"
#include "GraphicHelper.hpp"
#include "GraphicDefines.hpp"
#include "Map.hpp"
#include "HudRessources.hpp"
#include "Event.hpp"
#include "ConfigParser.hpp"

extern t_config* G_conf;

namespace VoidClashGraphics
{
    /////////////////////////////////////////////////////////////////////
    /////	Ctor/Dtor
    /////////////////////////////////////////////////////////////////////

    EntityPlayer::EntityPlayer(void) : AEntity()
    {
        _nickname = NULL;
        _levelsBar = NULL;
    }

    EntityPlayer::~EntityPlayer(void)
    {

    }

    /////////////////////////////////////////////////////////////////////
    /////	Copy entity into this
    /////////////////////////////////////////////////////////////////////

    void EntityPlayer::set(AEntity *entity)
    {
        AEntity::set(entity);

        // Dynamic cast into Player
        _player = std::dynamic_pointer_cast<Player>(_gameObject);
        EntityPlayer *ePlayer = dynamic_cast<EntityPlayer*>(entity);

        if (_player == NULL || ePlayer == NULL)
        {
            _good = false;
            return;
        }

        // Re-create the Nickname
        _nickname = new ogl::Text();
        _nickname->create(S_Font->getFont("UnispaceRegular.ttf"), "", _camera);

        // Re-create the Levels bar
        _levelsBar = new ogl::Quad();
        _levelsBar->create();

        // Copy PV Lost
        deletePVLost();
        for (auto it = ePlayer->_pvLost.begin(); it != ePlayer->_pvLost.end(); ++it)
        {
            _pvLost.push_back((*it)->copy(_camera));
        }

        // Copy life attributes
        _smoothLife = ePlayer->_smoothLife;
        _realLife = ePlayer->_realLife;
        _lifeDelay = ePlayer->_lifeDelay;

        // Copy rotation attributes
        _currentRotation = ePlayer->_currentRotation;
        _rotationTarget = ePlayer->_rotationTarget;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Start
    /////////////////////////////////////////////////////////////////////

    void EntityPlayer::start(void)
    {
        // Check if the parent still exists
        if (checkParent() == false)
        {
            return;
        }

        // Dynamic cast into Respawn
        _player = std::dynamic_pointer_cast<Player>(_gameObject);

        if (_player == NULL)
        {
            _good = false;
            return;
        }

        // Create the Nickname
        _nickname = new ogl::Text();
        _nickname->create(S_Font->getFont("ExoRegular.otf"), "", _camera);

        // Create the Levels bar
        _levelsBar = new ogl::Quad();
        _levelsBar->create();

        _smoothLife = _player->getLife();
        _realLife = _player->getLife();
        _lifeDelay = LIFE_SMOOTHING_DELAY;

        _rotationTarget = 0;
        _lastDiff = 0;
        _currentRotation = 123456789;

        // Everything is fine
        _good = true;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Stop
    /////////////////////////////////////////////////////////////////////

    void EntityPlayer::stop(void)
    {
        AEntity::stop();
        _player = NULL;

        // Delete nickname
        if (_nickname != NULL)
        {
            _nickname->destroy();
            safeDelete(_nickname);
        }

        if (_levelsBar != NULL)
        {
            _levelsBar->destroy();
            safeDelete(_levelsBar);
        }

        // Delete PVLost
        deletePVLost();
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update
    /////////////////////////////////////////////////////////////////////

    void EntityPlayer::update(float deltatime)
    {
        AEntity::update(deltatime);

        // Check if the parent still exists
        if (checkParent() == false)
        {
            return;
        }

        // Check If we have to display player
        if (_player->getTeam() == 0 || _player->getLife() <= 0)
        {
            _skipThisFrame = true;
            return;
        }

        // Update the rotation animation
        updateRotationAnimation(deltatime);

        // Update the geometry
        updateGeometry(deltatime);

        // Update attributes
        updateColor();
        _matrix = getTransform();
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update the rotation animation
    /////////////////////////////////////////////////////////////////////

    void EntityPlayer::updateRotationAnimation(float deltatime)
    {
        // ----- Get mouse position ----- //
        glm::vec2 mPos(0, 0);

    	// Main player - Use physic mouse position
    	if (_gameObject == S_Map->getCurrentPlayer())
    	{
    		const sf::Vector2i &mouse(HudRessources::getInstance()->getMousePosition());
    		screenToWorld(mouse.x, mouse.y, mPos.x, mPos.y, _camera);
    	}
    	// Other players - Use network mouse position
    	else
    	{
    		mPos = glm::vec2(_player->getAim().first, _player->getAim().second);
    	}

        // ----- Get player position ----- //

        glm::vec2 pPos(_player->getX(), _player->getY());

        // Save the real orientation
        _rotationTarget = 180 +
            G_TO_DEGREE(std::atan2(mPos.x - pPos.x, mPos.y - pPos.y));

        // First time we come here
        if (_currentRotation == 123456789)
        {
            _currentRotation = _rotationTarget;
            return;
        }

        // Clamp the diff
        // 1: Handles the stuck effect if angle are way bigger
        // than ROTATION_MAX_Y
        // 2: It also handles the switch between 0 and 360 degree

        float diff = _rotationTarget - _currentRotation;

        if (diff > ROTATION_MAX_Y)
        {
            _currentRotation = _rotationTarget - _lastDiff;
        }
        else if (diff < -ROTATION_MAX_Y)
        {
            _currentRotation = _rotationTarget - _lastDiff;
        }

        // Calulate the new diff
        diff = _rotationTarget - _currentRotation;

        // Round the diff, target reached
        if (diff > -0.001f && diff < 0.001f)
        {
            _currentRotation = _rotationTarget;
        }

        // Animate smoothly the transition
        else
        {
            _currentRotation = _currentRotation + diff *
                deltatime * ROTATION_SMOOTH_SPEED;
        }

        // Save last diff
        _lastDiff = diff;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update geometry
    /////////////////////////////////////////////////////////////////////

    void EntityPlayer::updateGeometry(float deltatime)
    {
        (void)deltatime;

        // Clamp the Y rotation
        float clampedRotation =
            std::max(
                -ROTATION_MAX_Y,
                std::min(ROTATION_MAX_Y, _currentRotation - _rotationTarget)
            );

        // Use a square function to smooth even more

        // Calulate final angle Y
        float finalAngleY = std::pow(std::abs(clampedRotation), 0.90f) * 1.5f;
        finalAngleY = (clampedRotation < 0) ? (-finalAngleY) : (finalAngleY);

        // Calculate final angle Z
        float diffAngleZ = _rotationTarget - _currentRotation;
        float finalAngleZ = std::pow(std::abs(diffAngleZ), 0.5);
        finalAngleZ = (diffAngleZ < 0) ? (-finalAngleZ) : (finalAngleZ);
        finalAngleZ += _currentRotation;

        // Apply the new transform
        resetTransform();
        scale(
            _player->getRadius() * 2.0f,
            _player->getRadius() * 2.0f,
            _player->getRadius() * 2.0f
        );
        rotate(finalAngleY, ogl::Yaxis);
        rotate(finalAngleZ, ogl::Zaxis);
        translate(_player->getX(), -_player->getY(), Z_PLAYER);
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update color
    /////////////////////////////////////////////////////////////////////

    void EntityPlayer::updateColor(void)
    {
        // Main player
        const std::shared_ptr<Player> &mainPlayer = S_Map->getCurrentPlayer();

        // Set color according to mode
        getTeamColor(_vec4, _player->getTeam(),
            _player == mainPlayer);

        if (_player->isInvulnerable() == true)
        {
            int time = (int)(G_TO_SECONDS(_player->getTimeSinceCreation()) * 6.0);
            _vec4.a = (time % 2 == 0) ? (0.25) : (1.0);
        }
    }

    /////////////////////////////////////////////////////////////////////
    /////	Get the assetId
    /////////////////////////////////////////////////////////////////////

    unsigned int EntityPlayer::getAssetId(void)
    {
        if (_gameObject != NULL)
        {
            return _gameObject->getTeam();
        }

        return 0;
    }
}

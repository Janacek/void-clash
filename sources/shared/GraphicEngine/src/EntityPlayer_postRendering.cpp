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
#include "GraphicHelper.hpp"
#include "Event.hpp"
#include "ConfigParser.hpp"

extern t_config* G_conf;

namespace VoidClashGraphics
{
    /////////////////////////////////////////////////////////////////////
    /////	Remove all pv lost
    /////////////////////////////////////////////////////////////////////

    void EntityPlayer::deletePVLost(void)
    {
        // Delete PVLost vector
        for (auto it = _pvLost.begin(); it != _pvLost.end(); ++it)
        {
            delete (*it);
        }
        _pvLost.clear();
    }

    /////////////////////////////////////////////////////////////////////
    /////	Render non post-processed stuff
    /////////////////////////////////////////////////////////////////////

    void EntityPlayer::postRendering(float deltatime, float renderPass)
    {
        (void)renderPass;

        // Check if the parent still exists
        if (checkParent() == false)
        {
            return;
        }

        // Check If we have to display player
        if (_player->getTeam() == 0 || _player->getLife() <= 0)
        {
			return;
        }

        glDisable(GL_DEPTH_TEST);

        // Update life values
        updateLife(deltatime);

        // Life points lost
        updatePVLost(deltatime);

        // Life and energy bar
        updateLevelsBar(deltatime);

        // Nickname
        updateNickname();

        glEnable(GL_DEPTH_TEST);
    }

    /////////////////////////////////////////////////////////////////////
    ////    Update life and energy bar
    /////////////////////////////////////////////////////////////////////

    void EntityPlayer::updateLevelsBar(float deltatime)
    {
        (void)deltatime;

        // Update geomety
        _levelsBar->resetTransform();
        _levelsBar->scale(220, 35);
        _levelsBar->translate(
            _player->getX(),
            -_player->getY() + _player->getRadius() + 40
        );

        // Update shader
        ogl::Shader *s = S_Shader->bindShader("PlayerLevels");
        s->setParameter("Camera", _camera->getFinal());
        s->setParameter("Model", _levelsBar->getTransform());
        s->setParameter("Life", _realLife / (float)G_conf->player->life);
        s->setParameter("SmoothLife", _smoothLife / (float)G_conf->player->life);
        s->setParameter("Energy", _player->getEnergy() / (float)G_conf->player->max_energy);
        s->setParameter("Time", G_TO_SECONDS(_player->getTimeSinceCreation()));

        // Draw
	    _levelsBar->draw();

        s->unbind();
    }

    /////////////////////////////////////////////////////////////////////
    ////    Update life
    /////////////////////////////////////////////////////////////////////

    void EntityPlayer::updateLife(float deltatime)
    {
        // Update delay
        _lifeDelay -= deltatime;

        // We've wait enought, progressive update of _smoothLife
        if (_smoothLife > _realLife && _lifeDelay <= 0.0f)
        {
            _smoothLife = std::max(
                _realLife,
                _smoothLife - LIFE_SMOOTHING_SPEED * deltatime
            );
        }

        // The player got healed or just respawned or he is dead
        if (_smoothLife < _realLife || _realLife <= 0)
        {
            _smoothLife = _realLife;
        }

        // Update the real player life
        _realLife = _player->getLife();
    }

    /////////////////////////////////////////////////////////////////////
    /////	Nickname
    /////////////////////////////////////////////////////////////////////

    void EntityPlayer::updateNickname(void)
    {
        // Update the Nickname
        _nickname->setString(_player->getNick());
        _nickname->setColor(glm::vec4(1, 1, 1, 1));

        _nickname->resetTransform();
    	_nickname->setScale(1.35f, 1.35f);

        glm::vec2 s = _nickname->getSize();
    	_nickname->setPosition(_player->getX() - s.x * 0.5f,
    		-_player->getY() + _player->getRadius() + 80);

        _nickname->draw();
    }

    /////////////////////////////////////////////////////////////////////
    /////	Nickname
    /////////////////////////////////////////////////////////////////////

    void EntityPlayer::updatePVLost(float deltatime)
    {
        // ---------- Add new PV Lost ---------- //

        auto ev = Event::getEventByType(ev_PLAYER_HIT);
    	if (ev != NULL)
    	{
    		for (auto it = ev->begin(); it != ev->end(); ++it)
    		{
                // I got hit by someone !
    			if (it->second.data == _player.get())
    			{
                    // Reset the delay
                    _lifeDelay = LIFE_SMOOTHING_DELAY;

                    // Get damage
                    int damage = std::abs(_smoothLife - _realLife);

                    // Create the PV Lost
                    PVLost *newPvLost = new PVLost(damage, _camera);
                    _pvLost.push_back(newPvLost);

                    int randRadius = 75;
                    int randX = std::rand() % (_player->getRadius() * 2 + (2 * randRadius)) -
                        (_player->getRadius() + randRadius);
                    int randY = std::rand() % (_player->getRadius() * 2 + (2 * randRadius)) -
                        (_player->getRadius() + randRadius);

                    newPvLost->position = glm::vec2(
                        _player->getX() + randX,
                        -_player->getY() + randY
                    );

                    break;
    			}
    		}
    	}

        // ---------- Update PV Lost ---------- //

        auto it = _pvLost.begin();
        auto end = _pvLost.end();

        while (it != end)
        {
            (*it)->timer += deltatime;

            // Delete PV Lost
            if ((*it)->timer > 0.5f)
            {
                delete (*it);
                it = _pvLost.erase(it);
            }
            else
            {
                (*it)->pvLost->resetTransform();
                (*it)->position.y += 100.0 * deltatime;

                // Calculate size and color according to damage
                float rate = std::max((float)(*it)->damage, _realLife);
                rate = (*it)->damage / rate;
                float scale = 1.75f;
                glm::vec4 color;

                if (rate > 0.75)
                {
                    color = glm::vec4(1, 0, 0, 1);
                }
                else if (rate > 0.5)
                {
                    color = glm::vec4(1, 1, 0, 1); scale *= 0.90;
                }
                else if (rate > 0.25)
                {
                    color = glm::vec4(1, 0.5, 0, 1); scale *= 0.80;
                }
                else
                {
                    color = glm::vec4(1, 1, 1, 1); scale *= 0.70;
                }

                (*it)->pvLost->setColor(color);
                (*it)->pvLost->setScale(scale, scale);
                (*it)->pvLost->setPosition(
                    (*it)->position.x - (*it)->pvLost->getSize().x * 0.5,
                    (*it)->position.y
                );

                // Draw
                (*it)->pvLost->draw();

                ++it;
            }
        }
    }
}

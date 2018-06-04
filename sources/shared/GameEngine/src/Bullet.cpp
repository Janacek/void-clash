//
// Bullet.cpp for Void Clash in /home/gegon/Void-Clash/client/src
//
// Made by brunel guillaume
// Login   <brunel_g@epitech.net>
//
// Started on  Thu Aug  8 21:23:41 2013 brunel guillaume
// Last update Tue Feb 25 17:41:28 2014 Gegon
//

#include	<iostream>
#include	<cmath>
#include	"Bullet.hpp"
#include	"MapParser.hpp"
#include	"Player.hpp"
#include	"Map.hpp"
#include	"Defines.h"
#include	"ConfigParser.hpp"
#include	"Bot.hpp"

extern t_config *G_conf;
extern bool	G_isServer;
extern bool	G_isOffline;

Bullet::Bullet() :
AWeapon(BULLET, 0, 0, 0, 0)
{
	_owner = NULL;
	_clock.restart();
}

Bullet::Bullet(float X, float Y,
	float dirX, float dirY,
	std::shared_ptr<Player>	owner, eObjectType makerType) :
	AWeapon(BULLET, X, Y, dirX, dirY)
{
	_owner = owner;
	_clock.restart();
	_makerType = makerType;
}

Bullet::~Bullet()
{
}

///////////////////////////////////////////////
/////   Called each frame

bool	Bullet::update()
{
	if (!AWeapon::update())
		return false;

	_time = _clock.getElapsedTime().asMilliseconds();
	if (_time > _property->duration * 1000.0f)
		return (false);
	else if (!_property->bounce && checkCollisionWithWalls())
		return (false);

	if (checkHitPlayers() == true || checkHitTurrets() == true || checkHitBots() == true)
		return (false);
	return (true);
}

///////////////////////////////////////////////
/////   Hit related functions

bool	Bullet::checkHitPlayers()
{
	auto	it = S_Map->getPlayers()->begin();
	auto	end = S_Map->getPlayers()->end();
	float	coef;

	while (it != end)
	{
		std::shared_ptr<Player>player = std::dynamic_pointer_cast<Player>(*it);
		if (player->getTeam() != 0 && !player->isRespawning())
		{
			coef = 100;
			if (player == _owner)
				coef = G_conf->game->friendly_fire_own;
			else if (S_Map->getMode()->getProperty()->team &&
				player->getTeam() == (_owner)->getTeam())
				coef = G_conf->game->friendly_fire_team;
			if (isInsideHitbox(player->getX(), player->getY(),
				player->getRadius() + _radius) == true)
			{
				if (coef > 0)
				{
					if (G_isServer || G_isOffline)
					{
						// drain energy
						if (_property->drain_energy)
							player->setEnergy(player->getEnergy() - _property->drain_energy);
						// drain energy
						if (_property->pushback_other)
						{
							float diffX = _property->pushback_other * ((*it)->getX() - getX()) / 100;
							float diffY = _property->pushback_other * ((*it)->getY() - getY()) / 100;
							player->setDirX(diffX);
							player->setDirY(diffY);

							player->setDirX(diffX);
							player->setDirY(diffY);
						}

						player->isHitBy(std::dynamic_pointer_cast<AWeapon>(shared_from_this()), coef * _property->damage / 100);
						return (true);
					}
				}
			}
		}
		++it;
	}
	return (false);
}

bool	Bullet::checkHitBots()
{
	auto	it = S_Map->getBots()->begin();
	auto	end = S_Map->getBots()->end();

	while (it != end)
	{
		Bot *bot = (*it).get();
		if (isInsideHitbox((*it)->getX(), (*it)->getY(), _radius + bot->getRadius()))
		{
			ADD_EVENT(ev_DELETE, s_event(shared_from_this()));
			bot->isHitBy(std::dynamic_pointer_cast<AWeapon>(shared_from_this()), _property->damage);
			return true;
		}
		++it;
	}
	return (false);
}

bool	Bullet::checkHitTurrets()
{
	auto	it = S_Map->getTurrets()->begin();
	auto	end = S_Map->getTurrets()->end();

	while (it != end)
	{
		Turret *turret = (*it).get();
		if (isInsideHitbox((*it)->getX(), (*it)->getY(), _radius + turret->getRadius() * 2))
		{
			int coef = 100;
			if (turret->getOwner() == _owner)
				coef = G_conf->game->friendly_fire_own;
			else if (S_Map->getMode()->getProperty()->team &&
				turret->getOwner()->getTeam() == (_owner)->getTeam())
				coef = G_conf->game->friendly_fire_team;

			// Apply effects
			int damage = _property->damage * coef / 100;
			if (turret && coef)
				ADD_EVENT(ev_DELETE, s_event(shared_from_this()));
			turret->isHitBy(this, damage);
			return true;
		}
		++it;
	}
	return (false);
}

bool	Bullet::isInsideHitbox(int X, int Y, float radius)
{
	if ((std::pow(X - getX(), 2.0f) + std::pow(Y - getY(), 2.0f))
		< (std::pow(radius, 2.0f)))
		return (true);
	return (false);
}

eObjectType	Bullet::getMakerType()
{
	return _makerType;
}

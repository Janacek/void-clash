//
// Explosion.cpp for Void Clash in /home/gegon/PFA/client/GameEngine/inc
// 
// Made by brunel guillaume
// Login   <brunel_g@epitech.net>
// 
// Started on  Mon Sep  2 00:57:18 2013 brunel guillaume
// Last update Tue Feb  4 03:24:39 2014 Gegon
//

#include	<iostream>
#include	<cmath>
#include	"Explosion.hpp"
#include	"Map.hpp"
#include	"Defines.h"
#include	"ConfigParser.hpp"
#include	"Bomb.hpp"

extern t_config *G_conf;
extern bool	G_isServer;
extern bool	G_isOffline;

Explosion::Explosion()
{
	_owner = NULL;
	_clock.restart();
}

Explosion::Explosion(float X, float Y,
	float dirX, float dirY,
	std::shared_ptr<Player>	owner, eObjectType makerType) :
	AWeapon(EXPLOSION, X, Y, dirX, dirY)
{
	_owner = owner;
	_clock.restart();
	_makerType = makerType;
}

Explosion::~Explosion()
{
}

///////////////////////////////////////////////
/////   Called each frame

bool	Explosion::update()
{
	_time = _clock.getElapsedTime().asSeconds();
	if (_time > 1.0)
		return (false);
	return (true);
}

void	Explosion::onStart()
{
	auto	it = S_Map->getElems()->begin();
	auto	end = S_Map->getElems()->end();
	int	coef;

	double distance;
	ADD_EVENT(ev_EXPLOSION, s_event(shared_from_this()));
	while (it != end)
	{
		// Checking if AObject is in range of bomb
		if ((distance = std::pow(getX() - (*it)->getX(), 2.0f) + std::pow(getY() - (*it)->getY(), 2.0f))
			< std::pow(_property->size_explosion, 2.0f))
		{
			// Check if no wall between explo and AObject
			if (checkSegmentSegment(*it) == false)
			{
				// Do damage
				distance = sqrt(distance);
				float	proximity = _property->size_explosion - distance;
				if ((*it)->getType() == BULLET)
					ADD_EVENT(ev_DELETE, s_event(*it));
				// Bomb chain explo
				if ((*it)->getType() == BOMB && (bool)_property->chain == true)
				{
					std::shared_ptr<Bomb>bomb = std::dynamic_pointer_cast<Bomb>(*it);
					if (bomb->isActive() && bomb->getOwner() && bomb->getOwner()->getBombOnHold() != bomb)
					{
						bomb->explode();
						ADD_EVENT(ev_DELETE, *it);
					}
				}
				if ((*it)->getType() == PLAYER)
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

						// Apply effects
						int damage = (proximity / _property->size_explosion) * _property->damage * coef / 100;
						if (coef > 0)
						{
							float	factor = sqrt(std::pow((*it)->getX() - getX(), 2.0f) + std::pow((*it)->getY() - getY(), 2.0f)) + 1;
							float	diffX = ((*it)->getX() - getX()) / factor;
							float	diffY = ((*it)->getY() - getY()) / factor;

							diffX = diffX * _property->pushback_other * proximity / _property->size_explosion;
							diffY = diffY * _property->pushback_other * proximity / _property->size_explosion;

							player->setDirX((*it)->getDirX() + diffX);
							player->setDirY((*it)->getDirY() + diffY);

							// EMP
							if (_property->drain_energy)
								player->setEnergy(player->getEnergy() - _property->drain_energy);

							// DAMAGE
							player->isHitBy(std::dynamic_pointer_cast<AWeapon>(shared_from_this()), damage);
						}
					}
				}
				if ((*it)->getType() == TURRET)
				{
					std::shared_ptr<Turret>turret = std::dynamic_pointer_cast<Turret>(*it);
					coef = 100;
					if (turret->getOwner() == _owner)
						coef = G_conf->game->friendly_fire_own;
					else if (S_Map->getMode()->getProperty()->team &&
						turret->getOwner()->getTeam() == (_owner)->getTeam())
						coef = G_conf->game->friendly_fire_team;

					// Apply effects
					int damage = (proximity / _property->size_explosion) * _property->damage * coef / 100;
					if (coef > 0)
					{
						// DAMAGE
						turret->isHitBy(this, damage);
					}
				}
				if ((*it)->getType() == BOT)
				{
					std::shared_ptr<Bot>bot = std::dynamic_pointer_cast<Bot>(*it);

					// Apply effects
					int damage = (proximity / _property->size_explosion) * _property->damage;
					// DAMAGE
					bot->isHitBy(std::dynamic_pointer_cast<AWeapon>(shared_from_this()), damage);
				}
				// Apply pushback


				//if ((*it)->getType() != GRAVITY_FIELD && (*it)->getType() != SPEED_FIELD &&
				//	(*it)->getType() != EXPLOSION && (*it)->getType() != RESPAWN &&
				//	(*it)->getType() != FLAG)
				//	(*it)->addForce(diffX, diffY);
			}
		}
		++it;
	}
}

// Check if collision with walls
bool	Explosion::checkSegmentSegment(std::shared_ptr<AObject>nmy)
{
	Point A;
	Point B;
	Point O;
	Point P;

	O.x = _pos.first;
	O.y = _pos.second;
	P.x = nmy->getX();
	P.y = nmy->getY();

	auto	it = S_Map->getWalls()->begin();
	auto	end = S_Map->getWalls()->end();
	while (it != end)
	{
		A.x = (*it)->getX();
		B.x = (*it)->getEndX();
		A.y = (*it)->getY();
		B.y = (*it)->getEndY();

		if (CollisionSegSeg(A, B, O, P))
			return true;
		++it;
	}
	return false;
}

bool	Explosion::CollisionSegSeg(Point A, Point B, Point O, Point P)
{
	if (CollisionDroiteSeg(A, B, O, P) == false)
		return false;  // inutile d'aller plus loin si le segment [OP] ne touche pas la droite (AB) 
	if (CollisionDroiteSeg(O, P, A, B) == false)
		return false;
	return true;
}

bool	Explosion::CollisionDroiteSeg(Point A, Point B, Point O, Point P)
{
	Vecteur AO, AP, AB;
	AB.x = B.x - A.x;
	AB.y = B.y - A.y;
	AP.x = P.x - A.x;
	AP.y = P.y - A.y;
	AO.x = O.x - A.x;
	AO.y = O.y - A.y;
	if ((AB.x*AP.y - AB.y*AP.x)*(AB.x*AO.y - AB.y*AO.x) < 0)
		return true;
	else
		return false;
}

eObjectType	Explosion::getMakerType()
{
	return _makerType;
}
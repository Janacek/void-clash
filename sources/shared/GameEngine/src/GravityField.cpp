//
// GravityField.cpp for GravityField.cpp in /home/gegon/PFA/client/GameEngine/src
// 
// Made by brunel guillaume
// Login   <brunel_g@epitech.net>
// 
// Started on  Mon Aug 26 21:43:10 2013 brunel guillaume
// Last update Fri Dec 27 17:49:43 2013 Gegon
//

#include	<list>
#include	<iostream>
#include	<cmath>
#include	"GravityField.hpp"
#include	"Wall.hpp"
#include	"Player.hpp"
#include	"Defines.h"
#include	"ConfigParser.hpp"
#include	"Map.hpp"

extern t_config *G_conf;
extern int sizeX;
extern int sizeY;

GravityField::GravityField(float X, float Y,
	float dirX, float dirY, const std::shared_ptr<Player> &owner) :
	AWeapon(GRAVITY_FIELD, X, Y, dirX, dirY)
{
	_owner = owner;
	_clock.restart();
}

GravityField::GravityField() :
AWeapon(GRAVITY_FIELD, 0, 0, 0, 0)
{
	_owner = NULL;
	_clock.restart();
}

GravityField::~GravityField()
{
}

///////////////////////////////////////////////
/////   Called each frame

bool	GravityField::update()
{
	if (!AWeapon::update())
		return false;

	_time = _clock.getElapsedTime().asMilliseconds();
	if (_time > _property->duration * 1000.0f)
		return (false);

	if (!_owner->isWeaponActive(_property))
		return false;

	// Check each frame effect (pushback)
	checkUpdateEffects();

	if ((_clock.getElapsedTime() - _lastActivityCheck).asMilliseconds() - GRAVITY_FRAME_ACTIVITY < 0.f)
		return true;

	// Check each X frames effect (damage)
	checkFrameActivityEffects();
	return (true);
}

///////////////////////////////////////////////
// Check each frame effect (pushback)
void	GravityField::checkUpdateEffects()
{
	std::list<std::shared_ptr<AObject>>::iterator	it = S_Map->getElems()->begin();
	std::list<std::shared_ptr<AObject>>::iterator	end = S_Map->getElems()->end();

	float distance;
	while (it != end)
	{
		const eObjectType &type = (*it)->getType();
		// Checking if AObject is in range of gravity field
		if (type == PLAYER || type == BOT || type == BULLET || type == BOMB || type == ROCKET || type == TURRET)
		{
			if ((distance = sqrt(std::pow(getX() - (*it)->getX(), 2.0f) + std::pow(getY() - (*it)->getY(), 2.0f)))
				< _radius + (*it)->getRadius())
				applyUpdateEffects(distance, (*it).get());
		}
		++it;
	}
}

void	GravityField::applyUpdateEffects(float distance, AObject *obj)
{
	if (obj == this)
		return;

	Player *player = dynamic_cast<Player *>(obj);
	AWeapon *weapon = dynamic_cast<AWeapon *>(obj);

	if (player && player == _owner.get())
		return;
	if (weapon && weapon->getOwner() == _owner)
		return;

	if (S_Map->getMode()->getProperty()->team)
	{
		if (player && player->getTeam() == _owner->getTeam())
			return;
		if (weapon && weapon->getOwner() && weapon->getOwner()->getTeam() == _owner->getTeam())
			return;
	}

	if (weapon && weapon->getProperty()->speed == 0.f)
		return;

	int proximity = _radius - distance + obj->getRadius();

	// Apply pushback
	float	factor = sqrt(std::pow(obj->getX() - getX(), 2.0f) + std::pow(obj->getY() - getY(), 2.0f)) + 1;
	float	diffX = (obj->getX() - getX()) / factor;
	float	diffY = (obj->getY() - getY()) / factor;

	//diffX = diffX * _property->pushback_other * std::pow(proximity, 2) * _deltaTime.asMilliseconds() / (GRAVITY_PUSHBACK_MITIGATION * std::pow(_property->size, 2));
	//diffY = diffY * _property->pushback_other * std::pow(proximity, 2) * _deltaTime.asMilliseconds() / (GRAVITY_PUSHBACK_MITIGATION * std::pow(_property->size, 2));

	diffX = diffX * _property->pushback_other * _deltaTime.asMilliseconds() / (GRAVITY_PUSHBACK_MITIGATION);
	diffY = diffY * _property->pushback_other * _deltaTime.asMilliseconds() / (GRAVITY_PUSHBACK_MITIGATION);

	float speed = sqrt(pow(obj->getDirX(), 2) + pow(obj->getDirY(), 2));


	if (weapon && speed > weapon->getProperty()->speed)
	{
		if (diffX * obj->getDirX() > 0)
			diffX = 0;
		if (diffY * obj->getDirY() > 0)
			diffY = 0;
	}

	if (player)
		player->isInsideGravityField();
	obj->addForce(diffX, diffY);

	obj->applySlow(_property);
}

///////////////////////////////////////////////
// Check each X frames effect (damage)

void	GravityField::checkFrameActivityEffects()
{
	auto	it = S_Map->getElems()->begin();
	auto	end = S_Map->getElems()->end();
	_lastActivityCheck = _clock.getElapsedTime();

	float distance;
	while (it != end)
	{
		const eObjectType &type = (*it)->getType();
		// Checking if AObject is in range of gravity field
		if (type == PLAYER || type == BOT)
		{
			if ((distance = sqrt(std::pow(getX() - (*it)->getX(), 2.0f) + std::pow(getY() - (*it)->getY(), 2.0f)))
				< _radius + (*it)->getRadius())
				applyFrameActivityEffects(distance, (*it).get());
		}
		++it;
	}
}

void	GravityField::applyFrameActivityEffects(float distance, AObject *obj)
{
	if (obj == this)
		return;

	Player *player = dynamic_cast<Player *>(obj);
	AWeapon *weapon = dynamic_cast<AWeapon *>(obj);

	if (player && player == _owner.get())
		return;
	if (weapon && weapon->getOwner() == _owner)
		return;

	if (S_Map->getMode()->getProperty()->team)
	{
		if (player && player->getTeam() == _owner->getTeam())
			return;
		if (weapon && weapon->getOwner()->getTeam() == _owner->getTeam())
			return;
	}

	if (weapon && weapon->getProperty()->speed == 0.f)
		return;

	int proximity = _radius - distance + obj->getRadius();

	// Apply damage
	if (player)
		player->isHitBy(std::dynamic_pointer_cast<AWeapon>(shared_from_this()), _property->damage * proximity / _property->size);

	// Apply drain energy
	if (player)
		player->setEnergy(player->getEnergy() - _property->drain_energy * proximity / _property->size);
}
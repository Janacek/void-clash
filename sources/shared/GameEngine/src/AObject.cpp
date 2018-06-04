//
// AObject.cpp for Void Clash in /home/gegon/PFA/client/src
// 
// Made by brunel guillaume
// Login   <brunel_g@epitech.net>
// 
// Started on  Mon Aug 19 16:52:05 2013 brunel guillaume
// Last update Mon Jan 20 23:24:29 2014 Gegon
//

#include	<iostream>
#include	<complex>
#include	"AObject.hpp"
#include	"Defines.h"
#include	"ConfigParser.hpp"
#include	"Player.hpp"
#include	"Event.hpp"
#include	"Wall.hpp"

extern t_config *G_conf;
extern bool	G_isServer;
extern int	G_id;

AObject::AObject() :
	_coefDeltaTime(0.0f),
	_timeSinceCreation(0.f),
	_pos(std::pair<float, float>(0.f, 0.f))
{
	_selected = false;
}

AObject::AObject(eObjectType type,
	float X, float Y,
	float dirX, float dirY) :
	_type(type),
	_pos(std::pair<float, float>(X, Y)),
	_prevFramePos(std::pair<float, float>(X, Y)),
	_dir(std::pair<float, float>(dirX, dirY)),
	_coefDeltaTime(0.0f),
	_timeSinceCreation(0.f),
	_id(0)
{
	if (G_isServer)
	{
		_id = G_id;
		G_id++;
	}
	_radius = 40;
	_owner = NULL;
	_slow = 0;
	_slowStartTime = 0.f;
	_slowDuration = 0.f;
	_selected = false;
}

AObject::~AObject()
{}

// Used to reinit variables (MapEditor)
void	AObject::damage(float posX, float posY, float endX, float endY, float width, float height, float team)
{
	_pos.first = posX;
	_pos.second = posY;
	_end.first = endX;
	_end.second = endY;
	_width = width;
	_height = height;
	_team = team;

	if (_type == WALL)
	{
		Wall *wall = dynamic_cast<Wall *>(this);
		wall->recompute();
	}
}

bool	AObject::update()
{
	return (true);
}

void	AObject::updateAObject(const sf::Time &deltaTime)
{
	_timeSinceCreation += deltaTime.asMicroseconds();
	_coefDeltaTime = deltaTime.asMicroseconds() / 20000.0f;
	_deltaTime = deltaTime;

	if (_timeSinceCreation - _slowStartTime > _slowDuration * 1000000)
		_slow = 0;
}

void  AObject::setPosition(float X, float Y)
{
	_pos.first = X;
	_pos.second = Y;
}

void  AObject::setPrevFramePosition(float X, float Y)
{
	_prevFramePos.first = X;
	_prevFramePos.second = Y;
}

void  AObject::setDirX(float dirX)
{
	_dir.first = dirX;
}

void  AObject::setDirY(float dirY)
{
	_dir.second = dirY;
}

void	AObject::setOwner(std::shared_ptr<Player> p)
{
	_owner = p;
}

void	AObject::applySlow(t_weapon *weapon)
{
	_slow = weapon->slow;
	_slowDuration = weapon->slow_duration;
	if (_slowDuration == 0.f)
		_slowDuration = 0.1;
	_slowStartTime = _timeSinceCreation;
}

///////////////////////////////////////////////
/////   Getterz

int AObject::getRadius() const
{
	return (_radius);
}

const std::pair<float, float>	&AObject::getPosition() const
{
	return (_pos);
}

const std::pair<float, float>	&AObject::getPrevFramePosition() const
{
	return (_prevFramePos);
}

const float	&AObject::getX() const
{
	return (_pos.first);
}

const float	&AObject::getY() const
{
	return (_pos.second);
}

const std::pair<float, float>	&AObject::getDirection(void) const
{
	return (_dir);
}

const float	&AObject::getDirX() const
{
	return (_dir.first);
}

const float	&AObject::getDirY() const
{
	return (_dir.second);
}

const std::pair<float, float>& AObject::getEnd() const
{
	return _end;
}

const float&		AObject::getEndX() const
{
	return _end.first;
}

const float&		AObject::getEndY() const
{
	return _end.second;
}
const eObjectType &AObject::getType() const
{
	return (_type);
}

const float&	AObject::getTimeSinceCreation() const
{
	return _timeSinceCreation;
}

std::shared_ptr<Player>	AObject::getOwner(void) const
{
	return _owner;
}

int	AObject::getSlow() const
{
	return _slow;
}

sf::Int32	AObject::getWidth() const
{
	return (_width);
}

sf::Int32	AObject::getHeight() const
{
	return (_height);
}

///////////////////////////////////////////////
/////   Team 

sf::Uint16	AObject::getTeam() const
{
	return (_team);
}

void		AObject::setTeam(sf::Uint16 team)
{
	_team = team;
}

///////////////////////////////////////////////
/////   Add force

void	AObject::addForce(float dirX, float dirY)
{
	_dir.first += dirX;
	_dir.second += dirY;
}

// Never called -_-'
void	AObject::invertForce()
{
	if (_type == PLAYER)
	{
		Player	*player = dynamic_cast<Player *>(this);

		if (player->isControled())
			ADD_EVENT(ev_WALL_COLLISION, s_event(shared_from_this()));
	}
	_dir.first = -_dir.first;
	_dir.second = -_dir.second;
}

void	AObject::divideForceBy(float nb)
{
	if (nb != 0)
	{
		_dir.first /= nb;
		_dir.second /= nb;
	}
}

///////////////////////////////////////////////
/////   ID

const sf::Uint32&	AObject::getId() const
{
	return (_id);
}

void		AObject::setId(sf::Uint32 id)
{
	_id = id;
}

//////////////////////////////////////////////////////////////////////
/////	MANDATORY to call this function to push AObject in MAP
//////////////////////////////////////////////////////////////////////

void	AObject::pushInMap(void)
{
	ADD_EVENT(ev_START, s_event(shared_from_this()));
}

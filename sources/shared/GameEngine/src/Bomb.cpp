//
// Bomb.cpp for Void Clash in /home/gegon/PFA/client/inc
// 
// Made by brunel guillaume
// Login   <brunel_g@epitech.net>
// 
// Started on  Tue Aug 13 19:16:10 2013 brunel guillaume
// Last update Sat Feb 15 06:41:49 2014 Gegon
//

#include	<iostream>
#include	"Bomb.hpp"
#include	"Wall.hpp"
#include	"Player.hpp"
#include	"Explosion.hpp"
#include	"Defines.h"
#include	"ConfigParser.hpp"
#include  "Event.hpp"

extern bool	G_isOffline;
extern bool	G_isServer;
extern t_config *G_conf;

Bomb::Bomb() :
AWeapon(BOMB, 0, 0, 0, 0)
{
  _owner = NULL;
  _launched = false;
  _clock.restart();
  _dir.first = 0;
  _dir.second = 0;
  _step = sf::seconds(0);
  _lastStep = false;
  _active = true;
}

Bomb::Bomb(float X, float Y,
	   float dirX, float dirY,
	   std::shared_ptr<Player>owner) :
  AWeapon(BOMB, X, Y, dirX, dirY)
{
  _owner = owner;
  _launched = false;
  _clock.restart();
  _dir.first = 0;
  _dir.second = 0;
  _step = sf::seconds(0);
  _lastStep = false;
  _active = true;
}

Bomb::~Bomb()
{
}

///////////////////////////////////////////////
/////   Called each frame

bool	Bomb::update()
{
	if (!AWeapon::update())
		return false;

  _step += _clock.getElapsedTime() - _precTime;
  _precTime = _clock.getElapsedTime();

  _time = _clock.getElapsedTime().asSeconds();
  if (_time > _property->duration)
    {
      explode();
      return (false);
    }
  else if (!_property->bounce && checkCollisionWithWalls())
  {
	  explode();
	  return (false);
  }
  else if (_owner && _launched == false)
  {
    _pos.first = _owner->getX();
    _dir.first = _owner->getDirX();
    _pos.second = _owner->getY();
    _dir.second = _owner->getDirY();
  }
  return (true);
}

void	Bomb::explode()
{
  //if (_owner && _owner->getBombOnHold() == this)
  //  {
  //    _owner->addForce(_owner->getDirX() * _property->acceleration,
		//  _owner->getDirY() * _property->acceleration);
  //    _owner->deleteBombOnHold();
	 // ADD_EVENT(ev_PLAYER_BOOST_BOMB, s_event(this->_owner));
  //  }
  //else
    {
      _active = false;
      std::shared_ptr<Explosion> toPush = std::make_shared<Explosion>(getX(), getY(), 0, 0, _owner, this->getType());
	  toPush->init(_property);
      toPush->onStart();
      toPush->pushInMap();
	  if (_owner && _owner->getBombOnHold().get() == this)
		 _owner->deleteBombOnHold();
    }
}

///////////////////////////////////////////////
/////   Launch

void	Bomb::launch()
{
  if (!_owner)
    return ;
  std::pair<float, float>	aim = _owner->getAimNormalized();

  _dir.first = aim.first * _property->speed;
  _dir.second = aim.second * _property->speed;

  _pos.first = _owner->getX() + _owner->getRadius() * aim.first;
  _pos.second = _owner->getY() + _owner->getRadius() * aim.second;

  _owner->addPlayerVelocity(&_dir.first, &_dir.second);

  _launched = true;
  _owner->deleteBombOnHold();
}

bool	Bomb::isLaunched() const
{
  return (_launched);
}

const sf::Clock& Bomb::getClock(void) const
{
  return _clock;
}

bool	Bomb::isActive() const
{
  return (_active);
}

// Graphics tools
bool Bomb::isNewStep(void)
{
  if (_step.asSeconds() > 1.0f)
  {
    _step = sf::seconds(0.0f);
    return true;
  }
  return false;
}

// Graphics tools
bool Bomb::isLastStep(void)
{
	if (_lastStep == false && _clock.getElapsedTime().asSeconds() / (float)_property->duration > 0.93f)
  {
    _lastStep = true;
    return true;
  }
  return false;
}

float Bomb::getRemainingTime(void)
{
	return _clock.getElapsedTime().asSeconds() - _property->duration;
}
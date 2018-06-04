//
// Bullet.hpp for Void Clash in /home/gegon/Void-Clash/client/src
//
// Made by brunel guillaume
// Login   <brunel_g@epitech.net>
//
// Started on  Thu Aug  8 21:24:20 2013 brunel guillaume
// Last update Sun Jan 19 23:59:00 2014 Gegon
//

#ifndef		BULLET_HPP_
# define	BULLET_HPP_

#include	"AWeapon.hpp"
#include	"ConfigParser.hpp"

class	GameEngine;
class	Player;

class	Bullet : public AWeapon
{
public:
  Bullet();
  Bullet(float, float, float, float, std::shared_ptr<Player>, eObjectType makerType = NONE);
  virtual ~Bullet();

  virtual bool	update();

  bool	checkHitPlayers();
  bool	checkHitTurrets();
  bool	checkHitBots();

  bool	isInsideHitbox(int X, int Y, float radius);
  eObjectType	getMakerType();

  friend sf::Packet& operator >>(sf::Packet& packet, Bullet &m);

private:
  float		_time;
  sf::Clock	_clock;
  eObjectType	_makerType;
};

sf::Packet& operator >>(sf::Packet& packet, Bullet &m);

#endif

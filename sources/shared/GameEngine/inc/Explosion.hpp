//
// Explosion.hpp for Void Clash in /home/gegon/PFA/client/GameEngine/inc
// 
// Made by brunel guillaume
// Login   <brunel_g@epitech.net>
// 
// Started on  Mon Sep  2 00:56:12 2013 brunel guillaume
// Last update Thu Jan  9 20:46:55 2014 Gegon
//

#ifndef		EXPLOSION_HPP_
# define	EXPLOSION_HPP_

#include	<utility>
#include	<SFML/System.hpp>
#include	"AWeapon.hpp"
#include	"Defines.h"

class	GameEngine;
class	Player;

class	Explosion : public AWeapon
{
public:
  Explosion();
  Explosion(float, float, float, float, std::shared_ptr<Player>, eObjectType makerType = NONE);
  virtual ~Explosion();

  void		onStart();
  virtual bool	update();

  bool	checkWallMet(std::shared_ptr<AObject>obj) const;
  bool	wallMet(float X, float Y) const;

  bool	checkSegmentSegment(std::shared_ptr<AObject>nmy);
  bool	CollisionSegSeg(Point A, Point B, Point O, Point P);
  bool	CollisionDroiteSeg(Point A, Point B, Point O, Point P);

  friend sf::Packet& operator >>(sf::Packet& packet, Explosion &m);

  eObjectType	getMakerType();

private:

  float		_time;
  sf::Clock	_clock;
  eObjectType	_makerType;
};

sf::Packet& operator >>(sf::Packet& packet, Explosion &m);

#endif

//
// Respawn.hpp for Void Clash in /home/gegon/pfa/client/GameEngine/inc
// 
// Made by Gegon
// Login   <gegon@epitech.net>
// 
// Started on  Thu Oct  3 17:41:54 2013 Gegon
// Last update Fri Dec 13 22:06:15 2013 Gegon
//

#ifndef		RESPAWN_HPP_
# define	RESPAWN_HPP_

#include	<utility>
#include	<SFML/Graphics.hpp>
#include	<SFML/System.hpp>
#include	"AObject.hpp"
#include	"Wall.hpp"


class	GameEngine;
class	Player;

struct Circle
{
  float x,y,rayon;
};

///////////////////////////////////////////////
/////   Rectangle with no friction and
/////	remove max speed for players

class	Respawn : public AObject
{
public:
  Respawn();
  Respawn(float, float, float, float, int width, int height, sf::Uint16 team);
  virtual ~Respawn();

  virtual bool	update();

  bool	isInsideRespawn(float X, float Y);
  // bool	respawnCollideWithWall(float X, float Y, int radius, Wall *wall) const;

  bool	respawnObject(AObject *obj);
  bool	checkCollisionWithWall(AObject *obj);
  bool  CollisionSegment(sf::Vector2f A,sf::Vector2f B,Circle C);
  bool	CollisionPointCercle(sf::Vector2f A, Circle C);
  bool  CollisionDroite(sf::Vector2f A,sf::Vector2f B,Circle C);

  friend sf::Packet& operator >>(sf::Packet& packet, Respawn &m);

private:
};

sf::Packet& operator >>(sf::Packet& packet, Wall &m);

#endif

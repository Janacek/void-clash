//
// GravityField.hpp for Void Clash in /home/gegon/PFA/client/GameEngine/inc
// 
// Made by brunel guillaume
// Login   <brunel_g@epitech.net>
// 
// Started on  Mon Aug 26 21:50:03 2013 brunel guillaume
// Last update Tue Dec 10 21:59:01 2013 Gegon
//

#ifndef		GRAVITY_FIELD_HPP_
# define	GRAVITY_FIELD_HPP_

#include	<utility>
#include	<SFML/System.hpp>
#include	"AWeapon.hpp"

class	GameEngine;
class	Player;

///////////////////////////////////////////////
/////   You can choice the size and the power
/////	of the Gravity Field

////	if power > 0 -> attractive
////	if power < 0 -> repulsive

#define	GRAVITY_FRAME_ACTIVITY	200	// Check hit each GRAVITY_FRAME_ACTIVITY msec
#define	GRAVITY_PUSHBACK_MITIGATION	20.f // Lower to increase pushback power

class	GravityField : public AWeapon
{
public:
  GravityField();
  GravityField(float, float, float, float, const std::shared_ptr<Player> &);
  virtual ~GravityField();

  virtual bool	update();

  // Each frame
  void	checkUpdateEffects();
  void	applyUpdateEffects(float distance, AObject *obj);
  // Each GRAVITY_FRAME_ACTIVITY ms
  void	checkFrameActivityEffects();
  void	applyFrameActivityEffects(float distance, AObject *obj);

  friend sf::Packet& operator >>(sf::Packet& packet, GravityField &m);

private:
	float		_time;
	sf::Clock	_clock;
	sf::Time	_lastActivityCheck;
};

sf::Packet& operator >>(sf::Packet& packet, GravityField &m);

#endif

//
// Bomb.hpp for Void Clash in /home/gegon/PFA/client/inc
// 
// Made by brunel guillaume
// Login   <brunel_g@epitech.net>
// 
// Started on  Tue Aug 13 19:12:19 2013 brunel guillaume
// Last update Mon Feb  3 21:41:22 2014 Gegon
//

#pragma once

#include	<utility>
#include	<SFML/System.hpp>
#include	"AWeapon.hpp"

class	GameEngine;
class	Player;

class	Bomb : public AWeapon
{
public:
  Bomb();
  Bomb(float, float, float, float, std::shared_ptr<Player>);
  virtual ~Bomb();

  virtual bool	update();

  void	launch();
  bool	isLaunched() const;
  void	onStart();
  void	explode();

  const sf::Clock& getClock(void) const;
  bool	isActive() const;
  bool isNewStep(void); // Graphic utils
  bool isLastStep(void); // Graphic utils
  float getRemainingTime(void);

  friend sf::Packet& operator >>(sf::Packet& packet, Bomb &m);

private:
  float		_time;
  sf::Clock	_clock;
  int		_damage;
  bool		_launched;
  bool		_active;

  bool _lastStep;
  sf::Time _step; // For graphical effect
  sf::Time _precTime;
};

sf::Packet& operator >>(sf::Packet& packet, Bomb &m);


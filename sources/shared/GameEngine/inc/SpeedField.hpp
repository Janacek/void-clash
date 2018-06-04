//
// SpeedField.hpp for Void Clash in /home/gegon/PFA/client/GameEngine/src
// 
// Made by brunel guillaume
// Login   <brunel_g@epitech.net>
// 
// Started on  Sun Sep  1 22:28:59 2013 brunel guillaume
// Last update Tue Dec 10 21:46:36 2013 Gegon
//

#ifndef		SPEED_FIELD_HPP_
# define	SPEED_FIELD_HPP_

#include	<utility>
#include	<SFML/System.hpp>
#include	"AObject.hpp"

class	GameEngine;
class	Player;

///////////////////////////////////////////////
/////   Rectangle with no friction and
/////	remove max speed for players

class	SpeedField : public AObject
{
public:
  SpeedField();
  SpeedField(float, float, float, float, float width, float height);
  virtual ~SpeedField();

  virtual bool	update();

  void	onStart();

  friend sf::Packet& operator >>(sf::Packet& packet, SpeedField &m);

private:
};

sf::Packet& operator >>(sf::Packet& packet, SpeedField &m);

#endif

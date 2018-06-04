//
// Wall.hpp for Void Clash in /home/gegon/PFA/client/inc
// 
// Made by brunel guillaume
// Login   <brunel_g@epitech.net>
// 
// Started on  Mon Aug 12 11:25:09 2013 brunel guillaume
// Last update Tue Dec 10 21:51:38 2013 Gegon
//

#ifndef		WALL_HPP_
# define	WALL_HPP_

#include	<utility>
#include	<SFML/System.hpp>
#include	"AObject.hpp"

class	GameEngine;
class	Player;

typedef enum	eOrientation
  {
    NO_DIR = 0,
    HORIZONTAL,
    VERTICAL
  }		eOrientation;

class	Wall : public AObject
{
public:
  Wall();
  Wall(float, float, int, enum eOrientation);
  Wall(float X, float Y, float endX, float endY);
  virtual			~Wall();
  void				recompute();

  void				calculateAngle();

  void				setOrientation(enum eOrientation orientation);

  enum eOrientation	getOrientation() const;
  int				getLen() const;
  const float&		getAngle() const;

  friend sf::Packet& operator >>(sf::Packet& packet, Wall &m);

private:
	int					_len;
	eOrientation		_orientation;
	float				_angle;
};

sf::Packet& operator >>(sf::Packet& packet, Wall &m);

#endif

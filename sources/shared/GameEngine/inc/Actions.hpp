//
// Actions.hpp for Void Clash in /home/gegon/pfa/common/GameEngine/inc
// 
// Made by Gegon
// Login   <gegon@epitech.net>
// 
// Started on  Fri Nov 15 05:30:18 2013 Gegon
// Last update Fri Nov 15 05:30:55 2013 Gegon
//

#ifndef		__ACTIONS_H__
# define	__ACTIONS_H__

struct		s_actions
{
  s_actions(void) {}
  s_actions(const s_actions &cpy) : moveX(cpy.moveX), moveY(cpy.moveY),
				    aimX(cpy.aimX), aimY(cpy.aimY),
					primary(cpy.primary), secondary(cpy.secondary) {}
  sf::Int16	moveX;
  sf::Int16	moveY;
  float		aimX;
  float		aimY;

  bool		primary;
  bool		secondary;

  //bool		switchPrimary;
  //bool		switchSecondary;
  // NEED TIMESTAMP ?
};


#endif		/* !__ACTIONS_H__ */

//
// main.hpp for main.hpp in /home/gegon/Semestre4/bomberman/GameEngine/src
// 
// Made by brunel guillaume
// Login   <brunel_g@epitech.net>
// 
// Started on  Thu May 23 17:38:10 2013 brunel guillaume
// Last update Thu Nov 14 02:50:49 2013 vial_j
//

#ifndef		MAIN_HPP_
# define	MAIN_HPP_

enum	debug
  {
    NOTHING,
    EXECUTION,
    THREAD,
    GRAPHICAL
  };

# define	DEBUG_MODE	NOTHING

# define	RESET		"\x1b[0m"
# define	RED		"\x1b[31m"
# define	GREEN		"\x1b[32m"
# define	YELLOW		"\x1b[33m"
# define	DARK_BLUE	"\x1b[34m"
# define	VIOLET		"\x1b[35m"
# define	LIGHT_BLUE	"\x1b[36m"

int		init_game();

#endif		/* MAIN_HPP_ */

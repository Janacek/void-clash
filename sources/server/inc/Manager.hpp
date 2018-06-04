//
// Manager.hpp for Void Clash in /home/vial_j/travail/voidclash
// 
// Made by vial_j
// Login   <vial_j@epitech.net>
// 
// Started on  Sat Nov 16 01:27:26 2013 vial_j
// Last update Thu Jan  2 20:43:02 2014 sadin_n
//

#ifndef		MANAGER_HPP_
# define	MANAGER_HPP_

#include	"GameEngine.hpp"
#include	"AObject.hpp"
#include	"NetworkEngine.hpp"
#include	"PhysicEngine.hpp"
#include	"ConfigParser.hpp"
#include	"Event.hpp"
#include	"MapDatabase.hpp"
#include	"AEngine.hpp"

///////////////////////////////////////////////
/////   This class is used to initialize engines
/////	and stock ptr to each engine

/////	Also used to main loop

class		Manager
{
public:
  Manager(void);
  ~Manager(void);

  void start(void);
  void stop(void);
  void run(void);

  //------------------------------------------------------------------//

  // Game content management
  void addNewObjects(void);
  void deleteObjects(void);

private:
  // Engines
  GameEngine	*_gameEngine;
  NetworkEngine	*_networkEngine;
  PhysicEngine	*_physicEngine;
};

bool	sameAObject(std::pair<eventType, s_event> one, std::pair<eventType, s_event> two);
bool    superiorAddr(std::pair<eventType, s_event> one, std::pair<eventType, s_event> two);
#endif		/* MANAGER_HPP_ */

//
// Manager.hpp for Void Clash in /home/vial_j/travail/voidclash
//
// Made by vial_j
// Login   <vial_j@epitech.net>
//
// Started on  Sat Nov 16 01:27:26 2013 vial_j
// Last update Tue Feb 11 01:27:12 2014 sadin_n
//

#ifndef		MANAGER_HPP_
# define	MANAGER_HPP_

#include	"Wall.hpp"
#include	"MapParser.hpp"
#include	"GameEngine.hpp"
#include	"AObject.hpp"
#include	"GraphicEngine.hpp"
#include	"SoundEngine_.hpp"
#include	"InputEngine_.hpp"
#include	"PhysicEngine.hpp"
#include	"NetworkEngine.hpp"
#include	"ConfigParser.hpp"
#include	"Event.hpp"
#include	"Map.hpp"
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

  // Map/config loader
  void		loadConfig(const char *);
  void		loadMap(const char *);

  std::list<AObject*>* getElems(void);

  //------------------------------------------------------------------//


private:
  // Engines
  GameEngine	*_gameEngine;
  SoundEngine_	*_soundEngine;
  InputEngine_	*_inputEngine;
  PhysicEngine	*_physicEngine;
  NetworkEngine	*_networkEngine;
  VoidClashGraphics::GraphicEngine	*_graphicEngine;

  eGameState	_gameState;
};

#endif		/* MANAGER_HPP_ */

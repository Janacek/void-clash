//////////////////////////////////////////////////////
// AEngine.hpp for project Void Clash
// 
// Made by Vial Joris
// 
// Started on  Tue Nov 12 20:27:02 2013 Vial Joris
// Updated on  Tue Nov 12 20:27:32 2013 Vial Joris
//////////////////////////////////////////////////////

#ifndef		__IENGINE_H__
# define	__IENGINE_H__

#include	<SFML/System/Time.hpp>

enum		eGameState
{
	RUN = 0,
	CHAT = 1,
	MENU = 2,
	EXIT = 3
};

class		AEngine
{
public:
  AEngine() {}
  virtual ~AEngine(void) {}

protected:
  virtual void start(void) = 0;
  virtual void stop(void) = 0;
  virtual eGameState update(const sf::Time &deltaTime) = 0;
};

#endif		/* !__IENGINE_H__ */


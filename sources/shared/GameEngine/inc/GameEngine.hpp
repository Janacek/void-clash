//
// GameEngine.hpp for Void Clash in /home/gegon/Void-Clash/client/src
// 
// Made by brunel guillaume
// Login   <brunel_g@epitech.net>
// 
// Started on  Wed Jul 31 17:04:22 2013 brunel guillaume
// Last update Fri Nov 29 13:54:15 2013 Gegon
//

#pragma once

#include	"AEngine.hpp"
#include	"ConfigParser.hpp"
#include	"MapDatabase.hpp"

class		Manager;

class		GameEngine : public AEngine
{
public:
  GameEngine(void);
  virtual ~GameEngine(void);

  // IEngine
  virtual void start(void);
  virtual void stop(void);
  virtual eGameState update(const sf::Time &deltaTime);

private:
	std::string _G_configPath;
};


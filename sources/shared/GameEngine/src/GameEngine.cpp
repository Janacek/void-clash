//
// GameEngine.cpp for Void Clash in /home/gegon/Void-Clash/client/src
// 
// Made by brunel guillaume
// Login   <brunel_g@epitech.net>
// 
// Started on  Wed Jul 31 17:05:44 2013 brunel guillaume
// Last update Wed Jan 29 23:01:08 2014 Gegon
//

#include	<iostream>
#include	"Files.hpp"
#include	"GameEngine.hpp"
#include	"MapDatabase.hpp"
#include	"Map.hpp"
#include	"AssetPath.h"
#include	"Log.hpp"

// Globals
extern t_config *G_conf;
extern bool		G_isOffline;
extern bool		G_isServer;
extern std::string G_configPath;

GameEngine::GameEngine()
{
}

GameEngine::~GameEngine()
{
}

//////////////////////////////////////////////////////////////////////
/////	Called only once at start
//////////////////////////////////////////////////////////////////////

void	GameEngine::start(void)
{
	if (!(G_isServer || G_isOffline)) // Do not load map when online client as it will be received from server
		return;

	//// Load config
	S_Map->loadConfig(G_configPath + "config.json");

	VC_INFO_CRITICAL("Map: " + G_conf->game->map);
	VC_INFO_CRITICAL("Mode : " + G_conf->game->mode);
	S_Map->init();
	S_Map->getMode()->initFromConfig();

	// Load map
	std::string fullPath = G_configPath + "maps";
	std::string mapName = G_conf->game->map;
	std::deque<std::string> mapList = S_Map->getMapDatabase()->getMapList();

	if (!S_Map->getMapDatabase()->getMapList().front().size())
		VC_ERROR_CRITICAL("Unable to find a map in map Folder");

	if (std::find(mapList.begin(), mapList.end(), G_conf->game->map) == mapList.end())
	{
		VC_WARNING_CRITICAL("Unable to find map " + G_conf->game->map + ". Loading default map.");
		mapName = S_Map->getMapDatabase()->getMapList().front(); // If map not found, get the first map
	}

	const char *file = Files::getPath(fullPath.c_str(), mapName.c_str());
	S_Map->loadMap(file);

	if (G_isOffline)
	{
		std::shared_ptr<Player> player = std::make_shared<Player>(1500, 1500, 0, 0, true);
		S_Map->addPlayer(player, 1, false);
	}
}

//////////////////////////////////////////////////////////////////////
/////	Called each frame
//////////////////////////////////////////////////////////////////////

eGameState	GameEngine::update(const sf::Time &deltaTime)
{
	 //std::cout << "Number of elements: " << S_Map->getElems()->size() << std::endl;
	 //std::cout << "Number of players: " << S_Map->getPlayers()->size() << std::endl;
	 //std::cout << "----------------------------" << std::endl;

	// WORLD OBJECT UPDATE
	{
		auto	it = S_Map->getElems()->begin();
		auto	end = S_Map->getElems()->end();

		while (it != end)
		{
			(*it)->updateAObject(deltaTime);
			if ((*it)->getType() != PLAYER)
			{
				// Update return false if element needs to be removed
				if ((*it)->update() == false)
					ADD_EVENT(ev_DELETE, s_event((*it)));
			}
			++it;
		}
	}

	// PLAYERS UPDATE
  {
	  auto	it = S_Map->getPlayers()->begin();
	  auto	end = S_Map->getPlayers()->end();

	  while (it != end)
	  {
		  // (*it)->updateAObject(deltaTime);
		  // Update return false if element needs to be removed
		  if ((*it)->update() == false)
			  ADD_EVENT(ev_DELETE, s_event((*it)));
		  ++it;
	  }
  }

  /////// Check events ///////

  // Weapon selection changed
  auto ev = Event::getEventByType(ev_WEAPON_SELECTION);
  if (ev != NULL)
  {
	  std::pair<t_weapon*, t_weapon*> *newWeapons =
		  static_cast<std::pair<t_weapon*, t_weapon*>*>(ev->begin()->second.data);

	  // Update main player weapons
	  if (newWeapons != NULL)
	  {
		  Player *mainPlayer = S_Map->getCurrentPlayer().get();
		  if (mainPlayer == NULL)
			  return RUN;

		  mainPlayer->setWeapons(
			  newWeapons->first, NULL,
			  newWeapons->second, NULL);
	  }
  }

  return RUN;
}

void	GameEngine::stop(void)
{
}

//
// Map.cpp for Void Clash in /home/gegon/pfa/sources/shared/Map/src
// 
// Made by Gegon
// Login   <gegon@epitech.net>
// 
// Started on  Thu Nov 28 22:10:55 2013 Gegon
// Last update Sat Feb 15 06:49:45 2014 Gegon
//


#include	<cmath>
#include	<iostream>
#include	"MapParser.hpp"
#include	"Player.hpp"
#include	"Wall.hpp"
#include	"GravityField.hpp"
#include	"SpeedField.hpp"
#include	"Respawn.hpp"
#include	"Flag.hpp"
#include	"Capture.hpp"
#include	"Map.hpp"
#include	"Bomb.hpp"
#include	"Files.hpp"
#include	"AssetPath.h"
#include	"Defines.h"
#include	"HudRessources.hpp"
#include	"Log.hpp"

extern bool G_isOffline;
extern bool G_isServer;
extern t_config *G_conf;
extern int G_id;
extern std::string G_configPath;

//////////////////////////////////////////////////////////////////////
/////	Ctor/Dtor
//////////////////////////////////////////////////////////////////////

using namespace Map;

MapUtils *MapUtils::_instance = NULL;

MapUtils::MapUtils(void)
{
	_elems = new std::list < std::shared_ptr<AObject> > ;
	_elems->clear();
	_players = new std::list < std::shared_ptr<Player> > ;
	_players->clear();
	_walls = new std::list < std::shared_ptr<Wall> > ;
	_walls->clear();
	_turrets = new std::list < std::shared_ptr<Turret> > ;
	_turrets->clear();
	_bots = new std::list < std::shared_ptr<Bot> > ;
	_bots->clear();
	_currentPlayer = NULL;
	_currentPlayerId = 0;
	_score = std::make_pair<int, int>(0, 0);
	_clock.restart();
	_globalClock.restart();
	_playerFollowed = NULL;
	_score.first = 0;
	_score.second = 0;
	_MapDatabase = new MapDatabase("maps");
	_mapMode = new MapMode();

	// Current zoom
	_currentZoom = -1;
}

MapUtils::~MapUtils(void)
{
	delete _elems;
	delete _players;
	delete _walls;
	delete _turrets;
	delete _bots;
	delete _MapDatabase;
	delete _mapMode;
}

MapUtils* MapUtils::getInstance(void)
{
	if (MapUtils::_instance == NULL)
		MapUtils::_instance = new MapUtils;
	return _instance;
}

void	MapUtils::init()
{
	// Timelimit
	_active = true;
	_displayScore = false;
	_displayScoreTime = sf::seconds(END_MAP_DURATION);
	_warmupTime = sf::seconds(G_conf->game->warmup_duration);
	_endOfMapTime = sf::seconds(G_conf->game->map_duration);
	//if (G_isOffline) // No switch map offline
	//	_endOfMapTime = sf::seconds(25);
}

///////////////////////////////////////////////
/////   Management funcs

void	MapUtils::initialState(t_map *map)
{
	// Don't add local map if we are online
	if (!G_isServer && !G_isOffline)
		return;

	std::shared_ptr<AObject> objectToAdd = NULL;

	if (!map)
	{
		std::cerr << "Could not load map : using default value." << std::endl;
		int width = 1000;
		int height = 1000;

		// Border walls (always)
		objectToAdd = std::make_shared<Wall>(0, 0, width, HORIZONTAL);
		objectToAdd->pushInMap();
		objectToAdd = std::make_shared<Wall>(0, 0, height, VERTICAL);
		objectToAdd->pushInMap();
		objectToAdd = std::make_shared<Wall>(width, 0, height, VERTICAL);
		objectToAdd->pushInMap();
		objectToAdd = std::make_shared<Wall>(0, height, width, HORIZONTAL);
		objectToAdd->pushInMap();

		return;
	}

	// Speed Fields
	for (int i = 0; i < map->nspeed; ++i)
		if (!map->speed[i].error)
		{
		objectToAdd = std::make_shared<SpeedField>(map->speed[i].X, map->speed[i].Y,
			map->speed[i].dirX, map->speed[i].dirY,
			map->speed[i].width, map->speed[i].height);
		objectToAdd->pushInMap();
		}

	// Walls
	for (int i = 0; i < map->nwall; ++i)
		if (!map->walls[i].error)
		{
		objectToAdd = std::make_shared<Wall>(map->walls[i].X, map->walls[i].Y, map->walls[i].len,
			map->walls[i].orientation);
		objectToAdd->pushInMap();
		}

	// Walls no dir
	for (int i = 0; i < map->nwallNoDir; ++i)
		if (!map->wallsNoDir[i].error)
		{
		objectToAdd = std::make_shared<Wall>(map->wallsNoDir[i].X, map->wallsNoDir[i].Y, map->wallsNoDir[i].endX,
			map->wallsNoDir[i].endY);
		objectToAdd->pushInMap();
		}

	// respawn
	for (int i = 0; i < map->nspawn; ++i)
		if (!map->spawn[i].error)
		{
		objectToAdd = std::make_shared<Respawn>(map->spawn[i].X, map->spawn[i].Y,
			map->spawn[i].dirX, map->spawn[i].dirY,
			map->spawn[i].width, map->spawn[i].height,
			map->spawn[i].team);
		objectToAdd->pushInMap();
		}

	// flags
	for (int i = 0; i < map->nflags; ++i)
		if (!map->flags[i].error)
		{
		objectToAdd = std::make_shared<Flag>(map->flags[i].X, map->flags[i].Y,
			map->flags[i].dirX, map->flags[i].dirY,
			map->flags[i].team);
		objectToAdd->pushInMap();
		}


	// capture
	for (int i = 0; i < map->ncapture; ++i)
		if (!map->capture[i].error)
		{
		objectToAdd = std::make_shared<Capture>(map->capture[i].X, map->capture[i].Y,
			map->capture[i].dirX, map->capture[i].dirY,
			map->capture[i].width, map->capture[i].height);
		objectToAdd->pushInMap();
		}
}


void	MapUtils::addPlayer(const std::shared_ptr<Player>& player, sf::Uint16 team, bool isAI)
{
	if (player->isControled() == true)
		_currentPlayer = player;
	else if (isAI)
		player->setAI();
	player->setTeam(team);

	// add to elems
	player->pushInMap();
}

//////////////////////////////////////////////////////////////////////
/////	Parsing functions
//////////////////////////////////////////////////////////////////////

MapDatabase	*MapUtils::getMapDatabase()
{
	return _MapDatabase;
}

void	MapUtils::loadConfig(const std::string &filename)
{
	ConfigParser	*parse = new ConfigParser;

	if (parse->loadFile(filename.c_str()))
		G_conf = (t_config *)parse->parse();
	if (G_conf == NULL)
		throw std::runtime_error("Unable to load conf file");
	delete parse;

	// Set zoom in S_Map
	S_Map->setZoom(G_conf->game->zoom);
}

//------------------------------------------------------------------//

void	MapUtils::loadMap(const char *filename)
{
	MapParser *parse = new MapParser;
	t_map *buff = NULL;
	if (parse->loadFile(filename))
		buff = (t_map *)parse->parse();
	if (buff == NULL)
		throw std::runtime_error("Unable to load map");
	S_Map->initialState(buff);
	delete parse;

	ADD_EVENT_SIMPLE(ev_MAP_LOADED);
}

//------------------------------------------------------------------//

void		MapUtils::nextMap()
{
	changeMap(_MapDatabase->next());
}

void		MapUtils::prevMap()
{
	changeMap(_MapDatabase->prev());
}

void		MapUtils::changeMap(const std::string &filename)
{
	if (G_isServer || G_isOffline)
	{
		S_Map->clearMap();
		S_Map->resetMap();
		_mapLoaded = true;
		//S_Map->deleteObjects();
		//Event::clearEvents();

		std::string fullPath = G_configPath + "maps";
		loadMap(Files::getPath(fullPath.c_str(), filename.c_str()));
		S_Map->getMapDatabase()->setCurrentMap((char *)filename.c_str());
		//S_Map->addNewObjects();
		//Event::clearEvents();
		_warmup = true;
		_clock.restart();
		_mapPath = filename;
		ADD_EVENT(ev_CHANGE_MAP, s_event(NULL, (void *)_mapPath.c_str()));
	}
}

void	MapUtils::clearMap()
{
	_mapMode->init();
	_active = true;
	_displayScore = false;
	_score.first = 0;
	_score.second = 0;
	_warmup = true;
	resetScore();
	_clock.restart();
	_timePreviousFrame = _clock.getElapsedTime();
	if (S_Map->getCurrentPlayer())
		S_Map->getCurrentPlayer()->setMapTime(sf::Time::Zero);

	// clear elems
	{
		auto	it = _elems->begin();
		auto	end = _elems->end();

		while (it != end)
		{
			if ((*it)->getType() != PLAYER)
				ADD_EVENT(ev_DELETE, s_event((*it)));
			//else
			//{
			//	if ((*it) != S_Map->getCurrentPlayer() && !G_isServer)
			//		ADD_EVENT(ev_DELETE, s_event((*it)));
			//}

			//else
			//{
			//	Player *player = dynamic_cast<Player *>(*it);
			//	if (G_isServer || !G_isOffline)
			//		player->setTeam(0);
			//	player->reset();
			//}
			++it;
		}
	}

	// clear walls
	auto	it = _walls->begin();
	auto	end = _walls->end();

	while (it != end)
	{
		ADD_EVENT(ev_DELETE, s_event((*it)));
		++it;
	}
}


//////////////////////////////////////////////////////////////////////
/////	These functions are designed to update the map content
/////	according to events start and delete
//////////////////////////////////////////////////////////////////////

void    MapUtils::deleteObjects(void)
{
	// No ev_Delete event                                                                                 
	if (Event::getEventByType(ev_DELETE) == NULL)
		return;

	// Sort and unique to remove possible duplicate of delete
	if (Event::getEventByType(ev_DELETE) != NULL)
	{
		Event::getEventByType(ev_DELETE)->sort(superiorAddr);
		Event::getEventByType(ev_DELETE)->unique(sameAObject);
	}

	bool	recurs = false;
	std::list<std::pair<eventType, s_event> >::const_iterator it =
		Event::getEventByType(ev_DELETE)->begin();
	std::list<std::pair<eventType, s_event> >::const_iterator end =
		Event::getEventByType(ev_DELETE)->end();

	while (it != end)
	{
		if (it->second.trigger)
		{
			// Elems found                                                                                    
			auto found =
				std::find(_elems->begin(), _elems->end(), it->second.trigger);
			// Walls found 
			auto foundWall =
				std::find(_walls->begin(), _walls->end(), it->second.trigger);

			if (found != _elems->end())
			{
				if (it->second.trigger->getType() == BOMB)
				{
					Bomb *b = dynamic_cast<Bomb *>(it->second.trigger.get());
					if (b->getOwner() && b->getOwner()->getBombOnHold().get() == b)
						b->getOwner()->deleteBombOnHold();
					b->setOwner(NULL);
				}

				// Need delete players from players list
				if (it->second.trigger->getType() == PLAYER)
				{
					std::shared_ptr<Player>player = std::dynamic_pointer_cast<Player>(it->second.trigger);
					if (player == S_Map->getPlayerFollowed())
						S_Map->setPlayerFollowed(NULL);
					if (player->getBombOnHold())
						player->getBombOnHold()->setOwner(NULL);
					player->deleteBombOnHold();
					auto	itWeapon = _elems->begin();
					auto	endWeapon = _elems->end();

					// Clean all weapons associated with the player
					while (itWeapon != endWeapon)
					{
						AWeapon *weapon = dynamic_cast<AWeapon *>((*itWeapon).get());
						if (weapon)
						{
							if (weapon->getOwner() == player)
							{
								weapon->setOwner(NULL);
								ADD_EVENT(ev_DELETE, s_event((*itWeapon)));
								recurs = true;
							}
						}
						++itWeapon;
					}

					auto foundP =
						std::find(_players->begin(), _players->end(), it->second.trigger);
					if (foundP != _players->end())
						_players->erase(foundP);
				}
				// Need delete turrets for turrets list
				if (it->second.trigger->getType() == TURRET)
				{
					auto foundP =
						std::find(_turrets->begin(), _turrets->end(), it->second.trigger);
					if (foundP != _turrets->end())
						_turrets->erase(foundP);
				}
				// Need delete turrets for turrets list
				if (it->second.trigger->getType() == BOT)
				{
					auto foundP =
						std::find(_bots->begin(), _bots->end(), it->second.trigger);
					if (foundP != _bots->end())
						_bots->erase(foundP);
				}
				// delete from elems
				std::shared_ptr<AObject>toto = it->second.trigger;
				_elems->erase(found);
			}


			else if (foundWall != _walls->end())
			{
				auto	wall = it->second.trigger;
				auto itWall =
					std::find(_walls->begin(), _walls->end(), wall);
				if (itWall != _walls->end())
				{
					//delete (it->second.trigger);
					_walls->erase(itWall);
				}
			}
		}
		++it;
	}
	if (recurs)
		deleteObjects();
}

void	MapUtils::addNewObjects(void)
{
	if (Event::getEventByType(ev_START) == NULL)
		return;

	std::list<std::pair<eventType, s_event> >::const_iterator it = Event::getEventByType(ev_START)->begin();
	std::list<std::pair<eventType, s_event> >::const_iterator end = Event::getEventByType(ev_START)->end();

	while (it != end)
	{
		if (!checkIfDeleteEventForObj(it->second.trigger))
		{
			if (it->second.trigger->getType() == WALL)
				_walls->push_back(std::dynamic_pointer_cast<Wall>(it->second.trigger));
			else
			{
				_elems->push_back(it->second.trigger);
				if (it->second.trigger->getType() == TURRET)
					_turrets->push_back(std::dynamic_pointer_cast<Turret>(it->second.trigger));
				if (it->second.trigger->getType() == BOT)
					_bots->push_back(std::dynamic_pointer_cast<Bot>(it->second.trigger));
				if (it->second.trigger->getType() == PLAYER)
					_players->push_back(std::dynamic_pointer_cast<Player>(it->second.trigger));
			}
		}
		++it;
	}
}

bool	MapUtils::checkIfDeleteEventForObj(const std::shared_ptr<AObject> &obj)
{
	if (Event::getEventByType(ev_DELETE) == NULL)
		return false;

	// Check if delete event for this obj
	std::list<std::pair<eventType, s_event> >::const_iterator it = Event::getEventByType(ev_DELETE)->begin();
	std::list<std::pair<eventType, s_event> >::const_iterator end = Event::getEventByType(ev_DELETE)->end();

	while (it != end)
	{
		if (it->second.trigger == obj)
			return true;
		++it;
	}
	return false;
}

//------------------------------------------------------------------//

void	MapUtils::resetScore()
{
	_score.first = 0;
	_score.second = 0;
}

void	MapUtils::setCurrentPlayerId(sf::Uint32 id)
{
	_currentPlayerId = id;
}

void	MapUtils::changeMode(enum eMapMode mode)
{
	_mapMode->changeMapMode(mode);
}

void	MapUtils::incScore(float scoreTeamOne, float scoreTeamTwo)
{
	_score.first += scoreTeamOne;
	_score.second += scoreTeamTwo;
}

void	MapUtils::setScore(int scoreTeamOne, int scoreTeamTwo)
{
	_score.first = scoreTeamOne;
	_score.second = scoreTeamTwo;
}

void	MapUtils::resetMap()
{
	_warmup = false;
	auto	it = _elems->begin();
	auto	end = _elems->end();

	while (it != end)
	{
		AWeapon *wpn = dynamic_cast<AWeapon *>((*it).get());
		if (wpn)
			ADD_EVENT(ev_DELETE, s_event((*it)));
		else
		{
			Player *player = dynamic_cast<Player *>((*it).get());

			if (player)
			{
				player->reset();
				if (G_isServer || G_isOffline)
					player->respawn();
			}
			Capture *cpt;
			cpt = dynamic_cast<Capture *>((*it).get());
			if (cpt)
				cpt->reset();
		}
		++it;
	}
	resetScore();
}

// Refresh delta time and sleep if needed
bool	MapUtils::update()
{
	_deltaTime = _globalClock.getElapsedTime() - _timePreviousFrame;
	if (G_isOffline || G_isServer) // Not client as it's send by packet sync
		_mapTime = _clock.getElapsedTime();

	// Cap FPS
	sf::Time maxSpeedFrame = sf::milliseconds(1000.0f / _fpsLimit);
	sf::sleep(maxSpeedFrame - _deltaTime);
	_deltaTime = _globalClock.getElapsedTime() - _timePreviousFrame;
	_timePreviousFrame = _globalClock.getElapsedTime();

	// End of game - displaying result
	if (_mapTime > _endOfMapTime + _warmupTime && !_displayScore)
	{
		std::shared_ptr<Player>p = getLeadingPlayer();
		ADD_EVENT(ev_END_MAP, s_event(p, NULL));
		VC_INFO_CRITICAL("End of map - Displaying results");
		_displayScore = true;
		_active = false;
	}

	// End of game - switch map (server side only, he will send packet welcome to clients)
	if (_mapTime - (_endOfMapTime + _warmupTime) > _displayScoreTime && G_isServer)
	{
		VC_INFO_CRITICAL("End of map - Switch map");
		nextMap();
		return true;
	}

	// Update map mode
	_mapMode->update();

	// Warmup
	if (G_isServer)
	{
		if (_warmup && _warmupTime > sf::seconds(1) && _mapTime + sf::seconds(1) > _warmupTime)
		{
			VC_INFO_CRITICAL("Warmup done - Starting game...");
			ADD_EVENT_SIMPLE(ev_GAME_START);
			resetMap();
		}
	}

	// Trigger when a map has been loaded on the previous frame
	if (_mapLoaded)
	{
		_mapLoaded = false;
		auto it = S_Map->getPlayers()->begin();
		auto end = S_Map->getPlayers()->end();
		while (it != end)
		{
			(*it)->respawn();
			++it;
		}
	}
	AISpawnHandler();

	return true;
}

void	MapUtils::AISpawnHandler()
{
	if (!(G_isServer || G_isOffline))
		return;

	int countOne = 0;
	int countTwo = 0;
	auto it = _players->begin();
	auto end = _players->end();
	std::shared_ptr<Player>pOne = NULL;
	std::shared_ptr<Player>pTwo = NULL;
	while (it != end)
	{
		std::shared_ptr<Player>p = std::dynamic_pointer_cast<Player>(*it);
		if (p)
		{
			if (p->getTeam() == 1)
			{
				++countOne;
				if (p->getAI())
					pOne = p;
			}
			if (p->getTeam() == 2)
			{
				++countTwo;
				if (p->getAI())
					pTwo = p;
			}
		}
		++it;
	}
	if (countOne + countTwo < G_conf->server->min_player)
	{
		if (countOne < countTwo)
			S_Map->addPlayer(std::make_shared<Player>(5000, 5000, 0, 0, false), 1, true);
		else
			S_Map->addPlayer(std::make_shared<Player>(5000, 5000, 0, 0, false), 2, true);
	}
	else if (countOne + countTwo > G_conf->server->min_player)
	{
		if (countOne < countTwo && pTwo)
			ADD_EVENT(ev_DELETE, s_event(pTwo));
		else if (countOne > countTwo && pOne)
			ADD_EVENT(ev_DELETE, s_event(pOne));
	}
	else
	{
		if (countOne >= countTwo + 2)
			ADD_EVENT(ev_DELETE, s_event(pOne));
		else if (countTwo >= countOne + 2)
			ADD_EVENT(ev_DELETE, s_event(pTwo));
	}
}

bool	MapUtils::isMapActive()
{
	return _active;
}

///////////////////////////////////////////////
//	If currentPlayer is inside a team he will be followed player
//	else (spectator mode) it will swap between player if click

bool	MapUtils::setPlayerFollowed()
{
	// If no current player, reset view to default
	if (S_Map->getCurrentPlayer() == NULL)
		return false;
	// If current player is playing
	else if (S_Map->getCurrentPlayer()->getTeam() != 0 && S_Map->getMode()->getProperty()->respawnTime != -1)
		_playerFollowed = S_Map->getCurrentPlayer();
	else if (S_Map->getCurrentPlayer()->getTeam() != 0 && S_Map->getMode()->getProperty()->respawnTime == -1 && S_Map->getCurrentPlayer()->getLife() > 0)
		_playerFollowed = S_Map->getCurrentPlayer();
	else	// Spec another player
		followingAnotherPlayer(false);

	//// Must switch player
	//if (Event::getEventByType(ev_KILL) && S_Map->getMode()->getProperty()->respawnTime == -1)
	//{
	//	std::list<std::pair<eventType, s_event> >::const_iterator it =
	//		Event::getEventByType(ev_KILL)->begin();
	//	std::list<std::pair<eventType, s_event> >::const_iterator end =
	//		Event::getEventByType(ev_KILL)->end();
	//	while (it != end)
	//	{
	//		Player *p = (Player*)(*it).second.data;
	//		if (p && p == S_Map->getCurrentPlayer())
	//			followingAnotherPlayer(true);
	//		++it;
	//	}
	//}

	if (_playerFollowed == NULL)
		return false;

	return true;
}

void	MapUtils::setPlayerFollowed(const std::shared_ptr<Player> &player)
{
	_playerFollowed = player;
}

void	MapUtils::followingAnotherPlayer(bool mustSwitch)
{
	auto it = S_Map->getPlayers()->begin();
	auto end = S_Map->getPlayers()->end();

	bool	currentFollowed = false;
	bool	wantSwitch = wantSwitchPlayerFollowed();
	if (mustSwitch)
		wantSwitch = true;

	// First pass - find next followed player when switching
	// If can't find him (example: he wants to switch and is the last player in the list) go to 2nd pass to start from begining

	while (it != end)
	{
		if ((*it).get() == S_Map->getPlayerFollowed().get() && (*it)->getTeam() != 0)
		{
			currentFollowed = true;
			if (!(wantSwitch))
				return;
		}
		else if (currentFollowed == true && wantSwitch &&
			(*it)->getTeam() != 0 && (*it)->getLife() > 0)
		{
			_playerFollowed = (*it);
			return;
		}
		++it;
	}


	// Second pass - start from begining to find
	it = S_Map->getPlayers()->begin();
	end = S_Map->getPlayers()->end();
	while (it != end)
	{
		if ((*it)->getTeam() != 0 && (*it)->getLife() > 0)
		{
			_playerFollowed = (*it);
			return;
		}
		++it;
	}

	// No solution found = set value by default to current player
	_playerFollowed = S_Map->getCurrentPlayer();
}

bool	MapUtils::wantSwitchPlayerFollowed()
{
	// Switch player followed
	if (Event::getEventByType(ev_PLAYER_ACTION) != NULL)
	{
		std::list<std::pair<eventType, s_event> >::const_iterator it =
			Event::getEventByType(ev_PLAYER_ACTION)->begin();
		s_actions *actions;
		actions = (s_actions*)(it->second.data);
		static bool pressed;
		if (actions->primary)
			pressed = true;
		else if (pressed == true)
		{
			pressed = false;
			return true;
		}
	}
	return false;
}


// Set FpsLimit (remove dependency with GraphicEngine)
void MapUtils::setFpsLimit(int limit)
{
	_fpsLimit = limit;
}

///////////////////////////////////////////////
/////   Utils

std::shared_ptr<AObject>		MapUtils::findPlayerWithID(sf::Uint32 id)
{
	if (id == 0)
		return NULL;

	auto it = _players->begin();
	auto end = _players->end();

	while (it != end)
	{
		if ((*it)->getId() == id)
			return (*it);
		++it;
	}
	return NULL;
}

std::shared_ptr<AObject>		MapUtils::findObjectWithID(sf::Uint32 id)
{
	if (id == 0)
		return NULL;

	{
		auto it = _elems->begin();
		auto end = _elems->end();

		while (it != end)
		{
			if ((*it)->getId() == id)
				return (*it);
			++it;
		}
	}

	auto it = _walls->begin();
	auto end = _walls->end();

	while (it != end)
	{
		if ((*it)->getId() == id)
			return (*it);
		++it;
	}
	return NULL;
}

std::shared_ptr<AObject>		MapUtils::findObjectAtPosition(float x, float y)
{
	{
		auto it = _elems->begin();
		auto end = _elems->end();

		while (it != end)
		{
			if ((*it)->getType() == RESPAWN)
			{
				Respawn *respawn = dynamic_cast<Respawn *>((*it).get());
				if (respawn->getTeam() != 0 &&
					x >= respawn->getX() && x <= respawn->getX() + respawn->getWidth() &&
					y >= respawn->getY() && y <= respawn->getY() + respawn->getHeight())
					return *it;
			}
			if ((*it)->getType() == SPEED_FIELD)
			{
				SpeedField *speedfield = dynamic_cast<SpeedField *>((*it).get());
				if (x >= speedfield->getX() && x <= speedfield->getX() + speedfield->getWidth() &&
					y >= speedfield->getY() && y <= speedfield->getY() + speedfield->getHeight())
					return *it;
			}
			if ((*it)->getType() == CAPTURE)
			{
				Capture *capture = dynamic_cast<Capture *>((*it).get());
				if (x >= capture->getX() && x <= capture->getX() + capture->getWidth() &&
					y >= capture->getY() && y <= capture->getY() + capture->getHeight())
					return *it;
			}
			if ((*it)->getType() == FLAG)
			{
				Flag *flag = dynamic_cast<Flag *>((*it).get());
				if (sqrt(powf(flag->getX() - x, 2) + powf(flag->getY() - y, 2)) < (*it)->getRadius() * 2)
					return *it;
			}
			++it;
		}
	}

	auto it = _walls->begin();
	auto end = _walls->end();

	Point A;
	Point B;
	Cercle C;
	while (it != end)
	{
		A.x = (*it)->getX();
		A.y = (*it)->getY();
		B.x = (*it)->getEndX();
		B.y = (*it)->getEndY();
		C.x = x;
		C.y = y;
		C.rayon = (*it)->getRadius() + 100;

		Vecteur u;
		u.x = B.x - A.x;
		u.y = B.y - A.y;
		Vecteur AC;
		AC.x = C.x - A.x;
		AC.y = C.y - A.y;
		float numerateur = u.x*AC.y - u.y*AC.x;   // norme du vecteur v
		if (numerateur < 0)
			numerateur = -numerateur;   // valeur absolue ; si c'est négatif, on prend l'opposé.
		float denominateur = sqrt(u.x*u.x + u.y*u.y);  // norme de u
		float CI = numerateur / denominateur;
		if (CI < C.rayon)
		{

			Vecteur AB, AC, BC;
			AB.x = B.x - A.x;
			AB.y = B.y - A.y;
			AC.x = C.x - A.x;
			AC.y = C.y - A.y;
			BC.x = C.x - B.x;
			BC.y = C.y - B.y;
			float pscal1 = AB.x*AC.x + AB.y*AC.y;  // produit scalaire
			float pscal2 = (-AB.x)*BC.x + (-AB.y)*BC.y;  // produit scalaire
			if (pscal1 >= 0.0f && pscal2 >= 0.0f)
				return *it;
		}
		++it;
	}
	return NULL;
}

std::list<std::shared_ptr<AObject>>	*MapUtils::getElems(void)
{
	return _elems;
}

std::list<std::shared_ptr<Player>>	*MapUtils::getPlayers(void)
{
	return _players;
}


std::list<std::shared_ptr<Bot>>		*MapUtils::getBots(void)
{
	return _bots;
}

std::list<std::shared_ptr<Turret>>	*MapUtils::getTurrets(void)
{
	return _turrets;
}

std::list<std::shared_ptr<Wall>>	*MapUtils::getWalls(void)
{
	return _walls;
}

Flag* MapUtils::getFlag(int team)
{
	auto it = _elems->begin();
	auto end = _elems->end();

	while (it != end)
	{
		if ((*it)->getType() == FLAG)
		{
			if ((*it)->getTeam() == team)
				return dynamic_cast<Flag*>((*it).get());
		}
		++it;
	}
	return NULL;
}

const std::shared_ptr<Player>	&MapUtils::getCurrentPlayer()
{
	//if (G_isServer)
	//	return NULL;
	return _currentPlayer;
}

std::shared_ptr<Player>	MapUtils::getLeadingPlayer()
{
	auto it = _players->begin();
	auto end = _players->end();

	std::shared_ptr<Player>p = NULL;
	int record = -1;
	while (it != end)
	{
		if ((*it)->getScore() == record)
			p = NULL;
		if ((*it)->getScore() > record)
		{
			record = (*it)->getScore();
			p = (*it);
		}
		++it;
	}
	return p;
}


sf::Uint32		MapUtils::getCurrentPlayerId()
{
	return _currentPlayerId;
}

MapMode		*MapUtils::getMode()
{
	return _mapMode;
}

std::pair<int, int>	MapUtils::getScore()
{
	return _score;
}

int					MapUtils::getNbOfPlayer(int team)
{
	auto	it = _players->begin();
	auto	end = _players->end();
	int	count = 0;

	while (it != end)
	{
		if ((*it)->getTeam() == team)
			count++;
		++it;
	}
	return count;
}

const std::shared_ptr<Player>	&MapUtils::getPlayerFollowed()
{
	return _playerFollowed;
}

//------------------------------------------------------------------//

const sf::Clock& MapUtils::getGlobalClock()
{
	return (_globalClock);
}

const sf::Clock& MapUtils::getClock()
{
	return (_clock);
}

void	MapUtils::setJoinServerTime(sf::Time joinTime)
{
	_mapTime = joinTime;
}


void	MapUtils::setMapDuration(sf::Time mapDuration)
{
	_endOfMapTime = mapDuration;
}

void	MapUtils::setWarmupDuration(sf::Time warmupDuration)
{
	_warmupTime = warmupDuration;
}
//------------------------------------------------------------------//

const sf::Time& MapUtils::getDeltaTime() const
{
	return (_deltaTime);
}

// Return join time server
sf::Time MapUtils::getTime()
{
	return _mapTime;
}

sf::Time MapUtils::getWarmupDuration()
{
	return _warmupTime;
}

sf::Time MapUtils::getMapDuration()
{
	return _endOfMapTime;
}

// Return remaining time on the map
sf::Time MapUtils::getWarmupTime()
{
	return _warmupTime - _mapTime;
}

// Return remaining time on the map
sf::Time MapUtils::getTimeRemaningOnMap()
{
	if (getWarmupTime() > sf::seconds(0))
		return _endOfMapTime;
	return _endOfMapTime + _warmupTime - _mapTime;
}

//////////////////////////////////////////////////////////////////////
/////	Other funcs
//////////////////////////////////////////////////////////////////////

bool    sameAObject(std::pair<eventType, s_event> one, std::pair<eventType, s_event> two)
{
	if (one.second.trigger == two.second.trigger)
		return true;
	return false;
}

bool    superiorAddr(std::pair<eventType, s_event> one, std::pair<eventType, s_event> two)
{
	if (one.second.trigger > two.second.trigger)
		return true;
	return false;
}

//////////////////////////////////////////////////////////////////////
/////	Get the number of player alive
////	(team = -1 by default)
////	don't pass parameters when no team mode
//////////////////////////////////////////////////////////////////////

int MapUtils::getAlivePlayerNumber(int team)
{
	auto it = _players->begin();
	auto end = _players->end();

	int alive = 0;
	while (it != end)
	{
		if ((*it)->getTeam() == 0 && team == 0)
		{
			++alive;
		}
		else if (((*it)->getTeam() == team && (*it)->getLife() > 0) ||
			(team == -1 && (*it)->getTeam() != 0 && (*it)->getLife() > 0))
		{
			++alive;
		}
		++it;
	}
	return alive;
}

//////////////////////////////////////////////////////////////////////
/////	Return the player number for the team passed as parameter
////	ignore bots
////	If (-1) is passed as, team it will return the total number
////	of real player on the server
//////////////////////////////////////////////////////////////////////

int MapUtils::getRealPlayerInTeam(int team)
{
	auto it = _players->begin();
	auto end = _players->end();

	int count = 0;
	while (it != end)
	{
		// Care only about real player
		if ((*it)->getAI() == NULL)
		{
			if (team == -1 || (*it)->getTeam() == team)
				++count;
		}
		++it;
	}
	return count;
}

//////////////////////////////////////////////////////////////////////
/////	Zoom
//////////////////////////////////////////////////////////////////////

void MapUtils::setZoom(float zoom)
{
	_currentZoom = zoom;
}

float MapUtils::getZoom(void)
{
	return _currentZoom;
}

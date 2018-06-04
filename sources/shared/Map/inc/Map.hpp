//
// Event.hpp for Void Clash in /home/vial_j/travail/voidclash/client
// 
// Made by vial_j
// Login   <vial_j@epitech.net>
// 
// Started on  Thu Nov 14 00:41:12 2013 vial_j
// Last update Fri Feb  7 04:43:58 2014 Gegon
//

#ifndef		__MAP_H__
# define	__MAP_H__

#include	<map>
#include	<list>
#include	<memory>
#include	"AObject.hpp"
#include	"Event.hpp"
#include	"Wall.hpp"
#include	"Turret.hpp"
#include	"Player.hpp"
#include	"Bot.hpp"
#include	"MapParser.hpp"
#include	"MapDatabase.hpp"
#include	"MapMode.hpp"

#define	SERVER_TICKRATE				128		// in tick / sec
#define END_MAP_DURATION			5		// in sec - used to display score

namespace	Map
{
	///////////////////////////////////////////////
	///////////////////////////////////////////////
	///////////////////////////////////////////////
	/////	Internal class
	class		MapUtils
	{
	public:
		MapUtils(void);
		~MapUtils(void);

		static MapUtils* getInstance(void);

		void	init();

		bool	update();

		void	initialState(t_map *map);
		void	addNewObjects(void);
		void	deleteObjects(void);
		bool	checkIfDeleteEventForObj(const std::shared_ptr<AObject> &obj);
		void	addPlayer(const std::shared_ptr<Player>& player, sf::Uint16 team, bool isAI = false);
		void	clearMap();
		void	resetScore();
		void	resetMap();
		void	setCurrentPlayerId(sf::Uint32 id);
		void	changeMode(enum eMapMode mode);
		void	incScore(float scoreTeamOne, float scoreTeamTwo);
		void	setScore(int scoreTeamOne, int scoreTeamTwo);
		const std::shared_ptr<Player>	&getPlayerFollowed();
		int	getAlivePlayerNumber(int team = -1);
		int getRealPlayerInTeam(int team);

		// Map / config loader
		MapDatabase	*getMapDatabase();
		void	loadConfig(const std::string &filename);
		void	loadMap(const char *filename);
		void	nextMap();
		void	prevMap();
		void	changeMap(const std::string &filename);

		std::list<std::shared_ptr<AObject>>		*getElems();
		std::list<std::shared_ptr<Player>>			*getPlayers();
		std::list<std::shared_ptr<Wall>>			*getWalls();
		std::list<std::shared_ptr<Bot>>			*getBots(void);
		std::list<std::shared_ptr<Turret>>			*getTurrets();
		Flag* getFlag(int team);
		std::shared_ptr<AObject>			findObjectWithID(sf::Uint32 id);
		std::shared_ptr<AObject>			findPlayerWithID(sf::Uint32 id);
		std::shared_ptr<AObject> findObjectAtPosition(float x, float y);
		const std::shared_ptr<Player>	&getCurrentPlayer();
		std::shared_ptr<Player>	getLeadingPlayer();
		sf::Uint32				getCurrentPlayerId();
		MapMode				*getMode();
		std::pair<int, int>			getScore();
		int					getNbOfPlayer(int team);

		// Clock
		const sf::Time& getDeltaTime(void) const;
		const sf::Clock& getGlobalClock(void);
		const sf::Clock& getClock(void);

		sf::Time getTime(); // return synced played time on the map
		sf::Time getMapDuration();
		sf::Time getWarmupDuration();

		sf::Time getWarmupTime();
		sf::Time getTimeRemaningOnMap(); // return time left on the map before displaying score
		bool	isMapActive(); // Define is map is playable (ie. not displaying scores)

		void	setJoinServerTime(sf::Time joinTime); // Used to sync client / serv
		void	setMapDuration(sf::Time mapDuration); // Used to sync client / serv
		void	setWarmupDuration(sf::Time warmupDuration); // Used to sync client / serv

		bool	setPlayerFollowed();
		void	setPlayerFollowed(const std::shared_ptr<Player> &player);

		// Remove dependency with GraphicEngine
		void setFpsLimit(int limit);

		// Zoom
		void setZoom(float zoom);
		float getZoom(void);

		// AI
		void	AISpawnHandler();

	private:
		// Game contents
		MapDatabase	*_MapDatabase;

		void	followingAnotherPlayer(bool mustSwitch);
		bool	wantSwitchPlayerFollowed();

		static MapUtils		*_instance;
		std::list<std::shared_ptr<AObject>>	*_elems;
		std::list<std::shared_ptr<Player>>		*_players;
		std::list<std::shared_ptr<Wall>>		*_walls;
		std::list<std::shared_ptr<Bot>>		*_bots;
		std::list<std::shared_ptr<Turret>>		*_turrets;
		std::shared_ptr<Player>	_currentPlayer;
		sf::Uint32			_currentPlayerId;
		MapMode				*_mapMode;
		std::string			_mapPath;

		// Game clock
		sf::Clock	_globalClock; // Relative to server creation time
		sf::Clock	_clock; // Relative to map
		sf::Time	_deltaTime;
		sf::Time	_timePreviousFrame;
		float		_coefDeltaTime;

		// Game (_endOfMapTime) -> Display Score (_displayScoreTime) -> next map...
		sf::Time	_warmupTime;	// Define the duration of the warmup
		sf::Time	_endOfMapTime;	// Define the duration of the map before next map
		sf::Time	_displayScoreTime; // Define the time during which we display the score
		sf::Time	_mapTime;	// Used to sync client time with serv
		bool		_displayScore; // Used to filter
		bool		_active;
		bool		_warmup;
		bool		_mapLoaded;

		// Fps Limit
		int _fpsLimit;
		// Current Zoom
		float _currentZoom;

		// Camera follow this player
		std::shared_ptr<Player>	_playerFollowed;

		std::pair<float, float>	_score;
	};
}

bool    sameAObject(std::pair<eventType, s_event> one, std::pair<eventType, s_event> two);
bool    superiorAddr(std::pair<eventType, s_event> one, std::pair<eventType, s_event> two);

#define S_Map Map::MapUtils::getInstance()

#endif		/* !__EVENTTYPE_H__ */

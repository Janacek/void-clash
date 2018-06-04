//
// ReceiverEventHandling.cpp for Void Clash in /home/gegon/pfa/sources/client/Common/src
// 
// Made by Gegon
// Login   <gegon@epitech.net>
// 
// Started on  Sun Dec 15 23:46:35 2013 Gegon
// Last update Mon Feb 24 17:10:28 2014 Gegon
//


#include	"NetworkEngine.hpp"
#include	"Receiver.hpp"
#include	"Manager.hpp"
#include	"Sender.hpp"
#include        "Bullet.hpp"
#include        "Bomb.hpp"
#include        "Flag.hpp"
#include        "Explosion.hpp"
#include        "GravityField.hpp"
#include        "SpeedField.hpp"
#include        "Respawn.hpp"
#include        "Wall.hpp"
#include	"main.hpp"
#include	"HudRessources.hpp"
#include	"Log.hpp"
#include	"Map.hpp"

extern t_config *G_conf;

///////////////////////////////////////////////
/////   Handle generic packet

void	Receiver::handleUpdate()
{
	if (!_welcomeReceived)
	{
		_packet.clear();
		return;
	}

	// Drop old packet update to work only on most recent ones
	float timestamp;
	if (_packet >> timestamp)
	{
		static float	previousTimestamp;
		float localTimestamp = S_Map->getClock().getElapsedTime().asMilliseconds();
		localTimestamp = S_Map->getClock().getElapsedTime().asMilliseconds();
		if (S_Map->getCurrentPlayer())
		{
			S_Map->getCurrentPlayer()->setLatency(localTimestamp - previousTimestamp);
			_averageLatency = (_averageLatency * NB_OF_PACKET_FOR_AVERAGE_LATENCY + (localTimestamp - previousTimestamp)) 
				/ (NB_OF_PACKET_FOR_AVERAGE_LATENCY + 1.f);
			//if (S_Map->getCurrentPlayer()->getLatency() > LAGGY_PACKET_THRESHOLD)
			//   {	
			//     _networkEngine->printLog(3, "Laggy packet received. Dropping it...");
			//     _packet.clear();
			//     previousTimestamp = localTimestamp;
			//     return ;
			//   }
			previousTimestamp = localTimestamp;
		}

		//if (_lastUpdate == 0.0f)
		//	_lastUpdate = timestamp;

		//if (timestamp < _lastUpdate)
		//{
		//	_networkEngine->printLog(2, "Older update packet received. Dropping it...");
		//	_lastUpdate = timestamp;
		//	_packet.clear();
		//	return;
		//}
		_lastUpdate = timestamp;
	}
	else
	{
		_networkEngine->printLog(1, "Unable to extract timestamp of update packet");
		return;
	}

	_lastUpdate = timestamp;
	//_networkEngine->printLog(3, "Update packet received");

	auto it = S_Map->getPlayers()->begin();
	auto end = S_Map->getPlayers()->end();
	while (it != end)
	{
		(*it)->setActive(false);
		++it;
	}

	while (!(_packet.endOfPacket()))
	{
		if (!extractWorldData())
			return;
	}

	it = S_Map->getPlayers()->begin();
	while (it != end)
	{
		if (!(*it)->isActive())
			ADD_EVENT(ev_DELETE, s_event(*it));
		++it;
	}
}

void	Receiver::handleWelcome()
{
	if (G_conf == NULL)
	{
		_packet.clear();
		return;
	}

	_welcomeReceived = true;
	ADD_EVENT(ev_WELCOME, s_event());
	ADD_EVENT(ev_CHANGE_MAP, s_event(NULL, NULL));

	_networkEngine->printLog(2, "Welcome packet received", VIOLET);
	S_Map->clearMap();

	// time update extraction
	sf::Int32	ms;
	_packet >> ms;
	sf::Time time = sf::milliseconds(ms);
	S_Map->setJoinServerTime(time);

	_packet >> ms;
	time = sf::milliseconds(ms);
	S_Map->setWarmupDuration(time);

	_packet >> ms;
	time = sf::milliseconds(ms);
	S_Map->setMapDuration(time);

	// score update
	sf::Int32	scoreOne;
	sf::Int32	scoreTwo;
	_packet >> scoreOne;
	_packet >> scoreTwo;
	S_Map->incScore(scoreOne, scoreTwo);

	// Extract world data
	while (!(_packet.endOfPacket()))
		extractWorldData();

	// Set time
	if (S_Map->getCurrentPlayer())
		S_Map->getCurrentPlayer()->setMapTime(time);
	else
	{
		VC_WARNING_CRITICAL("Unable to connect to server");
		// exit(0);
	}

	_networkEngine->getSender()->sendConfirmation(PACKET_WELCOME);
}

void	Receiver::handleSynchro()
{
	if (G_conf == NULL)
	{
		_packet.clear();
		return;
	}

	  _networkEngine->printLog(3, "Synchro packet received", VIOLET);
	  sf::Uint32 mode;
	  _packet >> mode;
	  if (mode != S_Map->getMode()->getModeEnum())
		  S_Map->changeMode(static_cast<enum eMapMode>(mode));
	  std::string mapName;
	  _packet >> mapName;
	  S_Map->getMapDatabase()->setCurrentMap((char *)mapName.c_str());
	  int scoreOne;
	  _packet >> scoreOne;
	  int scoreTwo;
	  _packet >> scoreTwo;
	  S_Map->setScore(scoreOne, scoreTwo);
	  int roundNb;
	  _packet >> roundNb;
	  S_Map->getMode()->setRoundNb(roundNb);

	  // time update extraction
	  sf::Int32	ms;
	  _packet >> ms;
	  sf::Time time = sf::milliseconds(ms);
	  S_Map->setJoinServerTime(time);

	while (!(_packet.endOfPacket()))
		extractWorldData();
}

void	Receiver::handleVersionCheck()
{
	sf::Uint32	version;
	_packet >> version;
	if (version != VOID_CLASH_VERSION)
	{
		ADD_EVENT(ev_INCORRECT_VERSION, s_event(NULL, (void *)version));
		VC_WARNING_CRITICAL("Missmatch of version. Client version: " + std::to_string(VOID_CLASH_VERSION) + ". Server version: " + std::to_string(version));
	}
	else
		VC_INFO_CRITICAL("Match of version SUCCESS. Version: " + std::to_string(VOID_CLASH_VERSION));
}
///////////////////////////////////////////////
/////   Handle events packet

void	Receiver::handleLaunchEvent()
{
	sf::Uint32	id;
	_packet >> id;
	if (id == 0)
		return;

	AObject	*obj = S_Map->findObjectWithID(id).get();
	if (obj)
	{
		Bomb *bomb = dynamic_cast<Bomb *>(obj);
		if (bomb)
			bomb->launch();
		else
			_networkEngine->printLogWithId(1, "Unable to find bomb for Event launch with id ", id);
	}
	_networkEngine->printLogWithId(2, "Event LAUNCHED packet received for item ", id, VIOLET);
}

void	Receiver::handleDeleteEvent(sf::Uint32	id)
{
	if (id == 0)
		return;

	std::shared_ptr<AObject>obj = S_Map->findObjectWithID(id);
	if (!obj)
	{
		// _networkEngine->printLogWithId(1, "Unable to find object with id ", id);
		return;
	}

	ADD_EVENT(ev_DELETE, s_event((obj)));
	if (obj->getType() == PLAYER) // Delete objs associated with the player
	{
		_networkEngine->printLogWithId(2, "Player is flagged as disconnected. Id is ", id);

		auto	it = S_Map->getElems()->begin();
		auto	end = S_Map->getElems()->end();
		while (it != end)
		{
			if ((*it)->getOwner() == obj)
				ADD_EVENT(ev_DELETE, s_event((*it)));
			++it;
		}
	}
}


void	Receiver::handlePlayerHitEvent()
{
	sf::Uint32	idHitter;
	sf::Uint32	idHitted;

	_packet >> idHitter;
	_packet >> idHitted;

	std::shared_ptr<AObject>hitter = S_Map->findObjectWithID(idHitter);
	std::shared_ptr<AObject>hitted = S_Map->findObjectWithID(idHitted);
	ADD_EVENT(ev_PLAYER_HIT, s_event(hitter, hitted.get()));
	if (hitter && hitter->getType() != GRAVITY_FIELD)
		ADD_EVENT(ev_DELETE, s_event(hitter));
}

void	Receiver::handleKillEvent()
{
	sf::Uint32	idKiller;
	sf::Uint32	idKilled;
	sf::Uint32	tmp;

	if (!(_packet >> idKiller >> idKilled >> tmp))
	{
		_networkEngine->printLog(1, "Unable to extract kill packet");
		return;
	}
	eObjectType	hitterType = static_cast<enum eObjectType>(tmp);

	std::shared_ptr<Player> killer = std::dynamic_pointer_cast<Player>(S_Map->findObjectWithID(idKiller));
	Player *killed = dynamic_cast<Player *>(S_Map->findObjectWithID(idKilled).get());
	if (killer && killed)
	{
		ADD_EVENT(ev_KILL, s_event(killer, (void *)killed));
		t_killHandler	*handler = new t_killHandler();
		handler->_killer = killer;
		handler->_killed = killed;
		handler->_hitterType = hitterType;
		handler->_isNew = true;
		HudRessources::getInstance()->getKillsReport().push_front(handler);
		killed->plusDeaths();
		killed->resetKillsStreak();
		if (killer->getId() != killed->getId())
		{
			killer->plusKills();
			killer->plusKillsStreak();
			if (S_Map->getMode()->getProperty()->killIncTeamScore)
			{
				if (killer->getTeam() == 1)
					S_Map->incScore(1, 0);
				if (killer->getTeam() == 2)
					S_Map->incScore(0, 1);
			}
			//_networkEngine->printLogWithId(2, "Event KILL packet received for killer's id ", idKiller, VIOLET);
		}
		//else
		//	_networkEngine->printLogWithId(2, "Event KILL packet (suicide !) received for player ", idKiller, VIOLET);
	}
}

void	Receiver::handleChatEvent()
{
	std::string	msg;
	sf::Uint32	id;

	_packet >> id;
	_packet >> msg;

	t_msgHandler	*handler = new t_msgHandler();
	handler->_msg = msg;
	handler->_id = id;
	handler->_isNew = true;
	HudRessources::getInstance()->getChat().push_front(handler);
	_networkEngine->printLogWithId(2, "Event CHAT packet received: \"" + msg + "\". Received from id ", id, VIOLET);

	// Send event
	ADD_EVENT(ev_CHAT_MSG_RECEIVED, s_event(NULL, handler));
}


void	Receiver::handleIdentification()
{
	sf::Uint32	id;
	_packet >> id;

	S_Map->setCurrentPlayerId(id);
	_networkEngine->printLogWithId(2, "Event IDENTIFICATION packet received. You are player ", id, VIOLET);
	_networkEngine->getSender()->sendConfirmation(PACKET_EVENT_IDENT);
}

void	Receiver::handleCaptureFlag()
{
	sf::Uint32	idFlag;
	sf::Uint32	idPlayer;

	_packet >> idFlag;
	_packet >> idPlayer;
	std::shared_ptr<Flag> flag = std::dynamic_pointer_cast<Flag>(S_Map->findObjectWithID(idFlag));
	std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(S_Map->findObjectWithID(idPlayer));

	flag->reset(true);
	_networkEngine->printLogWithId(2, "Event CAPTURE_FLAG packet received. Id flag is ", idFlag, VIOLET);
	// // inc personal score by 5
	// for (int i = 0; i < 5; i++)
	//   player->plusKills();
	// // inc team score by 1
	if (player->getTeam() == 1)
		S_Map->incScore(1, 0);
	if (player->getTeam() == 2)
		S_Map->incScore(0, 1);

	ADD_EVENT(ev_CAPTURE_FLAG, s_event(flag, player.get()));
}

void	Receiver::handleSwitchMapMode()
{
	sf::Uint32	tmp;
	_packet >> tmp;
	eMapMode mode = static_cast<enum eMapMode>(tmp);

	S_Map->changeMode(mode);
	_networkEngine->printLogWithId(2, "Event SWITCH_MAP_MODE received. Switch to map mode ", tmp);
}

void	Receiver::handleTurretDestroyed()
{
	sf::Uint32	id;

	_packet >> id;

	std::shared_ptr<AObject>obj = S_Map->findObjectWithID(id);
	if (obj)
	{
		ADD_EVENT(ev_TURRET_DESTROYED, s_event(obj));
		ADD_EVENT(ev_DELETE, s_event(obj));
		_networkEngine->printLog(3, "Event TURRET_DESTROYED received.");
	}
	_networkEngine->printLog(1, "Event TURRET_DESTROYED received but unable to find object.");
}

void	Receiver::handleWeaponSelection()
{
	while (!(_packet.endOfPacket()))
	{
		int id = 0;
		
		_packet >> id;
		std::shared_ptr<AObject>obj;
		obj = S_Map->findObjectWithID(id);
		Player *player = dynamic_cast<Player *>(obj.get());
		if (player == NULL)
			return;

		t_weapon	*primary = NULL;
		t_weapon	*primaryAlt = NULL;
		t_weapon	*secondary = NULL;
		t_weapon	*secondaryAlt = NULL;

		sf::Int16	index;
		_packet >> index;
		if (index != -1)
			primary = G_conf->weapons->at(index);

		_packet >> index;
		if (index != -1)
			primaryAlt = G_conf->weapons->at(index);

		_packet >> index;
		if (index != -1)
			secondary = G_conf->weapons->at(index);

		_packet >> index;
		if (index != -1)
			secondaryAlt = G_conf->weapons->at(index);

		player->setWeapons(primary, primaryAlt, secondary, secondaryAlt);
	}
}

void	Receiver::handleConfig()
{
	// Reset weapons
	//std::list<Player *>::iterator	it = S_Map->getPlayers()->begin();
	//std::list<Player *>::iterator	end = S_Map->getPlayers()->end();
	//while (it != end)
	//{
	//	Player *player = dynamic_cast<Player *>(*it);
	//	if (player)
	//		player->setWeapons(NULL, NULL, NULL, NULL);
	//	++it;
	//}

	// Set new cfg
	std::string	result;
	if (!(_packet >> result))
	{
		_networkEngine->printLog(1, "Unable to extract CONFIG packet");
		return;
	}
	ConfigParser	*parse = new ConfigParser;

	parse->loadString(result);
	G_conf = (t_config *)parse->parse();
	_networkEngine->printLog(2, "Event CONFIG packet received. Modifying config file...");

	S_Map->setZoom(G_conf->game->zoom);

	ADD_EVENT_SIMPLE(ev_CONFIG_RECEIVED);
	_networkEngine->getSender()->sendConfirmation(PACKET_CONFIG);
	delete parse;
}

void	Receiver::handlePing()
{
	float timestamp;
	if (!(_packet >> timestamp))
	{
		_networkEngine->printLog(1, "Unable to extract timestamp of PING packet");
		return;
	}
	_networkEngine->getSender()->sendPong(timestamp);
	_networkEngine->printLog(3, "Event PING received. Replying... ");
}

// Fill map list from server
void	Receiver::handleMapList()
{
	S_Map->getMapDatabase()->clearList();
	while (!(_packet.endOfPacket()))
	{
		std::string mapName;

		if (!(_packet >> mapName))
		{
			_networkEngine->printLog(1, "Unable to extract map list packet");
			return;
		}
		S_Map->getMapDatabase()->addMap(mapName.c_str());
	}
}

void	Receiver::handleResetRound()
{
	S_Map->getMode()->changeRound();
	_networkEngine->printLog(2, "New round packet received");
}

void	Receiver::handleDisplayString()
{
	int lvl;
	_packet >> lvl;
	std::string str;
	_packet >> str;

	t_msgHandler	*handler = new t_msgHandler();
	handler->_msg = str;
	handler->_id = 0;
	handler->_isNew = true;
	handler->_level = lvl;

	ADD_EVENT(ev_CHAT_MSG_RECEIVED, s_event(NULL, handler));
}

void	Receiver::handleServerFull()
{
	ADD_EVENT_SIMPLE(ev_SERVER_FULL);
}

// TMP FOR FUN - Should not handle kick via network event client side
void	Receiver::handleKick()
{
	exit(0);
}

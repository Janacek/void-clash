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

#include	"Map.hpp"

extern t_config *_conf;

///////////////////////////////////////////////
/////   Handle generic packet

void	Receiver::handleUpdate()
{
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
			// 
			// if (S_Map->getCurrentPlayer()->getLatency() > 100.0f)
			//   {	
			//     _networkEngine->printLog(2, "Laggy packet received. Dropping it...");
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
	_networkEngine->printLog(3, "Update packet received");
	while (!(_packet.endOfPacket()))
		extractWorldData();
}

void	Receiver::handleWelcome()
{
	_networkEngine->printLog(2, "Welcome packet received", VIOLET);
	S_Map->clearMap();

	// time update extraction
	sf::Int32	ms;
	_packet >> ms;
	sf::Time time = sf::milliseconds(ms);
	S_Map->setJoinServerTime(time);

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
		S_Map->getCurrentPlayer()->setJoinTime(time);
	else
	{
		std::cout << "Unable to connect to server, exiting" << std::endl;
		exit(0);
	}
}

void	Receiver::handleSynchro()
{
	//  _networkEngine->printLog(2, "Synchro packet received", VIOLET);
	// score update
	while (!(_packet.endOfPacket()))
		extractWorldData();
}

///////////////////////////////////////////////
/////   Handle events packet

void	Receiver::handleLaunchEvent()
{
	sf::Uint32	id;
	_packet >> id;
	if (id == 0)
		return;

	AObject	*obj = S_Map->findObjectWithID(id);
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

void	Receiver::handleDeleteEvent()
{
	sf::Uint32	id;
	_packet >> id;
	if (id == 0)
		return;

	AObject	*obj = S_Map->findObjectWithID(id);
	if (!obj)
	{
		_networkEngine->printLogWithId(1, "Unable to find object with id ", id);
		return;
	}
	Event::addEvent(ev_DELETE, s_event((obj)));

	// log
	if (obj->getType() == PLAYER)
		_networkEngine->printLogWithId(2, "Player is flagged as disconnected. Id is ", id);
}


void	Receiver::handlePlayerHitEvent()
{
	sf::Uint32	idHitter;
	sf::Uint32	idHitted;

	_packet >> idHitter;
	_packet >> idHitted;

	AObject	*hitter = S_Map->findObjectWithID(idHitter);
	AObject	*hitted = S_Map->findObjectWithID(idHitted);
	Event::addEvent(ev_PLAYER_HIT, s_event(hitter, hitted));
	if (hitter)
		Event::addEvent(ev_DELETE, s_event(hitter));
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

	Player *killer = dynamic_cast<Player *>(S_Map->findObjectWithID(idKiller));
	Player *killed = dynamic_cast<Player *>(S_Map->findObjectWithID(idKilled));
	if (killer && killed)
	{
		Event::addEvent(ev_KILL, s_event(killer, killed));
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
			_networkEngine->printLogWithId(2, "Event KILL packet received for killer's id ", idKiller, VIOLET);
		}
		else
			_networkEngine->printLogWithId(2, "Event KILL packet (suicide !) received for player ", idKiller, VIOLET);
	}
}

void	Receiver::handleChatEvent()
{
	sf::String	msg;
	sf::Uint32	id;

	_packet >> id;
	_packet >> msg;

	t_msgHandler	*handler = new t_msgHandler();
	handler->_msg = msg;
	handler->_id = id;
	handler->_isNew = true;
	HudRessources::getInstance()->getChat().push_front(handler);
	_networkEngine->printLogWithId(2, "Event CHAT packet received: \"" + msg + "\". Received from id ", id, VIOLET);
}


void	Receiver::handleIdentification()
{
	sf::Uint32	id;
	_packet >> id;

	S_Map->setCurrentPlayerId(id);
	_networkEngine->getSender()->sendConfirmation(PACKET_EVENT_IDENT);
	_networkEngine->printLogWithId(2, "Event IDENTIFICATION packet received. You are player ", id, VIOLET);
}

void	Receiver::handleCaptureFlag()
{
	sf::Uint32	idFlag;
	sf::Uint32	idPlayer;

	_packet >> idFlag;
	_packet >> idPlayer;
	Flag *flag = dynamic_cast<Flag *>(S_Map->findObjectWithID(idFlag));
	Player *player = dynamic_cast<Player *>(S_Map->findObjectWithID(idPlayer));

	flag->reset();
	_networkEngine->printLogWithId(2, "Event CAPTURE_FLAG packet received. Id flag is ", idFlag, VIOLET);
	// // inc personal score by 5
	// for (int i = 0; i < 5; i++)
	//   player->plusKills();
	// // inc team score by 1
	if (player->getTeam() == 1)
		S_Map->incScore(1, 0);
	if (player->getTeam() == 2)
		S_Map->incScore(0, 1);
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

	AObject	*obj = S_Map->findObjectWithID(id);
	if (obj)
	{
		Event::addEvent(ev_TURRET_DESTROYED, s_event(obj));
		Event::addEvent(ev_DELETE, s_event(obj));
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
		AObject *obj;
		obj = S_Map->findObjectWithID(id);
		Player *player = dynamic_cast<Player *>(obj);
		if (player == NULL)
			return;

		t_weapon	*primary = NULL;
		t_weapon	*primaryAlt = NULL;
		t_weapon	*secondary = NULL;
		t_weapon	*secondaryAlt = NULL;

		sf::Int16	index;
		_packet >> index;
		if (index != -1)
			primary = _conf->weapons->at(index);

		_packet >> index;
		if (index != -1)
			primaryAlt = _conf->weapons->at(index);

		_packet >> index;
		if (index != -1)
			secondary = _conf->weapons->at(index);

		_packet >> index;
		if (index != -1)
			secondaryAlt = _conf->weapons->at(index);

		player->setWeapons(primary, primaryAlt, secondary, secondaryAlt);
	}
}

void	Receiver::handleConfig()
{
	// Reset weapons
	std::list<Player *>::iterator	it = S_Map->getPlayers()->begin();
	std::list<Player *>::iterator	end = S_Map->getPlayers()->end();
	while (it != end)
	{
		Player *player = dynamic_cast<Player *>(*it);
		if (player)
			player->setWeapons(NULL, NULL, NULL, NULL);
		++it;
	}

	// Set new cfg
	std::string	result;
	if (!(_packet >> result))
	{
		_networkEngine->printLog(1, "Unable to extract CONFIG packet");
		return;
	}
	ConfigParser	*parse = new ConfigParser;

	parse->loadString(result);
	_conf = (t_config *)parse->parse();
	_networkEngine->printLog(1, "Event CONFIG packet received. Modifying config file...");
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
	S_Map->getMapManager()->clearList();
	while (!(_packet.endOfPacket()))
	{
		std::string mapName;

		if (!(_packet >> mapName))
		{
			_networkEngine->printLog(1, "Unable to extract map list packet");
			return;
		}
		S_Map->getMapManager()->addMap(mapName.c_str());
	}
}

void	Receiver::handleResetRound()
{
	S_Map->getMode()->changeRound();
	_networkEngine->printLog(2, "New round packet received");
}

// TMP FOR FUN - Should not handle kick via network event client side
void	Receiver::handleKick()
{
	exit(0);
}

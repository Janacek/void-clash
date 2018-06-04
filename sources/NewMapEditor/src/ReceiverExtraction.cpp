//
// ReceiverExtraction.cpp for Void Clash in /home/gegon/pfa/sources/client/Common/src
// 
// Made by Gegon
// Login   <gegon@epitech.net>
// 
// Started on  Sun Dec 15 23:56:17 2013 Gegon
// Last update Tue Feb 25 20:27:16 2014 Gegon
//

#include	"NetworkEngine.hpp"
#include	"Receiver.hpp"
#include	"Manager.hpp"

#include        "Turret.hpp"
#include        "Bullet.hpp"
#include        "Rocket.hpp"
#include        "Bomb.hpp"
#include        "Flag.hpp"
#include        "Bot.hpp"
#include        "Explosion.hpp"
#include        "GravityField.hpp"
#include        "SpeedField.hpp"
#include        "Capture.hpp"
#include        "Respawn.hpp"
#include        "Wall.hpp"
#include	"main.hpp"
#include	"HudRessources.hpp"

#include	"Map.hpp"

extern bool	GIsOffline;
extern bool	GIsServer;
extern t_config *_conf;

///////////////////////////////////////////////
/////   Overload of operator >>
/////	to fill our classes with received data from serv

sf::Packet& operator >>(sf::Packet& packet, Player &m)
{
	return packet >> m._aim.first >> m._aim.second >> m._energy >> m._life >> m._kills >> m._deaths >> m._nick >> m._latency;
}

sf::Packet& operator >>(sf::Packet& packet, Bullet &m)
{
	return packet >> m._pos.first >> m._pos.second >> m._dir.first >> m._dir.second;
}

sf::Packet& operator >>(sf::Packet& packet, Bomb &m)
{
	return packet >> m._pos.first >> m._pos.second >> m._dir.first >> m._dir.second >> m._launched;
}

sf::Packet& operator >>(sf::Packet& packet, Rocket &m)
{
	return packet >> m._pos.first >> m._pos.second >> m._dir.first >> m._dir.second >> m._impact.first >> m._impact.second;
}

sf::Packet& operator >>(sf::Packet& packet, Turret &m)
{
	return packet >> m._pos.first >> m._pos.second >> m._dir.first >> m._dir.second;
}

sf::Packet& operator >>(sf::Packet& packet, Bot &m)
{
	return packet >> m._pos.first >> m._pos.second >> m._dir.first >> m._dir.second;
}

sf::Packet& operator >>(sf::Packet& packet, GravityField &m)
{
	return packet >> m._pos.first >> m._pos.second >> m._dir.first >> m._dir.second;
}

sf::Packet& operator >>(sf::Packet& packet, Explosion &m)
{
	return packet >> m._pos.first >> m._pos.second >> m._dir.first >> m._dir.second;
}

sf::Packet& operator >>(sf::Packet& packet, SpeedField &m)
{
	return packet >> m._pos.first >> m._pos.second >> m._dir.first >> m._dir.second >> m._width >> m._height;
}

sf::Packet& operator >>(sf::Packet& packet, Capture &m)
{
	return packet >> m._pos.first >> m._pos.second >> m._dir.first >> m._dir.second >> m._width >> m._height >> m._controlBar;
}

sf::Packet& operator >>(sf::Packet& packet, Respawn &m)
{
	return packet >> m._pos.first >> m._pos.second >> m._dir.first >> m._dir.second >> m._width >> m._height >> m._team;
}

sf::Packet& operator >>(sf::Packet& packet, Flag &m)
{
	return packet >> m._pos.first >> m._pos.second >> m._dir.first >> m._dir.second >> m._team >> m._initPos.first >> m._initPos.second;
}

sf::Packet& operator >>(sf::Packet& packet, Wall &m)
{
	return packet >> m._pos.first >> m._pos.second >> m._end.first >> m._end.second >> m._angle >> m._dir.first >> m._dir.second >> m._len;
}

///////////////////////////////////////////////
/////   Extract all data

void	Receiver::extractWorldData()
{
	sf::Uint32		packetType;
	_packet >> packetType;
	if (packetType == PACKET_EVENT_DELETE)
	{
		handleDeleteEvent();
		return;
	}
	sf::Uint32	id;
	_packet >> id;
	if (packetType == PACKET_EVENT_ADD)
	{
		sf::Uint32		objType;
		_packet >> objType;

		if (!(extractObject(objType, id)))
		{
			_networkEngine->printLog(1, "Unknown type of object. Clearing packet...");
			_packet.clear();
		}
	}
}

bool	Receiver::extractObject(sf::Uint32 type, sf::Uint32 id)
{
	if (type == BULLET)
	{
		Bullet *bullet = extractWeapons<Bullet>(id);
		if (bullet)
		{
			Event::addEvent(ev_BULLET_LAUNCHED, s_event(bullet->getOwner()));
		}
	}
	else if (type == BOMB)
	{
		Bomb *bomb = extractWeapons<Bomb>(id);
		if (bomb)
		{
			Event::addEvent(ev_BOMB_PRIMED, s_event(bomb->getOwner()));
			bomb->getOwner()->setBombOnHold(bomb);
		}
	}
	else if (type == ROCKET)
	{
		Rocket *rocket = extractWeapons<Rocket>(id);
		if (rocket)
			Event::addEvent(ev_ROCKET_LAUNCHED, s_event(rocket->getOwner()));
	}
	else if (type == TURRET)
	{
		Turret *turret = extractWeapons<Turret>(id);
		if (turret)
			Event::addEvent(ev_TURRET_LAUNCHED, s_event(turret->getOwner()));
	}
	else if (type == GRAVITY_FIELD)
	{
		GravityField *gravity = extractWeapons<GravityField>(id);
		if (gravity)
			Event::addEvent(ev_GRAVITY_LAUNCHED, s_event(gravity->getOwner()));
	}
	else if (type == BOT)
		extractObject<Bot>(id);
	else if (type == SPEED_FIELD)
		extractObject<SpeedField>(id);
	else if (type == CAPTURE)
		extractObject<Capture>(id);
	else if (type == RESPAWN)
		extractObject<Respawn>(id);
	else if (type == FLAG)
		extractObject<Flag>(id);
	else if (type == WALL)
		extractObject<Wall>(id);
	else if (type == PLAYER)
		updatePlayer(id);
	else
		return (false);
	return (true);
}

// Check if object exist
// if yes - update obj
// if no - create it and push it to list

template<typename T>
void	Receiver::extractObject(sf::Uint32 id)
{
	AObject	*obj;
	obj = S_Map->findObjectWithID(id);

	T *templatedObj;
	if (obj)
		templatedObj = dynamic_cast<T *>(obj);
	else
	{
		templatedObj = new T();
		templatedObj->pushInMap();
	}
	_packet >> *templatedObj;
	templatedObj->setId(id);
}

// If it's controled player object - do not create it via network -> return
template<typename T>
T	*Receiver::extractWeapons(sf::Uint32 id)
{
	sf::Uint32	ownerId;
	_packet >> ownerId;
	// TO VERIFY - Own dynamic obj are no longer created locally
	// if (S_Map->getCurrentPlayer() && ownerId == S_Map->getCurrentPlayer()->getId())
	//   {
	//     _packet.clear();
	//     return NULL;
	//   }

	AObject *objOwner = S_Map->findObjectWithID(ownerId);
	Player *owner = dynamic_cast<Player *>(objOwner);
	if (!owner)
	{
		_networkEngine->printLogWithId(1, "No owner for dynamic object found for item ", id);
		return NULL;
	}

	AObject *obj = S_Map->findObjectWithID(id);
	T *templatedObj;
	if (obj)
	{
		templatedObj = dynamic_cast<T *>(obj);
		_packet >> *templatedObj;
		templatedObj->setOwner(owner);
		templatedObj->setId(id);

		sf::Int32  weaponIndex;
		_packet >> weaponIndex; // Just to purge
	}
	else
	{
		templatedObj = new T();
		templatedObj->pushInMap();
		_packet >> *templatedObj;
		templatedObj->setOwner(owner);
		templatedObj->setId(id);

		sf::Int32  weaponIndex;
		_packet >> weaponIndex;
		templatedObj->init(_conf->weapons->at(weaponIndex));
	}

	// simulateUpdatePhysObject(templatedObj);
	// TODO - Call physic engine simulate function
	return templatedObj;
}

///////////////////////////////////////////////
/////   Update player

Player *ghost = NULL;

void	Receiver::updatePlayer(sf::Uint32 id)
{
	sf::Uint16	team;
	_packet >> team;
	float	posX;
	_packet >> posX;
	float	posY;
	_packet >> posY;
	float	dirX;
	_packet >> dirX;
	float	dirY;
	_packet >> dirY;

	//  Check if player found
	AObject	*obj = S_Map->findObjectWithID(id);
	Player	*player = dynamic_cast<Player *>(obj);
	if (player)
	{
		player->setTeam(team);
		 if (!player->isControled())
		{
			player->setDirX(dirX);
			player->setDirY(dirY);
		}
		// first var is the latency hard coded 10ms atm
		// S_Map->getCurrentPlayer()->getLatency() / 2.0f = last ping

		// 4 ATM NEED 2 WITH NEW PHY ENGINE
		float	latency = 0.0f;
		if (S_Map->getCurrentPlayer())
			latency = S_Map->getCurrentPlayer()->getLatency() / 4.0f / 20000.0f / _conf->gamespeed;

		float range = sqrt(std::pow(posX +
			dirX * latency
			- player->getX(), 2) +
			std::pow(posY +
			dirY * latency
			- player->getY(), 2));
		float speed = _conf->player->max_speed + sqrt(std::pow(player->getDirX(), 2) +
			std::pow(player->getDirY(), 2));
		// Ghost
		// if (GIsServer == false && GIsOffline == false)
		// 	{
		// 	  if (ghost == NULL)
		// 	    {
		// 	      ghost = new Player(0,0,0,0,false);
		// 	      S_Map->addPlayer(ghost, 2);
		// 	    }
		// 	  ghost->setPosition(posX //+ latency * dirX
		// 			     , posY //+ latency * dirY
		// 			     );
		// 	}

		if (range > speed / 3.0f)
		{
			_networkEngine->printLog(1, "Reset position with server");
			player->setPosition((posX), (posY));
		}
		// smooth
		player->setPosition((5.0f * player->getX() + posX + player->getDirX() * latency) / 6.0f,
			(5.0f * player->getY() + posY + +player->getDirY() * latency) / 6.0f);
		// bruteforce
		// player->setPosition(posX, posY);
		_packet >> *player;
	}
	else
		registerNewPlayer(id, team);
}

void	Receiver::registerNewPlayer(sf::Uint32 id, sf::Uint16 team)
{
	Player	*player;

	// Verify that there is not already an event start fort this player (2+ packets receive in same frame)
	if (Event::eventAlreadyAddedForId(ev_START, id))
	{
		// used only to unload packet
		player = new Player(0, 0, 0, 0, true);
		_packet >> *player;
		return;
	}

	// Check if new player is myself
	if (id == S_Map->getCurrentPlayerId())
	{
		_networkEngine->printLogWithId(2, "Controled player received from server with ID ", id, DARK_BLUE);
		player = new Player(0, 0, 0, 0, true);
	}
	else // create player not controled
	{
		_networkEngine->printLogWithId(2, "New uncontroled player received from server with ID ", id, LIGHT_BLUE);
		player = new Player(0, 0, 0, 0, false);
	}

	_packet >> *player;
	player->setId(id);
	S_Map->addPlayer(player, team);
}

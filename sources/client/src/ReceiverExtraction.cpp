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

extern bool	G_isOffline;
extern bool	G_isServer;
extern t_config *G_conf;

///////////////////////////////////////////////
/////   Overload of operator >>
/////	to fill our classes with received data from serv

sf::Packet& operator >>(sf::Packet& packet, Player &m)
{
	return packet >> m._energy >> m._life >> m._kills >> m._deaths >> m._latency >> m._score >> m._invul;
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

bool	Receiver::extractWorldData()
{
	sf::Uint32		packetType;
	_packet >> packetType;
	sf::Uint32	id;
	_packet >> id;
	if (packetType == PACKET_EVENT_DELETE)
	{
		handleDeleteEvent(id);
		return true;
	}
	else if (packetType == PACKET_EVENT_ADD)
	{
		sf::Uint32		objType;
		_packet >> objType;

		if (!(extractObject(objType, id)))
		{
			_networkEngine->printLog(1, "Unknown type of object. Clearing packet...");
			_packet.clear();
			return false;
		}
	}
	else
	{
		_networkEngine->printLog(1, "Unknown type of extract world data.");
		_packet.clear();
		return false;
	}
	return true;
}

bool	Receiver::extractObject(sf::Uint32 type, sf::Uint32 id)
{
	bool created;
	if (type == BULLET)
	{
		std::shared_ptr<Bullet> bullet = extractWeapons<Bullet>(id, created);
		if (bullet && created)
		{
			ADD_EVENT(ev_BULLET_LAUNCHED, s_event(bullet->getOwner()));
		}
	}
	else if (type == BOMB)
	{
		std::shared_ptr<Bomb>	bomb = extractWeapons<Bomb>(id, created);
		if (bomb && created)
		{
			ADD_EVENT(ev_BOMB_PRIMED, s_event(bomb->getOwner()));
			bomb->getOwner()->setBombOnHold(bomb);
		}
	}
	else if (type == ROCKET)
	{
		std::shared_ptr<Rocket> rocket = extractWeapons<Rocket>(id, created);
		if (rocket && created)
			ADD_EVENT(ev_ROCKET_LAUNCHED, s_event(rocket->getOwner()));
	}
	else if (type == TURRET)
	{
		std::shared_ptr<Turret> turret = extractWeapons<Turret>(id, created);
		if (turret && created)
			ADD_EVENT(ev_TURRET_LAUNCHED, s_event(turret->getOwner()));
	}
	else if (type == GRAVITY_FIELD)
	{
		std::shared_ptr<GravityField> gravity = extractWeapons<GravityField>(id, created);
		if (gravity && created)
			ADD_EVENT(ev_GRAVITY_LAUNCHED, s_event(gravity->getOwner()));
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
	{
		extractObject<Flag>(id);
		int ownerId;
		_packet >> ownerId;
		Flag *f = dynamic_cast<Flag *> (S_Map->findObjectWithID(id).get());
		if (f)
			f->setOwner(std::dynamic_pointer_cast<Player>(S_Map->findObjectWithID(ownerId)));
	}
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
	std::shared_ptr<AObject>obj = S_Map->findObjectWithID(id);
	std::shared_ptr<T> templatedObj;
	if (obj)
		templatedObj = std::dynamic_pointer_cast<T>(obj);
	else
	{
		templatedObj = std::make_shared<T>();
		templatedObj->pushInMap();
	}
	if (!templatedObj)
	{
		_networkEngine->printLogWithId(1, "Unable to dynamic cast extracted object ", id);
		_packet.clear();
		return;
	}
	_packet >> *templatedObj;
	templatedObj->setId(id);
	return;
}

// If it's controled player object - do not create it via network -> return
template<typename T>
std::shared_ptr<T>	Receiver::extractWeapons(sf::Uint32 id, bool &created)
{
	created = false;
	sf::Uint32	ownerId;
	_packet >> ownerId;
	// TO VERIFY - Own dynamic obj are no longer created locally
	// if (S_Map->getCurrentPlayer() && ownerId == S_Map->getCurrentPlayer()->getId())
	//   {
	//     _packet.clear();
	//     return NULL;
	//   }

	std::shared_ptr<AObject>objOwner = S_Map->findObjectWithID(ownerId);
	std::shared_ptr<Player>owner = std::dynamic_pointer_cast<Player>(objOwner);
	if (!owner)
	{
		_networkEngine->printLogWithId(1, "No owner for dynamic object found for item ", id);
		return NULL;
	}

	std::shared_ptr<AObject>obj = S_Map->findObjectWithID(id);
	std::shared_ptr<T>templatedObj;
	if (obj)
	{
		std::shared_ptr<T> templatedObj = std::dynamic_pointer_cast<T>(obj);
		if (!templatedObj)
		{
			_networkEngine->printLogWithId(1, "Unable to dynamic cast object ", id);
			_packet.clear();
			return NULL;
		}
		_packet >> *templatedObj;
		templatedObj->setOwner(owner);
		templatedObj->setId(id);

		sf::Int32  weaponIndex;
		_packet >> weaponIndex; // Just to purge
	}
	else
	{
		created = true;
		templatedObj = std::make_shared<T>();
		templatedObj->pushInMap();
		_packet >> *(templatedObj.get());
		templatedObj->setOwner(owner);
		templatedObj->setId(id);

		sf::Int32  weaponIndex;
		_packet >> weaponIndex;
		templatedObj->init(G_conf->weapons->at(weaponIndex));
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
	float	aimX;
	_packet >> aimX;
	float	aimY;
	_packet >> aimY;
	int weaponIndex;
	_packet >> weaponIndex;

	//  Check if player found
	AObject	*obj = S_Map->findObjectWithID(id).get();
	Player	*player = dynamic_cast<Player *>(obj);
	if (player)
	{
		player->setActive(true);
		player->setTeam(team);
		if (!player->isControled())
		{
			player->setDirX(dirX);
			player->setDirY(dirY);
			player->setAim(aimX, aimY);
		}
		// first var is the latency hard coded 10ms atm
		// S_Map->getCurrentPlayer()->getLatency() / 2.0f = last ping

		// 4 ATM NEED 2 WITH NEW PHY ENGINE
		float	latency = 0.0f;
		//if (S_Map->getCurrentPlayer())
		//	latency = S_Map->getCurrentPlayer()->getLatency() / 4.0f / 20000.0f / G_conf->game->speed;

		float range = sqrt(std::pow(posX +
			dirX * latency
			- player->getX(), 2) +
			std::pow(posY +
			dirY * latency
			- player->getY(), 2));
		float speed = G_conf->player->max_speed + sqrt(std::pow(player->getDirX(), 2) +
			std::pow(player->getDirY(), 2));
		// Ghost
		//if (G_isServer == false && G_isOffline == false)
		//	{
		//	  if (ghost == NULL)
		//	    {
		//	      ghost = new Player(0,0,0,0,false);
		//	      S_Map->addPlayer(ghost, 2);
		//	    }
		//	  ghost->setPosition(posX //+ latency * dirX
		//			     , posY //+ latency * dirY
		//			     );
		//	}

		if (range > speed / 3.0f)
		{
			//_networkEngine->printLog(1, "Reset position with server");
			player->setPosition((posX), (posY));
		}
		// smooth
		float smoothPower = G_conf->server->tickrate / 32.f;
		player->setPosition((smoothPower * player->getX() + posX) / (smoothPower + 1),
			(smoothPower * player->getY() + posY) / (smoothPower + 1));
		//player->setPosition((smoothPower * player->getX() + posX + player->getDirX() * latency) / smoothPower + 1,
		//	(smoothPower * player->getY() + posY + player->getDirY() * latency) / smoothPower + 1);
		// bruteforce
		// player->setPosition(posX, posY);
		if (weaponIndex == -1)
			player->setShield(NULL);
		else
			player->setShield(G_conf->weapons->at(weaponIndex));
		_packet >> *player;

		if (_packetType == PACKET_SYNCHRO)
		{
			std::string nick;
			_packet >> nick;
			player->setNick(nick);
		}
	}
	else
		registerNewPlayer(id, team);
}

void	Receiver::registerNewPlayer(sf::Uint32 id, sf::Uint16 team)
{
	// Verify that there is not already an event start fort this player (2+ packets receive in same frame)
	if (Event::eventAlreadyAddedForId(ev_START, id))
	{
		// used only to unload packet
		//Player p(0, 0, 0, 0, true);
		//_packet >> p;
		//delete player;

		_packet.clear();
		return;
	}

	std::shared_ptr<Player>	player;
	// Check if new player is myself
	if (id == S_Map->getCurrentPlayerId())
	{
		_networkEngine->printLogWithId(2, "Controled player received from server with ID ", id, DARK_BLUE);
		player = std::make_shared<Player>(0, 0, 0, 0, true);
	}
	else // create player not controled
	{
		_networkEngine->printLogWithId(2, "New uncontroled player received from server with ID ", id, LIGHT_BLUE);
		player = std::make_shared<Player>(0, 0, 0, 0, false);
	}

	_packet >> *player;
	player->setId(id);
	S_Map->addPlayer(player, team);
}

//
// SenderInsertion.cpp for Void Clash in /home/gegon/pfa/sources/server/NetworkEngine/src
// 
// Made by Gegon
// Login   <gegon@epitech.net>
// 
// Started on  Thu Dec 26 15:06:12 2013 Gegon
// Last update Mon Jan 13 18:33:07 2014 Gegon
//

#include	"main.hpp"
#include	"Manager.hpp"
#include	"NetworkEngine.hpp"
#include	"ClientHandle.hpp"
#include	"Sender.hpp"
#include	"Turret.hpp"
#include	"Bullet.hpp"
#include	"GravityField.hpp"
#include	"Bomb.hpp"
#include	"Bot.hpp"
#include	"Rocket.hpp"
#include	"Flag.hpp"
#include	"Explosion.hpp"
#include	"GravityField.hpp"
#include	"SpeedField.hpp"
#include	"Capture.hpp"
#include	"Respawn.hpp"
#include	"Event.hpp"
#include	"Map.hpp"

///////////////////////////////////////////////
/////   Overload of operator <<
/////	to create packet with classes info

sf::Packet& operator <<(sf::Packet& packet, const Player &m)
{
	return packet << m.getEnergy() << m.getLife() << m.getKills() << m.getDeaths() << m.getLatency() << m.getScore() << m.isInvulnerable();
}

sf::Packet& operator <<(sf::Packet& packet, const Bullet &m)
{
	return packet << m.getX() << m.getY() << m.getDirX() << m.getDirY();
	// X / Y / DIRX / DIRY
}

sf::Packet& operator <<(sf::Packet& packet, const GravityField &m)
{
	return packet << m.getX() << m.getY() << m.getDirX() << m.getDirY();
	// X / Y / DIRX / DIRY
}

sf::Packet& operator <<(sf::Packet& packet, const Bomb &m)
{
	return packet << m.getX() << m.getY() << m.getDirX() << m.getDirY() << m.isLaunched();
	// X / Y / DIRX / DIRY / LAUNCHED
}

sf::Packet& operator <<(sf::Packet& packet, const Rocket &m)
{
	return packet << m.getX() << m.getY() << m.getDirX() << m.getDirY() << m.getImpact().first << m.getImpact().second;
	// X / Y / DIRX / DIRY / LAUNCHED
}


sf::Packet& operator <<(sf::Packet& packet, const Turret &m)
{
	return packet << m.getX() << m.getY() << m.getDirX() << m.getDirY();
	// X / Y / DIRX / DIRY
}

sf::Packet& operator <<(sf::Packet& packet, const Bot &m)
{
	return packet << m.getX() << m.getY() << m.getDirX() << m.getDirY();
	// X / Y / DIRX / DIRY
}


sf::Packet& operator <<(sf::Packet& packet, const Explosion &m)
{
	return packet << m.getX() << m.getY() << m.getDirX() << m.getDirY();
	// X / Y / DIRX / DIRY
}

sf::Packet& operator <<(sf::Packet& packet, const SpeedField &m)
{
	return packet << m.getX() << m.getY() << m.getDirX() << m.getDirY() << m.getWidth() << m.getHeight();
	// X / Y / DIRX / DIRY / WIDTH / HEIGTH
}

sf::Packet& operator <<(sf::Packet& packet, const Capture &m)
{
	return packet << m.getX() << m.getY() << m.getDirX() << m.getDirY() << m.getWidth() << m.getHeight() << m.getControledStatus();
	// X / Y / DIRX / DIRY / WIDTH / HEIGTH / times
}

sf::Packet& operator <<(sf::Packet& packet, const Respawn &m)
{
	return packet << m.getX() << m.getY() << m.getDirX() << m.getDirY() << m.getWidth() << m.getHeight() << m.getTeam();
	// X / Y / DIRX / DIRY / WIDTH / HEIGTH / TEAM
}

sf::Packet& operator <<(sf::Packet& packet, const Flag &m)
{
	return packet << m.getX() << m.getY() << m.getDirX() << m.getDirY() << m.getTeam() << m.getInitX() << m.getInitY();
	// X / Y / DIRX / DIRY / WIDTH / HEIGTH / TEAM
}

sf::Packet& operator <<(sf::Packet& packet, const Wall &m)
{
	return packet << m.getX() << m.getY() << m.getEndX() << m.getEndY() << m.getAngle() << m.getDirX() << m.getDirY() << m.getLen();
	// ID / X / Y / ENDX / ENDY / ANGLE / DIRX / DIRY / LEN
}

///////////////////////////////////////////////
/////   Create a packet and send it to ALL clients
/////	to update their existing classes
/////	or create new class

void	Sender::createPacket(ePacketType packetType)
{
	// Insert player's info
	for (std::list<std::shared_ptr<Player>>::const_iterator it = S_Map->getPlayers()->begin(); it != S_Map->getPlayers()->end(); it++)
	{
		Player	*player = dynamic_cast<Player *>((*it).get());
		int shieldIndex = -1;
		if (player->getShield())
			shieldIndex = player->getShield()->getWeaponIndex();

		_packet << PACKET_EVENT_ADD << player->getId() << player->getType() << player->getTeam() << player->getX() << player->getY() << player->getDirX() << player->getDirY() << player->getAim().first << player->getAim().second << shieldIndex << *player;
		if (packetType == PACKET_SYNCHRO)
			_packet << player->getNick();
	}
	insertAddedObj();
	insertDeletedObj();

	// All world data (walls / speedfields...)
	if (packetType == PACKET_WELCOME || packetType == PACKET_SYNCHRO)
		insertWorldData();

	// // DEBUG - Send it to all clients
	// int	clientNb = _networkEngine->getNbClient();
	// if (DEBUG_LEVEL >= 3 && clientNb)
	//   {
	//     if (clientNb == 1)
	// 	std::cout << DARK_BLUE << _networkEngine->getManager()->getClock().getElapsedTime().asMilliseconds()  << " - Sent update packet (" << _packet.getDataSize() << " bytes) to 1 client" << RESET << std::endl;
	//     else
	// 	std::cout << DARK_BLUE << _networkEngine->getManager()->getClock().getElapsedTime().asMilliseconds()  << " - Sent update packet (" << _packet.getDataSize() << "bytes) to " << clientNb << " clients" << RESET << std::endl;
	//   }
}

// Called each tick
void	Sender::insertDeletedObj()
{
	if (Event::getEventByType(ev_DELETE) == NULL)
		return;
	std::list<std::pair<eventType, s_event> >::iterator it =
		Event::getEventByType(ev_DELETE)->begin();

	while (it != Event::getEventByType(ev_DELETE)->end())
	{
		//if ((*it).second.trigger && ((*it).second.trigger->getType() == PLAYER || (*it).second.trigger->getType() == TURRET))
		if ((*it).second.trigger && ((*it).second.trigger->getType() == PLAYER))
			_packet << PACKET_EVENT_DELETE << (*it).second.trigger->getId();
		++it;
	}
}

// Called each tick
void	Sender::insertAddedObj()
{
	if (Event::getEventByType(ev_START) == NULL)
		return;
	std::list<std::pair<eventType, s_event> >::iterator it =
		Event::getEventByType(ev_START)->begin();

	while (it != Event::getEventByType(ev_START)->end())
	{
		if ((*it).second.trigger->getType() == BULLET)
			insertWeapons<Bullet>((*it).second.trigger.get());
		else if ((*it).second.trigger->getType() == BOMB)
			insertWeapons<Bomb>((*it).second.trigger.get());
		else if ((*it).second.trigger->getType() == ROCKET)
			insertWeapons<Rocket>((*it).second.trigger.get());
		else if ((*it).second.trigger->getType() == TURRET)
			insertWeapons<Turret>((*it).second.trigger.get());
		else if ((*it).second.trigger->getType() == GRAVITY_FIELD)
			insertWeapons<GravityField>((*it).second.trigger.get());
		else if ((*it).second.trigger->getType() == BOT)
			insertObject<Bot>((*it).second.trigger.get());
		++it;
	}
}

// Call on welcome / synch
void	Sender::insertWorldData()
{
	// ELEMS OBJ
	{
		auto	it = S_Map->getElems()->begin();
		auto	end = S_Map->getElems()->end();
		while (it != end)
		{
			if (!S_Map->checkIfDeleteEventForObj(*it))
			{
				if ((*it)->getType() == SPEED_FIELD)
					insertObject<SpeedField>((*it).get());
				if ((*it)->getType() == CAPTURE)
					insertObject<Capture>((*it).get());
				else if ((*it)->getType() == RESPAWN)
					insertObject<Respawn>((*it).get());
				else if ((*it)->getType() == FLAG)
				{
					insertObject<Flag>((*it).get());
					Flag *f = dynamic_cast<Flag *>((*it).get());
					if (f)
					{
						if (f->getOwner())
							_packet << f->getOwner()->getId();
						else
							_packet << 0;
					}
				}
				else if ((*it)->getType() == BOT)
					insertObject<Bot>((*it).get());
				//else if ((*it)->getType() == TURRET)
				//	insertWeapons<Turret>((*it));
				//else if ((*it)->getType() == GRAVITY_FIELD)
				//	insertWeapons<GravityField>((*it));
			}
			++it;
		}
	}

	// WALLS
	auto	it = S_Map->getWalls()->begin();
	auto	end = S_Map->getWalls()->end();
	while (it != end)
	{
		insertObject<Wall>((*it).get());
		++it;
	}
}

// World object
template<typename T>
void	Sender::insertObject(AObject *obj)
{
	T	*templatedObj;
	templatedObj = dynamic_cast<T *>(obj);
	if (templatedObj)
		_packet << PACKET_EVENT_ADD << obj->getId() << obj->getType() << *templatedObj;
}

// Weapons (bullet / bomb...)
template<typename T>
void	Sender::insertWeapons(AObject *obj)
{
	T	*templatedObj;
	templatedObj = dynamic_cast<T *>(obj);

	if (templatedObj)
	{
		unsigned int ownerId = 0;
		if (templatedObj && templatedObj->getOwner())
			ownerId = templatedObj->getOwner()->getId();
		_packet << PACKET_EVENT_ADD << obj->getId() << obj->getType() << ownerId << *templatedObj << templatedObj->getWeaponIndex();
	}
}

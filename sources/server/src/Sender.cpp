//
// Sender.cpp for Void Clash in /home/gegon/pfa/server
// 
// Made by Gegon
// Login   <gegon@epitech.net>
// 
// Started on  Wed Sep 25 17:25:10 2013 Gegon
// Last update Sat Feb 15 07:21:03 2014 Gegon
//

#include	"main.hpp"
#include	"Manager.hpp"
#include	"NetworkEngine.hpp"
#include	"ClientHandle.hpp"
#include	"Sender.hpp"
#include	"Bullet.hpp"
#include	"Bomb.hpp"
#include	"Flag.hpp"
#include	"Explosion.hpp"
#include	"GravityField.hpp"
#include	"SpeedField.hpp"
#include	"Respawn.hpp"
#include	"Event.hpp"
#include	"Map.hpp"

extern t_config *G_conf;

Sender::Sender(NetworkEngine *networkEngine) :
_networkEngine(networkEngine)
{
}

Sender::~Sender()
{}

void	Sender::update()
{
	_networkEngine->pingClients();
	sendUpdateEvents();
	sendPacketGeneric(PACKET_UPDATE);
}

///////////////////////////////////////////////
/////   Send pre-made packet

void	Sender::sendPacket(sf::Packet &packet, ClientHandle *client)
{
	_packet.clear();
	_packet = packet;
	sendPacketTo(client);
}

///////////////////////////////////////////////
/////   Send packet with world infos
/////	Minimal for update to all clients
/////	All for welcome to one client

/////	If no client specified, will be sent to all

void	Sender::sendPacketGeneric(ePacketType packetType, ClientHandle *client)
{
	_packet.clear();
	_packet << packetType;
	_packetType = packetType;

	if (packetType == PACKET_SYNCHRO)
	{
		sf::Uint32 mode = S_Map->getMode()->getModeEnum();
		_packet << mode;
		_packet << S_Map->getMapDatabase()->getCurrentMapName();
		_packet << S_Map->getScore().first;
		_packet << S_Map->getScore().second;
		_packet << S_Map->getMode()->getRoundNumber();
		_packet << S_Map->getClock().getElapsedTime().asMilliseconds();
	}
	if (packetType == PACKET_UPDATE)
	{
		float timestamp;
		timestamp = S_Map->getClock().getElapsedTime().asMicroseconds();
		_packet << timestamp;
	}
	if (packetType == PACKET_WELCOME)
	{
		_packet << S_Map->getClock().getElapsedTime().asMilliseconds() << S_Map->getWarmupDuration().asMilliseconds() << S_Map->getMapDuration().asMilliseconds() << S_Map->getScore().first << S_Map->getScore().second;
	}
	createPacket(packetType);
	sendPacketTo(client);
}

///////////////////////////////////////////////
/////   Send packet about 1 specific event
/////	on one object

void	Sender::sendPacketEvent(ePacketType packetType, sf::Uint32 id, ClientHandle *client)
{
	_packetType = packetType;
	_packet.clear();
	_packet << packetType << id;
	sendPacketTo(client);
}

void	Sender::sendPacketEvent(ePacketType packetType, sf::Uint32 id, sf::IpAddress ip, unsigned short port)
{
	_packetType = packetType;
	_packet.clear();
	_packet << packetType << id;
	sendPacketTo(ip, port);
}

///////////////////////////////////////////////
/////   Send packet about 1 specific event
/////	on one object

void	Sender::sendPacketEventTwo(ePacketType packetType, sf::Uint32 id, sf::Uint32 id2, ClientHandle *client)
{
	_packetType = packetType;
	_packet.clear();
	_packet << packetType << id << id2;
	sendPacketTo(client);
}

////////
// lvl 0 = error
// 1 = success
// 2 = common
// ....

void	Sender::sendStringToDisplay(std::string str, int level, ClientHandle *client)
{
	_packetType = PACKET_DISPLAY_STRING;
	_packet.clear();
	_packet << PACKET_DISPLAY_STRING << level << str;
	sendPacketTo(client);
}

///////////////////////////////////////////////
/////   Send ping

void	Sender::sendPing(float timestamp, ClientHandle *client)
{
	_packetType = PACKET_PING;
	_packet.clear();
	_packet << PACKET_PING << timestamp;
	sendPacketTo(client);
}

///////////////////////////////////////////////
/////   Send packet to clients

void	Sender::sendPacketTo(ClientHandle *client)
{
	if (client == NULL)
	{
		for (std::list<ClientHandle *>::const_iterator it = _networkEngine->getClients().begin(); it != _networkEngine->getClients().end(); ++it)
		{
			if (_packetType != PACKET_UPDATE || (*it)->isResponsive(S_Map->getGlobalClock().getElapsedTime()))
				_networkEngine->getSocket()->send(_packet, (*it)->getIp(), (*it)->getPort());
		}
	}
	else
	{
		if (_packetType != PACKET_UPDATE || client->isResponsive(S_Map->getGlobalClock().getElapsedTime()))
			_networkEngine->getSocket()->send(_packet, (client)->getIp(), (client)->getPort());
	}
}

void	Sender::sendPacketTo(sf::IpAddress ip, unsigned short port)
{
	_networkEngine->getSocket()->send(_packet, ip, port);
}



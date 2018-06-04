//
// Sender.cpp for Void Clash in /home/gegon/pfa/client/NetworkEngine/src
// 
// Made by Gegon
// Login   <gegon@epitech.net>
// 
// Started on  Sat Oct  5 00:03:30 2013 Gegon
// Last update Wed Feb 12 06:04:19 2014 sadin_n
//

#include <cstring>
#include "Sender.hpp"
#include "NetworkEngine.hpp"
#include "Manager.hpp"
#include "Log.hpp"
#include "Bullet.hpp"
#include "Bomb.hpp"
#include "Explosion.hpp"
#include "GravityField.hpp"
#include "SpeedField.hpp"
#include "main.hpp"
#include "Map.hpp"
#include "HudRessources.hpp"
#include "AssetPath.h"

///////////////////////////////////////////////
/////   Overload of operator <<
/////	to create a packet with player's inputs

sf::Packet&	operator <<(sf::Packet &packet, s_actions *a)
{
	//	return packet << a->moveX << a->moveY << a->aimX << a->aimY << a->primary << a->secondary << a->switchPrimary << a->switchSecondary;
	return packet << a->moveX << a->moveY << a->aimX << a->aimY << a->primary << a->secondary;
}

Sender::Sender(NetworkEngine *networkEngine) :
_networkEngine(networkEngine)
{
	_playerActions = NULL;
	_packetSequence = PACKET_NONE;
}

Sender::~Sender(void)
{
}


///////////////////////////////////////////////
/////   Send player input to server

bool	Sender::update()
{
	_packet.clear();
	checkReliableUDP();

	std::shared_ptr<Player>	curPlayer = S_Map->getCurrentPlayer();
	if (!curPlayer) // Used to first contact serv
	{
		sendInput();
		return false;
	}
	sendEvents();
	sendMessage();
	sendInput();
	sendConfig();
	sendMapChange();
	sendModeChange();
	sendPlayerKick();
	sendHordeActivation();
	sendQuit();
	sendSwitchTeam();
	sendChangeNick();
	return (true);
}


void	Sender::sendConfig(void)
{
	if (Event::getEventByType(ev_CONFIG) == NULL)
		return;

	// Create packet
	ConfigParser	*parse = new ConfigParser;

	_packet.clear();
	if (!parse->loadFile(ASSETS_PATH + "config.json"))
		return;

	std::string toSend = parse->getString();
	_packet << PACKET_CONFIG << toSend;

	// Send packet to server
	if (_networkEngine->getSocket().send(_packet, _networkEngine->getServerIp(), _networkEngine->getServerPort()) != sf::Socket::Done)
		VC_WARNING_CRITICAL("Error when sending packet config to " + _networkEngine->getServerIp() + ":" + std::to_string(_networkEngine->getServerPort()));
}

void	Sender::sendMapChange(void)
{
	if (Event::getEventByType(ev_REQUEST_CHANGE_MAP) == NULL)
		return;

	_packet.clear();
	std::list<std::pair<eventType, s_event> >::const_iterator it =
		Event::getEventByType(ev_REQUEST_CHANGE_MAP)->begin();
	const char *raw = (const char *)(it->second.data);
	std::string	filename = raw;

	_packet << PACKET_REQUEST_CHANGE_MAP << filename;

	// Send packet to server
	if (_networkEngine->getSocket().send(_packet, _networkEngine->getServerIp(), _networkEngine->getServerPort()) != sf::Socket::Done)
		VC_WARNING_CRITICAL("Error when sending packet map change to " + _networkEngine->getServerIp() + ":" + std::to_string(_networkEngine->getServerPort()));
}


void	Sender::sendModeChange(void)
{
	if (Event::getEventByType(ev_SWITCH_MAP_MODE) == NULL)
		return;

	_packet.clear();
	std::list<std::pair<eventType, s_event> >::const_iterator it =
		Event::getEventByType(ev_SWITCH_MAP_MODE)->begin();
	sf::Int16	raw = (sf::Int16)(it->second.data);
	_packet << PACKET_REQUEST_CHANGE_MODE << raw;

	// Send packet to server
	if (_networkEngine->getSocket().send(_packet, _networkEngine->getServerIp(), _networkEngine->getServerPort()) != sf::Socket::Done)
		VC_WARNING_CRITICAL("Error when sending packet mode change to " + _networkEngine->getServerIp() + ":" + std::to_string(_networkEngine->getServerPort()));
}

void	Sender::sendHordeActivation(void)
{
	if (Event::getEventByType(ev_HORDE_ACTIVATE) != NULL)
	{
		_packet.clear();
		std::list<std::pair<eventType, s_event> >::const_iterator it =
			Event::getEventByType(ev_HORDE_ACTIVATE)->begin();
		sf::Uint32 id = (sf::Uint32)(it->second.data);
		_packet << PACKET_REQUEST_HORDE_ACTIVATE;

		// Send packet to server
		if (_networkEngine->getSocket().send(_packet, _networkEngine->getServerIp(), _networkEngine->getServerPort()) != sf::Socket::Done)
			VC_WARNING_CRITICAL("Error when sending horde activate to " + _networkEngine->getServerIp() + ":" + std::to_string(_networkEngine->getServerPort()));
	}

	if (Event::getEventByType(ev_HORDE_DESACTIVATE) != NULL)
	{
		_packet.clear();
		std::list<std::pair<eventType, s_event> >::const_iterator it =
			Event::getEventByType(ev_HORDE_DESACTIVATE)->begin();
		sf::Uint32 id = (sf::Uint32)(it->second.data);
		_packet << PACKET_REQUEST_HORDE_DESACTIVATE;

		// Send packet to server
		if (_networkEngine->getSocket().send(_packet, _networkEngine->getServerIp(), _networkEngine->getServerPort()) != sf::Socket::Done)
			VC_WARNING_CRITICAL("Error when sending horde desactivate to " + _networkEngine->getServerIp() + ":" + std::to_string(_networkEngine->getServerPort()));
	}
}

void	Sender::sendPlayerKick(void)
{
	if (Event::getEventByType(ev_PLAYER_KICK) == NULL)
		return;

	_packet.clear();
	std::list<std::pair<eventType, s_event> >::const_iterator it =
		Event::getEventByType(ev_PLAYER_KICK)->begin();
	sf::Uint32 id = (sf::Uint32)(it->second.data);
	_packet << PACKET_REQUEST_PLAYER_KICK << id;

	// Send packet to server
	if (_networkEngine->getSocket().send(_packet, _networkEngine->getServerIp(), _networkEngine->getServerPort()) != sf::Socket::Done)
		VC_WARNING_CRITICAL("Error when sending player kick to " + _networkEngine->getServerIp() + ":" + std::to_string(_networkEngine->getServerPort()));
}


void	Sender::sendInput(void)
{
	if (Event::getEventByType(ev_PLAYER_ACTION) == NULL)
		return;

	_packet.clear();
	float timestamp;
	timestamp = S_Map->getClock().getElapsedTime().asMilliseconds();
	_packet << PACKET_INPUT << timestamp;

	// Get player input
	std::list<std::pair<eventType, s_event> >::const_iterator it =
		Event::getEventByType(ev_PLAYER_ACTION)->begin();
	_playerActions = (s_actions*)(it->second.data);
	_packet << _playerActions;

	// Send packet
	if (_networkEngine->getSocket().send(_packet, _networkEngine->getServerIp(), _networkEngine->getServerPort()) != sf::Socket::Done)
		VC_WARNING_CRITICAL("Error when sending packet to " + _networkEngine->getServerIp() + ":" + std::to_string(_networkEngine->getServerPort()));
}

void	Sender::sendMessage()
{
	auto events = Event::getEventByType(ev_CHAT_TO_SEND);
	_packet.clear();
	if (events != NULL)
	{
		std::string *str = (std::string *)events->begin()->second.data;
		if (str != NULL && str->size() > 0)
		{
			_packet << PACKET_EVENT_CHAT << *str;
			if (_networkEngine->getSocket().send(_packet, _networkEngine->getServerIp(), _networkEngine->getServerPort()) != sf::Socket::Done)
				_networkEngine->printLog(1, "Unable to send chat message");
			delete str;
		}
	}
}

void	Sender::sendPong(float timestamp)
{
	_packet.clear();
	_packet << PACKET_PING << timestamp;
	if (_networkEngine->getSocket().send(_packet, _networkEngine->getServerIp(), _networkEngine->getServerPort()) != sf::Socket::Done)
		_networkEngine->printLog(1, "Unable to send pong");
}


// Inform server that the received packet has been received
void	Sender::sendConfirmation(ePacketType receivedPacket)
{
	if (receivedPacket == _packetSequence && _packetClock.getElapsedTime() < sf::milliseconds(1000))
		return;

	_packetSequence = receivedPacket;
	_packetClock.restart();

	_packet.clear();
	_packet << PACKET_CONFIRMATION << receivedPacket;
	_networkEngine->printLogWithId(2, "Sending confirmation packet to server for packet type ", receivedPacket);
	if (_networkEngine->getSocket().send(_packet, _networkEngine->getServerIp(), _networkEngine->getServerPort()) != sf::Socket::Done)
		_networkEngine->printLog(1, "Unable to send confirmation");
}

void	Sender::checkReliableUDP()
{
	if (_packetSequence != PACKET_NONE && _packetClock.getElapsedTime() > sf::milliseconds(1000))
	{
		//_packetClock.restart();
		//std::cout << "Reliability packet " << _packetSequence << " sent." << std::endl;
		if (_packetSequence == PACKET_EVENT_IDENT)
			sendConfirmation(PACKET_EVENT_IDENT);
		if (_packetSequence == PACKET_CONFIG)
			sendConfirmation(PACKET_CONFIG);
	}
}

void	Sender::sendSwitchTeam()
{
	auto events = Event::getEventByType(ev_JOIN_TEAM);
	if (events == NULL)
		return;

	_packet.clear();
	sf::Uint16 team = (sf::Uint16)events->begin()->second.data;
	_packet << PACKET_EVENT_JOIN_TEAM << team;
	if (_networkEngine->getSocket().send(_packet, _networkEngine->getServerIp(), _networkEngine->getServerPort()) != sf::Socket::Done)
		_networkEngine->printLog(1, "Unable to send team swap");
}

void	Sender::sendChangeNick()
{
	auto events = Event::getEventByType(ev_NICKNAME_CHANGED);
	if (events == NULL)
		return;

	_packet.clear();
	std::string *str = (std::string *)events->begin()->second.data;
	_packet << PACKET_NICKNAME_CHANGE << *str;
	if (_networkEngine->getSocket().send(_packet, _networkEngine->getServerIp(), _networkEngine->getServerPort()) != sf::Socket::Done)
		_networkEngine->printLog(1, "Unable to send team swap");
	delete str;
}

void	Sender::sendQuit()
{
	if (Event::getEventByType(ev_QUIT) == NULL)
		return;

	_packet.clear();
	_packet << PACKET_EVENT_QUIT;
	_networkEngine->printLog(2, "Sending quit packet to server");
	if (_networkEngine->getSocket().send(_packet, _networkEngine->getServerIp(), _networkEngine->getServerPort()) != sf::Socket::Done)
		_networkEngine->printLog(1, "Unable to send quit packet");
}

void	Sender::finishReliableUdpSequence()
{
	_packetSequence = PACKET_NONE;
}

void	Sender::sendEvents()
{
	// Only weapon selection atm
	auto it = Event::getEventByType(ev_WEAPON_SELECTION);
	if (it == NULL)
		return;

	_packet.clear();
	//float timestamp;
	//timestamp = S_Map->getClock().getElapsedTime().asMilliseconds();
	_packet << PACKET_WEAPON_SELECTION;
	
	// Get player input
	std::pair<t_weapon*, t_weapon*> *w =
		static_cast<std::pair<t_weapon*, t_weapon*>*>(it->begin()->second.data);
	t_weapon	*primary = w->first;
	t_weapon	*primaryAlt = NULL;
	t_weapon	*secondary = w->second;
	t_weapon	*secondaryAlt = NULL;

	sf::Int16 index = -1;
	if (primary)
		index = primary->getWeaponIndex();
	_packet << index;

	index = -1;
	if (primaryAlt)
		index = primaryAlt->getWeaponIndex();
	_packet << index;

	index = -1;
	if (secondary)
		index = secondary->getWeaponIndex();
	_packet << index;

	index = -1;
	if (secondaryAlt)
		index = secondaryAlt->getWeaponIndex();
	_packet << index;

	if (_networkEngine->getSocket().send(_packet, _networkEngine->getServerIp(), _networkEngine->getServerPort()) != sf::Socket::Done)
		_networkEngine->printLog(1, "Unable to send weapon selection");
}

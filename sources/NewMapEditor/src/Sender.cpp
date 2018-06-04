//
// Sender.cpp for Void Clash in /home/gegon/pfa/client/NetworkEngine/src
// 
// Made by Gegon
// Login   <gegon@epitech.net>
// 
// Started on  Sat Oct  5 00:03:30 2013 Gegon
// Last update Wed Feb 12 06:04:19 2014 sadin_n
//

#include	<cstring>
#include	"Sender.hpp"
#include	"NetworkEngine.hpp"
#include	"Manager.hpp"

#include        "Bullet.hpp"
#include        "Bomb.hpp"
#include        "Explosion.hpp"
#include        "GravityField.hpp"
#include        "SpeedField.hpp"
#include	"main.hpp"
#include	"Map.hpp"
#include	"HudRessources.hpp"

///////////////////////////////////////////////
/////   Overload of operator <<
/////	to create a packet with player's inputs

sf::Packet&	operator <<(sf::Packet &packet, s_actions *a)
{
	return packet << a->moveX << a->moveY << a->aimX << a->aimY << a->primary << a->secondary << a->switchPrimary << a->switchSecondary;
}

Sender::Sender(NetworkEngine *networkEngine) :
_networkEngine(networkEngine)
{
	_playerActions = NULL;
}

Sender::~Sender(void)
{
}

void	Sender::sendConfig(void)
{
	if (Event::getEventByType(ev_CONFIG) == NULL)
		return;

	// Create packet
	ConfigParser	*parse = new ConfigParser;

	_packet.clear();
	if (!parse->loadFile("config.json"))
		return;

	std::string toSend = parse->getString();
	_packet << PACKET_CONFIG << toSend;

	// Send packet to server
	if (_networkEngine->getSocket().send(_packet, _networkEngine->getServerIp(), _networkEngine->getServerPort()) != sf::Socket::Done)
		std::cout << RED << S_Map->getClock().getElapsedTime().asMilliseconds() << " - Error when sending packet config to " << _networkEngine->getServerIp() << ":" << _networkEngine->getServerPort() << RESET << std::endl;
}


void	Sender::sendMapChange(void)
{
	if (Event::getEventByType(ev_CHANGE_MAP) == NULL)
		return;

	_packet.clear();
	std::list<std::pair<eventType, s_event> >::const_iterator it =
		Event::getEventByType(ev_CHANGE_MAP)->begin();
	const char *raw = (const char *)(it->second.data);
	std::string	filename = raw;

	_packet << PACKET_REQUEST_CHANGE_MAP << filename;

	// Send packet to server
	if (_networkEngine->getSocket().send(_packet, _networkEngine->getServerIp(), _networkEngine->getServerPort()) != sf::Socket::Done)
		std::cout << RED << S_Map->getClock().getElapsedTime().asMilliseconds() << " - Error when sending packet map change to " << _networkEngine->getServerIp() << ":" << _networkEngine->getServerPort() << RESET << std::endl;
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
		std::cout << RED << S_Map->getClock().getElapsedTime().asMilliseconds() << " - Error when sending packet mode change to " << _networkEngine->getServerIp() << ":" << _networkEngine->getServerPort() << RESET << std::endl;
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
			std::cout << RED << S_Map->getClock().getElapsedTime().asMilliseconds() << " - Error when sending horde activate to " << _networkEngine->getServerIp() << ":" << _networkEngine->getServerPort() << RESET << std::endl;
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
			std::cout << RED << S_Map->getClock().getElapsedTime().asMilliseconds() << " - Error when sending horde desactivate to " << _networkEngine->getServerIp() << ":" << _networkEngine->getServerPort() << RESET << std::endl;
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
		std::cout << RED << S_Map->getClock().getElapsedTime().asMilliseconds() << " - Error when sending player kick to " << _networkEngine->getServerIp() << ":" << _networkEngine->getServerPort() << RESET << std::endl;
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
		std::cout << RED << S_Map->getClock().getElapsedTime().asMilliseconds() << " - Error when sending packet to " << _networkEngine->getServerIp() << ":" << _networkEngine->getServerPort() << RESET << std::endl;
}

void	Sender::sendMessage()
{
	_packet.clear();
	if (Event::getEventByType(ev_CHAT_TO_SEND) != NULL)
	{
		sf::String	toSend = HudRessources::getInstance()->getChatString();
		if (toSend.getSize() > 0)
		{
			_packet << PACKET_EVENT_CHAT << toSend;
			if (_networkEngine->getSocket().send(_packet, _networkEngine->getServerIp(), _networkEngine->getServerPort()) != sf::Socket::Done)
				_networkEngine->printLog(1, "Unable to send chat message");
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
	_packet.clear();
	_packet << PACKET_CONFIRMATION << receivedPacket;
	if (_networkEngine->getSocket().send(_packet, _networkEngine->getServerIp(), _networkEngine->getServerPort()) != sf::Socket::Done)
		_networkEngine->printLog(1, "Unable to send confirmation");
}

///////////////////////////////////////////////
/////   Send player input to server

bool	Sender::update()
{
	_packet.clear();

	Player	*curPlayer = S_Map->getCurrentPlayer();
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

	// IN GAME - swap team
	if (curPlayer->getTeam())
	{
		if (HudRessources::getInstance()->getTeamSwap())
		{
			_packet.clear();
			curPlayer->setTeam(0);
			_packet << PACKET_EVENT_JOIN_TEAM << 0;
			if (_networkEngine->getSocket().send(_packet, _networkEngine->getServerIp(), _networkEngine->getServerPort()) != sf::Socket::Done)
				_networkEngine->printLog(1, "Unable to send team swap");
		}
		_packet.clear();
	}
	else // SPECTATOR - join team
	{
		_packet.clear();
		sf::Uint16 team = HudRessources::getInstance()->getTeamJoined();
		_packet << PACKET_EVENT_JOIN_TEAM << team;
		if (team != 0)
		{
			if (_networkEngine->getSocket().send(_packet, _networkEngine->getServerIp(), _networkEngine->getServerPort()) != sf::Socket::Done)
				_networkEngine->printLog(1, "Unable to send join team");
		}
	}
	return (true);
}

void	Sender::sendEvents()
{
	// Only weapon selection atm
	if (Event::getEventByType(ev_WEAPON_SELECTION) == NULL)
		return;

	_packet.clear();
	//float timestamp;
	//timestamp = S_Map->getClock().getElapsedTime().asMilliseconds();
	_packet << PACKET_WEAPON_SELECTION;

	// Get player input
	std::list<std::pair<eventType, s_event> >::const_iterator it =
		Event::getEventByType(ev_PLAYER_ACTION)->begin();
	Player *player = dynamic_cast<Player *>((*it).second.trigger);
	t_weapon	*primary = player->getWeapons(true, false);
	t_weapon	*primaryAlt = player->getWeapons(true, true);
	t_weapon	*secondary = player->getWeapons(false, false);
	t_weapon	*secondaryAlt = player->getWeapons(false, true);

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

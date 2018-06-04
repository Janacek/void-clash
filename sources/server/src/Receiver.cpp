//
// Receiver.cpp for Void Clash in /home/gegon/pfa/server/NetworkEngine/inc
// 
// Made by Gegon
// Login   <gegon@epitech.net>
// 
// Started on  Wed Oct  9 16:49:39 2013 Gegon
// Last update Fri Dec 27 19:35:49 2013 Gegon
//

#include <fstream>
#include <string>
#include <iostream>
#include	"main.hpp"
#include	"Manager.hpp"
#include	"NetworkEngine.hpp"
#include	"ClientHandle.hpp"
#include	"Receiver.hpp"
#include	"Command.hpp"
#include	"Bullet.hpp"
#include	"Bomb.hpp"
#include	"Explosion.hpp"
#include	"GravityField.hpp"
#include	"SpeedField.hpp"
#include	"Command.hpp"

extern t_config	*G_conf;

Receiver::Receiver(NetworkEngine *networkEngine) :
_networkEngine(networkEngine)
{
	_command = new Command(_networkEngine);
}

Receiver::~Receiver()
{
	delete _command;
}

///////////////////////////////////////////////
/////   Update

bool	Receiver::update()
{
	sf::IpAddress		ip;
	unsigned short	port;
	sf::Uint32		type;

	_packet.clear();
	while (_networkEngine->getSocket()->receive(_packet, ip, port) == sf::Socket::Done)
	{
		updateClientActivity(ip, port);
		ClientHandle *client = _networkEngine->findClientHandleWithIP(ip, port);
		_packet >> type;
		if (type == PACKET_INPUT)
			updateClientInput(ip, port);
		else if (type == PACKET_CONFIRMATION)
		{
			_packet >> type;
			if (client)
			{
				if (type == PACKET_EVENT_IDENT)
				{
					_networkEngine->printLogWithId(2, "Confirmation packet of PACKET_EVENT_IDENT received from ", client->getPlayer()->getId());
					_networkEngine->getSender()->sendConfig(client);
					client->setStarted(true);
				}
				if (type == PACKET_CONFIG)
				{
					_networkEngine->printLogWithId(2, "Confirmation packet of PACKET_CONFIG received from ", client->getPlayer()->getId());
					_networkEngine->getSender()->sendPacketGeneric(PACKET_WELCOME, client);
					_networkEngine->getSender()->sendPacketEvent(PACKET_EVENT_SWITCH_MAP_MODE, S_Map->getMode()->getModeEnum(), client);
					_networkEngine->getSender()->sendMapList(client);
				}
			}
			else
				_networkEngine->printLog(1, "Confirmation packet received but unable to find player");
		}
		else if (type == PACKET_PING)
			client->pong(_packet);
		else if (type == PACKET_EVENT_CHAT)
			handleChat(ip, port);
		else if (type == PACKET_EVENT_JOIN_TEAM)
			playerJoinTeam(ip, port);
		else if (type == PACKET_WEAPON_SELECTION)
			handleWeaponSelection(ip, port);
		else if (type == PACKET_EVENT_QUIT)
			handleQuit(ip, port);
		else if (type == PACKET_NICKNAME_CHANGE)
			handleChangeNick(ip, port);
		else if (type == PACKET_REQUEST_CHANGE_MAP)
			handleRequestSwitchMap(ip, port);
		else if (type == PACKET_REQUEST_CHANGE_MODE)
			handleRequestSwitchMode(ip, port);
		else if (type == PACKET_REQUEST_PLAYER_KICK)
			handleRequestPlayerKick(ip, port);
		else if (type == PACKET_REQUEST_HORDE_ACTIVATE)
		{
			if (!client->isAdmin())
				_networkEngine->getSender()->sendStringToDisplay("Unauthorized access to activate horde. Use /auth password", 0, client);
			else
				S_Map->getMode()->activeHorde();
		}
		else if (type == PACKET_REQUEST_HORDE_DESACTIVATE)
		{
			if (!client->isAdmin())
				_networkEngine->getSender()->sendStringToDisplay("Unauthorized access to desactivate horde. Use /auth password", 0, client);
			else
				S_Map->getMode()->desactiveHorde();
		}
		else
		{
			_networkEngine->printLog(1, "Packet integrity check failed. Clearing it...");
			_packet.clear();
		}
		_packet.clear();
	}
	checkClientsActivity();
	return (true);
}

///////////////////////////////////////////////
/////   Receive inputs from clients
/////	Create new client if IP is not registered

void	Receiver::updateClientInput(sf::IpAddress ip, unsigned short port)
{
	ClientHandle *client;
	client = _networkEngine->findClientHandleWithIP(ip, port);
	if (client == NULL)
		return;
	client->setActions(_packet);
}

void	Receiver::handleChat(sf::IpAddress ip, unsigned short port)
{
	std::string	msg;

	_packet >> msg;
	_packet.clear();
	if (_command->chatHandle(msg, ip, port)) // Don't send commands to clients
		return;
	_packet << PACKET_EVENT_CHAT;
	_packet << _networkEngine->findPlayerWithIP(ip, port)->getId();
	_packet << msg;
	_networkEngine->getSender()->sendPacket(_packet);
	_networkEngine->printLogWithId(2, "Message received : \"" + msg + "\" from player ",
		_networkEngine->findPlayerWithIP(ip, port)->getId());
}

void	Receiver::playerJoinTeam(sf::IpAddress ip, unsigned short port)
{
	Player *player;
	player = _networkEngine->findPlayerWithIP(ip, port).get();
	if (player == NULL)
		return;

	sf::Uint16	team;
	_packet >> team;
	player->setTeam(team);
	player->startRespawnSequence();

	// log
	if (team == 0 && player->getTeam() > 0)
		_networkEngine->printLogWithId(2, "Reset team detected for player ", player->getId());
	// Assign team
	else if (player->getTeam() == 0 && team > 0)
		_networkEngine->printLogWithId(2, "Join team detected for player ", player->getId());
}

void	Receiver::handleWeaponSelection(sf::IpAddress ip, unsigned short port)
{
	std::shared_ptr<Player>player;
	player = _networkEngine->findPlayerWithIP(ip, port);
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

	_networkEngine->printLogWithId(2, "Weapon selection packet received for player ", player->getId());

	player->setWeapons(primary, primaryAlt, secondary, secondaryAlt);
	_networkEngine->getSender()->sendWeaponSelection();
}

void	Receiver::handleQuit(sf::IpAddress ip, unsigned short port)
{
	ClientHandle *client = _networkEngine->findClientHandleWithIP(ip, port);
	if (client)
	{
		std::list<ClientHandle *>::iterator	it = _networkEngine->getClients().begin();
		std::list<ClientHandle *>::iterator	end = _networkEngine->getClients().end();

		// Check activity
		while (it != end)
		{
			if ((*it) == client)
			{
				std::string strToSend = (*it)->getPlayer()->getNick() + " has left the game";
				_networkEngine->getSender()->sendStringToDisplay(strToSend, 2);
				_networkEngine->printLogWithId(2, "Client is flagged as disconnect. Id of player is ", (*it)->getPlayer()->getId(), YELLOW);
				ADD_EVENT(ev_DELETE, std::dynamic_pointer_cast<AObject>((*it)->getPlayer()));

				it = _networkEngine->getClients().erase(it);
				return;
			}
			++it;
		}
	}
}


void	Receiver::handleChangeNick(sf::IpAddress ip, unsigned short port)
{
	std::shared_ptr<Player>player = _networkEngine->findPlayerWithIP(ip, port);
	std::string nick;
	if (!(_packet >> nick))
	{
		_networkEngine->printLog(1, "Unable to extract nickname from PACKET_NICKNAME_CHANGE");
		_packet.clear();
		return;
	}

	if (player == NULL || nick.empty())
	{
		_networkEngine->printLog(1, "Unable to change nick of " + ip.toString());
		return;
	}

	std::string strToSend = player->getNick() + " changed his name to " + nick;
	player->setNick(nick);
	_networkEngine->printLog(2, strToSend, DARK_BLUE);
	_networkEngine->getSender()->sendStringToDisplay(strToSend, 2);
}



void	Receiver::handleRequestSwitchMap(sf::IpAddress ip, unsigned short port)
{
	ClientHandle *client = _networkEngine->findClientHandleWithIP(ip, port);
	if (!client->isAdmin())
	{
		_networkEngine->getSender()->sendStringToDisplay("Unauthorized access to switch map. Use /auth password", 0, client);
		return;
	}

	std::string	result;
	if (!(_packet >> result))
	{
		_networkEngine->printLog(1, "Unable to extract REQUEST MAP SWITCH packet");
		_packet.clear();
		return;
	}

	S_Map->changeMap(result.c_str());
}

void	Receiver::handleRequestSwitchMode(sf::IpAddress ip, unsigned short port)
{
	ClientHandle *client = _networkEngine->findClientHandleWithIP(ip, port);
	if (!client->isAdmin())
	{
		_networkEngine->getSender()->sendStringToDisplay("Unauthorized access to switch mode. Use /auth password", 0, client);
		return;
	}

	sf::Int16	raw;

	if (!(_packet >> raw))
	{
		_networkEngine->printLog(1, "Unable to extract REQUEST MODE SWITCH packet");
		_packet.clear();
		return;
	}
	eMapMode	result = (eMapMode)raw;
	S_Map->getMode()->changeMapMode(result);
}


void	Receiver::handleRequestPlayerKick(sf::IpAddress ip, unsigned short port)
{
	ClientHandle *client = _networkEngine->findClientHandleWithIP(ip, port);
	if (!client->isAdmin())
	{
		_networkEngine->getSender()->sendStringToDisplay("Unauthorized access to kick a player. Use /auth password", 0, client);
		return;
	}

	sf::Uint32	id;

	if (!(_packet >> id))
	{
		_networkEngine->printLog(1, "Unable to extract REQUEST PLAYER KICK packet");
		_packet.clear();
		return;
	}


	ClientHandle *toKick = _networkEngine->findClientHandleWithID(id);
	if (toKick == NULL)
	{
		_networkEngine->printLogWithId(1, "Unable to kick player with id ", id);
		return;
	}
	_networkEngine->getSender()->sendPacketEvent(PACKET_EVENT_KICK_PLAYER, id, toKick);
	_networkEngine->printLogWithId(2, "Kick player ", id, DARK_BLUE);
}


///////////////////////////////////////////////
/////   Check activity of clients and send disco if not
/////	Update player activity or add them if new client

void	Receiver::updateClientActivity(sf::IpAddress ip, unsigned short port)
{
	ClientHandle *client;
	client = _networkEngine->findClientHandleWithIP(ip, port);
	if (client == NULL)
	{
		if (_networkEngine->getClients().size() + 1 > (unsigned int)G_conf->server->max_player)
		{
			_networkEngine->getSender()->sendPacketEvent(PACKET_EVENT_SERVER_FULL, 0, ip, port);
			return;
		}
		_networkEngine->addNewClient(ip, port);
		return;
	}
	// Refresh clock of client handler
	client->active(S_Map->getGlobalClock().getElapsedTime());
}

void	Receiver::checkClientsActivity()
{
	std::list<ClientHandle *>::iterator	it = _networkEngine->getClients().begin();
	std::list<ClientHandle *>::iterator	end = _networkEngine->getClients().end();

	// Check activity
	while (it != end)
	{
		if (!(*it)->isActive(S_Map->getGlobalClock().getElapsedTime()))
		{
			std::string strToSend = (*it)->getPlayer()->getNick() + " has been disconnected";
			_networkEngine->getSender()->sendStringToDisplay(strToSend, 2);
			_networkEngine->printLogWithId(2, "Client is flagged as disconnect. Id of player is ", (*it)->getPlayer()->getId(), YELLOW);
			ADD_EVENT(ev_DELETE, std::dynamic_pointer_cast<AObject>((*it)->getPlayer()));

			delete *it;
			it = _networkEngine->getClients().erase(it);
			
			return;
		}
		++it;
	}
}

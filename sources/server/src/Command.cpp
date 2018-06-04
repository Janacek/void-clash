//
// Command.cpp for pfa in /home/volent/dev/newpfa
// 
// Made by florent espanet
// Login   <espane_f@epitech.net>
// 
// Started on  Fri Nov 22 13:06:52 2013 florent espanet
// Last update Mon Jan 20 00:20:59 2014 Gegon
//

#include <ConfigParser.hpp>
#include "Command.hpp"
#include "Event.hpp"
#include "main.hpp"
#include	"ClientHandle.hpp"

extern t_config	*G_conf;

Command::Command(NetworkEngine *networkEngine) : _networkEngine(networkEngine) {}
Command::~Command() {}

bool Command::chatHandle(const std::string &msg, sf::IpAddress ip, unsigned short port)
{
	if (msg[0] != '/')
		return (false);
	_networkEngine->printLog(2, "Received command : \"" + msg + "\" from " + ip.toString(), LIGHT_BLUE);
	//if (!msg.compare("/next_map"))
	//	nextMap();
	//else if (!msg.compare("/prev_map"))
	//	prevMap();
	//else if (!msg.compare("/config"))
	//	reloadConfig();
	//else if (!msg.compare("/dm"))
	//	switchMode(mode_FFA);
	//else if (!msg.compare("/tdm"))
	//	switchMode(mode_TEAM_DM);
	//else if (!msg.compare("/ctf"))
	//	switchMode(mode_CTF);
	//else if (!msg.compare("/surv"))
	//	switchMode(mode_SURVIVOR);
	//else if (!msg.compare("/tsurv"))
	//	switchMode(mode_TEAM_SURVIVOR);
	//else if (!msg.compare("/help"))
	//	help();
	//else if (!msg.compare("/cheat"))
	//	_networkEngine->findPlayerWithIP(ip, port)->plusKills(4);
	if (!msg.compare(0, 6, "/auth "))
	{
		char password[11] = { 0 };
		msg.copy(password, 10, 6);
		authRequest(password, ip, port);
	}
	else if (!msg.compare(0, 6, "/nick "))
	{
		char nick[11] = { 0 };
		msg.copy(nick, 10, 6);
		changeNick(nick, ip, port);
	}
	//else if (!msg.compare(0, 6, "/kick "))
	//{
	//	char id[11] = { 0 };
	//	msg.copy(id, 10, 6);
	//	kick((sf::Uint32)atoi(id));
	//}
	//else if (!msg.compare(0, 6, "/slap "))
	//{
	//	char id[11] = { 0 };
	//	msg.copy(id, 10, 6);
	//	slap((sf::Uint32)atoi(id));
	//}
	else
		unknown(ip, port);
	return (true);
}

///////////////////////////////////////////////
/////   Specific commands related functions

void Command::nextMap()
{
	S_Map->nextMap();
}

void Command::prevMap()
{
	S_Map->prevMap();
}

void Command::reloadConfig()
{
	S_Map->loadConfig("config.json");
	ADD_EVENT(ev_CONFIG, s_event(NULL));
}

void Command::switchMode(enum eMapMode mode)
{
	S_Map->changeMode(mode);
	ADD_EVENT(ev_SWITCH_MAP_MODE, s_event(NULL));
}

void Command::authRequest(const std::string &password, sf::IpAddress ip, unsigned short port)
{
	ClientHandle *client = _networkEngine->findClientHandleWithIP(ip, port);
	if (client == NULL)
	{
		_networkEngine->printLog(1, "Unable to find registered client with IP " + ip.toString());
		return;
	}
	if (password.compare(G_conf->server->password))
	{
		_networkEngine->printLogWithId(1, "Authentification FAILED for player ", client->getPlayer()->getId());
		_networkEngine->getSender()->sendStringToDisplay("Authentification failed. Wrong password.", 0, client);
		return;
	}
	_networkEngine->printLogWithId(2, "Authentification SUCCESS for player ", client->getPlayer()->getId());
	_networkEngine->getSender()->sendStringToDisplay("Authentification successful. You are now server administrator.", 1, client);
	client->admin();
}

void Command::changeNick(const std::string &nick, sf::IpAddress ip, unsigned short port)
{
	Player *player = _networkEngine->findPlayerWithIP(ip, port).get();
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

void Command::kick(sf::Uint32 id)
{
	ClientHandle *client = _networkEngine->findClientHandleWithID(id);
	if (client == NULL)
	{
		_networkEngine->printLogWithId(1, "Unable to kick player with id ", id);
		return;
	}
	_networkEngine->getSender()->sendPacketEvent(PACKET_EVENT_KICK_PLAYER, id, client);
	_networkEngine->printLogWithId(2, "Kick player ", id, DARK_BLUE);
}

void Command::slap(sf::Uint32 id)
{
	Player *player = dynamic_cast<Player *>(_networkEngine->findObjectWithID(id).get());
	if (player == NULL)
	{
		_networkEngine->printLogWithId(1, "Unable to slap player with id ", id);
		return;
	}
	player->addForce(-5000 + (std::rand() % 10000), -5000 + (std::rand() % 10000));
}

void Command::help()
{
	_networkEngine->printLog(2, "TODO HELP");
}

void Command::unknown(sf::IpAddress ip, unsigned short port)
{
	ClientHandle *client = _networkEngine->findClientHandleWithIP(ip, port);
	if (!client)
		return;
	_networkEngine->printLog(1, "Unknown command");
	_networkEngine->getSender()->sendStringToDisplay("Unknown command", 0, client);
}

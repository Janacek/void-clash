//
// Receiver.cpp for Void Clash in /home/gegon/pfa/client/NetworkEngine/src
// 
// Made by Gegon
// Login   <gegon@epitech.net>
// 
// Started on  Fri Oct  4 23:01:56 2013 Gegon
// Last update Fri Jan 31 17:29:48 2014 Gegon
//


#include	"NetworkEngine.hpp"
#include	"Receiver.hpp"
#include	"Manager.hpp"

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
#include "Sender.hpp"

#include	"Map.hpp"

extern t_config *G_conf;


Receiver::Receiver(NetworkEngine *networkEngine) :
_networkEngine(networkEngine)
{
	_lastUpdate = 0.0f;
	_averageLatency = 0.0f;
	_lastConnectivity = S_Map->getClock().getElapsedTime();
	_welcomeReceived = false;

}

Receiver::~Receiver()
{}

///////////////////////////////////////////////
/////   Received messages from server
/////	And extract packet type

bool	Receiver::update()
{
	// ip and port not used
	sf::IpAddress		ip;
	unsigned short	port;
	sf::Uint32		packetType;

	_packet.clear();
	packetType = PACKET_NONE;
	while (_networkEngine->getSocket().receive(_packet, ip, port) == sf::Socket::Done)
	{
		_lastConnectivity = S_Map->getClock().getElapsedTime();
		if (_packet >> packetType)
		{
			if (extractPacketFromType(packetType) == false)
				return true;
		}
		else
			_networkEngine->printLog(1, "Unable to extract packet");
	}
	return createServerConnectivityEvent();
}

bool	Receiver::createServerConnectivityEvent()
{
	_msSinceLastConnectivity = S_Map->getClock().getElapsedTime().asMilliseconds() - _lastConnectivity.asMilliseconds();
	ADD_EVENT(ev_SERVER_CONNECTIVITY, s_event(NULL, (&_msSinceLastConnectivity)));
	if (_msSinceLastConnectivity > INACTIVITY_TIMEOUT * 1000)
		return false;
	return true;
}

///////////////////////////////////////////////
/////   Execute or call function according
/////	to packet type

/////	return false to stop receive on this frame (used for welcome packet)

bool	Receiver::extractPacketFromType(sf::Uint32 packetType)
{
	_packetType = packetType;
	if (packetType == PACKET_WELCOME)
	{
		handleWelcome();
		_networkEngine->getSender()->finishReliableUdpSequence();
		return false;
	}
	else if (packetType == PACKET_UPDATE)
	{
		handleUpdate();
	}
	else if (packetType == PACKET_VERSION_CHECK)
		handleVersionCheck();
	else if (packetType == PACKET_SYNCHRO)
		handleSynchro();
	else if (packetType == PACKET_WEAPON_SELECTION)
		handleWeaponSelection();
	else if (packetType == PACKET_CONFIG)
		handleConfig();
	else if (packetType == PACKET_PING)
		handlePing();
	else if (packetType == PACKET_EVENT_LAUNCHED)
		handleLaunchEvent();
	else if (packetType == PACKET_EVENT_KILL)
		handleKillEvent();
	else if (packetType == PACKET_EVENT_PLAYER_HIT)
		handlePlayerHitEvent();
	else if (packetType == PACKET_EVENT_CHAT)
		handleChatEvent();
	else if (packetType == PACKET_EVENT_IDENT)
		handleIdentification();
	else if (packetType == PACKET_EVENT_SWITCH_MAP_MODE)
		handleSwitchMapMode();
	else if (packetType == PACKET_EVENT_CAPTURE_FLAG)
		handleCaptureFlag();
	else if (packetType == PACKET_EVENT_TURRET_DESTROYED)
		handleTurretDestroyed();
	else if (packetType == PACKET_EVENT_KICK_PLAYER)
		handleKick();
	else if (packetType == PACKET_MAP_LIST)
		handleMapList();
	else if (packetType == PACKET_EVENT_SERVER_FULL)
		handleServerFull();
	else if (packetType == PACKET_RESET_ROUND)
		handleResetRound();
	else if (packetType == PACKET_DISPLAY_STRING)
		handleDisplayString();
	else if (packetType == PACKET_GAME_START)
	{
		ADD_EVENT_SIMPLE(ev_GAME_START);
		S_Map->resetMap();
	}
	else
	{
		_networkEngine->printLog(1, "Packet integrity failed. Clearing packet...");
		_packet.clear();
	}
	return true;
}

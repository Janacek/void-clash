//
// Client.cpp for Void Clash in /home/gegon/pfa/server/NetworkEngine/src
// 
// Made by Gegon
// Login   <gegon@epitech.net>
// 
// Started on  Sat Sep 21 10:53:32013 Gegon
// Last update Fri Jan 31 00:08:31 2014 Gegon
//

#include	<cstring>
#include	"Player.hpp"
#include	"ClientHandle.hpp"
#include	"NetworkEngine.hpp"
#include	"Manager.hpp"
#include	"Map.hpp"

#include	"main.hpp"

ClientHandle::ClientHandle(NetworkEngine *networkEngine, sf::IpAddress ip, unsigned short port)
{
	_ip = ip;
	_port = port;

	_networkEngine = networkEngine;
	_player = std::make_shared<Player>(1500, 750, 0, 0, true);
	S_Map->addPlayer(_player, 0);
	_actions = new s_actions();
	_lastActivity = S_Map->getGlobalClock().getElapsedTime();
	_lastUpdateInput = 0.0f;
	_clock.restart();
	_admin = false;
	_started = false;
}

ClientHandle::~ClientHandle() {}

sf::Packet&	operator >>(sf::Packet &packet, s_actions *a)
{
//	return packet >> a->moveX >> a->moveY >> a->aimX >> a->aimY >> a->primary >> a->secondary >> a->switchPrimary >> a->switchSecondary;
	return packet >> a->moveX >> a->moveY >> a->aimX >> a->aimY >> a->primary >> a->secondary;
}

///////////////////////////////////////////////
/////   Check player latency funcs

bool	ClientHandle::ping()
{
	if (_clock.getElapsedTime().asSeconds() > 1.0f && _started)
	{
		// Add a packet synchro every sec
		_networkEngine->getSender()->sendPacketGeneric(PACKET_SYNCHRO, this);
		_networkEngine->getSender()->sendWeaponSelection();

		float timestamp;

		timestamp = S_Map->getClock().getElapsedTime().asMicroseconds();
		_networkEngine->getSender()->sendPing(timestamp, this);
		_clock.restart();
	}
	return (true);
}

void	ClientHandle::setStarted(bool started)
{
	_started = true;
}

void	ClientHandle::pong(sf::Packet &packet)
{
	float timestamp;
	if (packet >> timestamp)
	{
		_player->setLatency(S_Map->getClock().getElapsedTime().asMicroseconds() - timestamp);
	}
	else
		_networkEngine->printLog(1, "Unable to extract timestamp");
}


///////////////////////////////////////////////
/////   Setterz

void	ClientHandle::setPort(unsigned short port)
{
	_port = port;
}

void	ClientHandle::setActions(sf::Packet &packet)
{
	// Drop old packet to work only on most recent ones
	float timestamp;
	if (packet >> timestamp)
	{
		//if (timestamp < _lastUpdateInput)
		//{
		//	_networkEngine->printLog(2, "Older input packet received. Dropping it... Received from " + _ip.toString());
		//	packet.clear();
		//	return;
		//}
		_lastUpdateInput = timestamp;
	}
	else
	{
		_networkEngine->printLog(1, "Unable to extract timestamp of packet input");
	}


	// extract
	if (packet >> _actions)
	{
		ADD_EVENT(ev_PLAYER_ACTION, s_event(_player, _actions));
		//_networkEngine->printLog(3, "Update success from " + _ip.toString());
	}
	else
		_networkEngine->printLog(1, "Update packet extraction failed from " + _ip.toString());
}

void	ClientHandle::resetActions()
{
	memset(_actions, 0, sizeof(*_actions));
}

///////////////////////////////////////////////
/////   Getterz

const sf::IpAddress	&ClientHandle::getIp() const
{
	return (_ip);
}

unsigned short	ClientHandle::getPort() const
{
	return (_port);
}

std::shared_ptr<Player>	ClientHandle::getPlayer() const
{
	return (_player);
}

///////////////////////////////////////////////
/////   Activity

void	ClientHandle::active(sf::Time time)
{
	_lastActivity = time;
}

bool	ClientHandle::isActive(sf::Time time)
{
	if ((time - _lastActivity).asSeconds() > INACTIVITY_TIMEOUT)
		return false;
	return true;
}

bool	ClientHandle::isResponsive(sf::Time time)
{
	if ((time - _lastActivity).asSeconds() > RESPONSIVE_TIMEOUT)
		return false;
	return true;
}

///////////////////////////////////////////////
/////   Admin

void	ClientHandle::admin()
{
	_admin = true;
}

bool	ClientHandle::isAdmin()
{
	return _admin;
}
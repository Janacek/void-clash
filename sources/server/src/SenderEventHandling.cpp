//
// SenderEventHandling.cpp for Void Clash in /home/gegon/pfa/sources/server/Common/src
// 
// Made by Gegon
// Login   <gegon@epitech.net>
// 
// Started on  Mon Dec 16 00:20:38 2013 Gegon
// Last update Thu Feb 27 16:31:39 2014 Gegon
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
#include	"AssetPath.h"

extern t_config *G_conf;
extern std::string G_configPath;

// Config
void	Sender::sendConfig(ClientHandle *client)
{
	//// Reset all weapons
	//std::list<Player *>::iterator	it = S_Map->getPlayers()->begin();
	//std::list<Player *>::iterator	end = S_Map->getPlayers()->end();
	//while (it != end)
	//{
	//	Player *player = dynamic_cast<Player *>(*it);
	//	if (player)
	//		player->setWeapons(NULL, NULL, NULL, NULL);
	//	++it;
	//}

	// Send config to clients
	ConfigParser	*parse = new ConfigParser;

	_packet.clear();
	//S_Map->loadConfig(G_configPath + "config.json");
	if (!parse->loadFile(G_configPath + "config.json"))
		return;

	_networkEngine->printLogWithId(2, "Sending config packet to ", client->getPlayer()->getId());
	std::string toSend = parse->getString();
	_packet << PACKET_CONFIG << toSend;
	sendPacketTo(client);
}

void	Sender::sendMapList(ClientHandle *client)
{
	_packet.clear();
	std::deque<std::string>&mapList(S_Map->getMapDatabase()->getMapList());

	_packet << PACKET_MAP_LIST;
	for (unsigned int i = 0; i < mapList.size(); ++i)
	{
		std::string toSend = mapList.at(i);
		_packet << toSend;
	}
	sendPacketTo(client);
}


// Send weapon
void	Sender::sendWeaponSelection(ClientHandle *client)
{
	_packet.clear();
	_packet << PACKET_WEAPON_SELECTION;

	auto	it = S_Map->getPlayers()->begin();
	auto	end = S_Map->getPlayers()->end();
	while (it != end)
	{
		Player *player = dynamic_cast<Player *>((*it).get());
		_packet << player->getId();

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
		++it;
	}

	sendPacketTo(client);
}

void	Sender::sendUpdateEvents()
{
	// SEND RELOAD CONFIG
	if (Event::getEventByType(ev_CONFIG) != NULL)
	{
		sendPacketGeneric(PACKET_WELCOME);
		sendConfig();
		sendPacketEvent(PACKET_EVENT_SWITCH_MAP_MODE, S_Map->getMode()->getModeEnum());
	}

	// SEND BOMB LAUNCH EVENT
	if (Event::getEventByType(ev_BOMB_LAUNCHED) != NULL)
	{
		std::list<std::pair<eventType, s_event> >::const_iterator it = Event::getEventByType(ev_BOMB_LAUNCHED)->begin();
		std::list<std::pair<eventType, s_event> >::const_iterator end = Event::getEventByType(ev_BOMB_LAUNCHED)->end();

		while (it != end)
		{
			AObject *obj = (*it).second.trigger.get();
			sendPacketEvent(PACKET_EVENT_LAUNCHED, obj->getId());
			++it;
		}
	}


	// SEND SWITCH MODE EVENT
	if (Event::getEventByType(ev_SWITCH_MAP_MODE) != NULL)
	{
		std::string toSend;
		auto changeRequest = Event::getEventByType(ev_SWITCH_MAP_MODE)->front();
		sf::Uint32 mode = S_Map->getMode()->getModeEnum();
		sendPacketEvent(PACKET_EVENT_SWITCH_MAP_MODE, mode);

		toSend = "Switch map mode requested " + std::to_string(mode);
		_networkEngine->getSender()->sendStringToDisplay(toSend, 0);
		S_Map->resetScore();
		auto      it = S_Map->getPlayers()->begin();
		auto      end = S_Map->getPlayers()->end();

		while (it != end)
		{
			//(*it)->setTeam(0);
			//(*it)->respawn();
			(*it)->reset();
			++it;
		}
		sendPacketGeneric(PACKET_WELCOME);
	}

	// SEND KILL EVENT - Trigger = hitter, data = killed
	if (Event::getEventByType(ev_KILL) != NULL)
	{
		std::list<std::pair<eventType, s_event> >::const_iterator it = Event::getEventByType(ev_KILL)->begin();
		std::list<std::pair<eventType, s_event> >::const_iterator end = Event::getEventByType(ev_KILL)->end();

		while (it != end)
		{
			AObject *hitter = (Player *)((*it).second.trigger.get());
			void *tmp = (*it).second.data;

			Player *killed = (Player *)(tmp);
			Player *killer = NULL;
			if (hitter)
				killer = hitter->getOwner().get();

			if (killed)
				killed->plusDeaths();

			// inc score / death
			if (killer && killed)
			{
				killed->resetKillsStreak();
				if (killer->getId() != killed->getId())
				{
					killer->isHealedFor(G_conf->player->life / 2.5f);
					killer->plusKills();
					killer->plusKillsStreak();
				}

				// inc score for team DM
				if (S_Map->getMode()->getProperty()->killIncTeamScore)
				{
					if (killer->getTeam() == 1)
						S_Map->incScore(1, 0);
					if (killer->getTeam() == 2)
						S_Map->incScore(0, 1);
				}

				sf::Packet packet;
				eObjectType typeToSend = hitter->getType();
				if (hitter->getType() == BULLET)
				{
					Bullet *b = dynamic_cast<Bullet *>(hitter);
					if (b->getMakerType() != NONE)
						typeToSend = b->getMakerType();
				}
				if (hitter->getType() == EXPLOSION)
				{
					Explosion *e = dynamic_cast<Explosion *>(hitter);
					typeToSend = e->getMakerType();
				}
				packet << PACKET_EVENT_KILL << killer->getId() << killed->getId() << typeToSend;
				sendPacket(packet);
			}
			++it;
		}
	}

	// SEND HIT EVENT - Trigger = Object who killed (bullet...), data = killed
	if (Event::getEventByType(ev_PLAYER_HIT) != NULL)
	{
		std::list<std::pair<eventType, s_event> >::const_iterator it = Event::getEventByType(ev_PLAYER_HIT)->begin();
		std::list<std::pair<eventType, s_event> >::const_iterator end = Event::getEventByType(ev_PLAYER_HIT)->end();

		while (it != end)
		{
			AObject *hitter = (Player *)((*it).second.trigger.get());
			void *tmp = (*it).second.data;
			Player *hitted = (Player *)(tmp);

			if (hitter && hitted)
				sendPacketEventTwo(PACKET_EVENT_PLAYER_HIT, hitter->getId(), hitted->getId());
			++it;
		}
	}

	// SEND CAPTURE FLAG EVENT

	if (Event::getEventByType(ev_CAPTURE_FLAG) != NULL)
	{
		std::list<std::pair<eventType, s_event> >::const_iterator it = Event::getEventByType(ev_CAPTURE_FLAG)->begin();
		std::list<std::pair<eventType, s_event> >::const_iterator end = Event::getEventByType(ev_CAPTURE_FLAG)->end();
		while (it != end)
		{
			Flag *flag = (Flag *)(*it).second.trigger.get();
			void *tmp = (*it).second.data;
			Player *player = (Player *)(tmp);
			sendPacketEventTwo(PACKET_EVENT_CAPTURE_FLAG, flag->getId(), player->getId());
			++it;
		}
	}

	// SEND TURRET DESTROYED

	if (Event::getEventByType(ev_TURRET_DESTROYED) != NULL)
	{
		std::list<std::pair<eventType, s_event> >::const_iterator it = Event::getEventByType(ev_TURRET_DESTROYED)->begin();
		std::list<std::pair<eventType, s_event> >::const_iterator end = Event::getEventByType(ev_TURRET_DESTROYED)->end();
		while (it != end)
		{
			AObject *turret = (AObject *)(*it).second.trigger.get();
			sendPacketEvent(PACKET_EVENT_TURRET_DESTROYED, turret->getId());
			++it;
		}
	}

	// CHANGE MAP

	if (Event::getEventByType(ev_CHANGE_MAP) != NULL)
	{
		//sendPacketGeneric(PACKET_WELCOME);

		std::list<ClientHandle *>::iterator	it = _networkEngine->getClients().begin();
		std::list<ClientHandle *>::iterator	end = _networkEngine->getClients().end();
		while (it != end)
		{
			sendPacketEvent(PACKET_EVENT_IDENT, (*it)->getPlayer()->getId(), (*it));
			++it;
		}
	}

	if (Event::getEventByType(ev_START_ROUND) != NULL)
	{
		_packet.clear();
		_packet << PACKET_RESET_ROUND;
		sendPacketTo();
	}

	if (Event::getEventByType(ev_GAME_START) != NULL)
	{
		_packet.clear();
		_packet << PACKET_GAME_START;
		sendPacketTo();
	}
}

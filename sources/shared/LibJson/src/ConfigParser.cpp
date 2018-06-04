#include <iostream>
#include <fstream>
#include "json.hpp"
#include  "Defines.h"
#include "ConfigParser.hpp"

extern t_config *G_conf;
extern int volume;

ConfigParser::ConfigParser() {}
ConfigParser::~ConfigParser() {}

void *ConfigParser::parse()
{
	Json::Reader	reader;
	t_config	*conf;

	if (!reader.parse(_string.c_str(), _root, false))
	{
		std::cerr << "Syntax error in config.json" << std::endl;
		return (NULL);
	}


	const Json::Value player = _root["player"];
	const Json::Value weapons = _root["weapons"];
	const Json::Value game = _root["game"];
	const Json::Value respawn = _root["respawn"];
	const Json::Value flag = _root["flag"];
	const Json::Value wall = _root["wall"];
	const Json::Value speedfield = _root["speedfield"];
	const Json::Value horde = _root["horde"];
	const Json::Value server = _root["server"];

	conf = new t_config;
	conf->player = new t_player;
	conf->horde = new t_horde;
	conf->server = new t_server;
	conf->game = new t_game;
	conf->weapons = new std::vector < t_weapon * >;

	conf->game->speed = game.get("speed", 0).asInt();
	conf->game->zoom = game.get("zoom", 0).asFloat();
	conf->game->friendly_fire_own = game.get("friendly_fire_own", 0).asInt();
	conf->game->friendly_fire_team = game.get("friendly_fire_team", 0).asInt();
	conf->game->map = game.get("map", "").asString();
	conf->game->map_duration = game.get("map_duration", 1800).asInt();
	conf->game->mode = game.get("mode", "").asString();
	conf->game->round_nb = game.get("round_nb", 0).asInt();
	conf->game->warmup_duration = game.get("warmup_duration", 60).asInt();

	// Server
	conf->server->name = server.get("name", "").asString();
	conf->server->password = server.get("password", "").asString();
	conf->server->tickrate = server.get("tickrate", 128).asInt();
	conf->server->max_player = server.get("max_player", 0).asInt();
	conf->server->min_player = server.get("min_player", 0).asInt();

	// Horde
	conf->horde->respawnTime = horde.get("respawn_time", 0).asFloat();
	conf->horde->depopTime = horde.get("depop_time", 0).asFloat();
	conf->horde->speed = horde.get("speed", 0).asInt();
	conf->horde->life = horde.get("life", 0).asInt();
	conf->horde->size = horde.get("size", 0).asInt();
	conf->horde->spawn_range = horde.get("spawn_range", 0).asInt();
	conf->horde->damage = horde.get("damage", 0).asInt();
	conf->horde->acceleration = horde.get("acceleration", 0).asInt();

	// end new

	conf->player->acceleration = player.get("acceleration", 0).asInt();
	conf->player->max_speed = player.get("max_speed", 0).asInt();
	conf->player->friction = player.get("friction", 0).asFloat();
	conf->player->size = player.get("size", 0).asInt();
	conf->player->velocity = player.get("velocity_factor", 0).asFloat();
	conf->player->max_energy = player.get("max_energy", 0).asInt();
	conf->player->regen_energy = player.get("regen_energy", 0).asInt();
	conf->player->life = player.get("life", 0).asInt();
	conf->player->regen_life = player.get("regen_life", 0).asFloat();
	conf->player->speed_cap = player.get("speed_cap", 0).asInt();
	conf->player->invulnerable_time = player.get("invulnerable_time", 0).asFloat();

	int i = 0;
	int j = 0;
	Json::Value::Members weaponNames = weapons.getMemberNames();
	Json::Value::const_iterator it = weapons.begin();
	Json::Value::const_iterator end = weapons.end();
	while (it != end)
	{
		const Json::Value weaponValues = *it;
		t_weapon *weaponToPush = new t_weapon;
		// Parse values
		weaponToPush->name = weaponNames[i];
		weaponToPush->category = weaponValues.get("category", "").asString();
		weaponToPush->energy_cost = weaponValues.get("energy_cost", 0).asInt();
		weaponToPush->init_energy_cost = weaponValues.get("init_energy_cost", 0).asInt();
		weaponToPush->damage = weaponValues.get("damage", 0).asInt();
		weaponToPush->speed = weaponValues.get("speed", 0).asInt();
		weaponToPush->duration = weaponValues.get("duration", 2.f).asFloat();
		weaponToPush->fire_rate = weaponValues.get("fire_rate", 0.f).asFloat();
		weaponToPush->size = weaponValues.get("size", 0).asInt();
		weaponToPush->size_explosion = weaponValues.get("size_explosion", 0).asInt();
		weaponToPush->pushback_fire = weaponValues.get("pushback_fire", 0).asInt();
		weaponToPush->pushback_other = weaponValues.get("pushback_other", 0).asInt();
		weaponToPush->acceleration = weaponValues.get("acceleration", 0).asInt();
		weaponToPush->chain = weaponValues.get("chain", 0).asInt();
		weaponToPush->drain_energy = weaponValues.get("drain_energy", 0).asInt();
		weaponToPush->angle = weaponValues.get("angle", 0).asInt();
		weaponToPush->shot_nb = weaponValues.get("shot_nb", 1).asInt();
		weaponToPush->collide_walls = weaponValues.get("collide_walls", true).asBool();
		
		weaponToPush->bounce = weaponValues.get("bounce", 0).asInt();

		weaponToPush->detection_range = weaponValues.get("detection_range", 0).asInt();
		weaponToPush->life = weaponValues.get("life", 0).asInt();
		weaponToPush->capacity = weaponValues.get("capacity", 0).asInt();

		weaponToPush->slow = weaponValues.get("slow", 0).asInt();
		weaponToPush->slow_duration = weaponValues.get("slow_duration", 0.1f).asFloat();

		weaponToPush->subWeaponName = weaponValues.get("sub_weapon", "").asString();

		// Displayed stats in the weapon selection
		weaponToPush->desc = weaponValues.get("desc", "").asString();

		weaponToPush->ratings = NULL;
		const Json::Value ratings = weaponValues["ratings"];
		if (!ratings.isNull())
		{
			auto vecToPush = new std::vector < std::pair<std::string, int> >;
			std::pair<std::string, unsigned short> ratingToPush;
			Json::Value::Members ratingsNameS = ratings.getMemberNames();

			Json::Value::const_iterator itR = ratings.begin();
			Json::Value::const_iterator endR = ratings.end();
			while (itR != endR)
			{
				const Json::Value ratingValue = *itR;
				ratingToPush.first = ratingsNameS[j];
				ratingToPush.second = ratingValue.asInt();
				vecToPush->push_back(ratingToPush);

				++itR;
				++j;
			}
			j = 0;
			weaponToPush->ratings = vecToPush;
		}


		// Push new weapon
		conf->weapons->push_back(weaponToPush);
		++it;
		i++;
	}

	// Fill subweapon
	for (unsigned int i = 0; i < conf->weapons->size(); i++)
	{
		for (unsigned int j = 0; j < conf->weapons->size(); j++)
		{
			if (conf->weapons->at(i)->subWeaponName.compare("") &&
				!(conf->weapons->at(i)->subWeaponName.compare(conf->weapons->at(j)->name)))
				conf->weapons->at(i)->subWeapon = conf->weapons->at(j);
		}
	}

	return ((void *)conf);
}

t_config	*ConfigParser::loadDefaultConf()
{
	t_config	*conf;

	conf = new t_config;
	conf->player = new t_player;
	conf->horde = new t_horde;
	conf->server = new t_server;
	conf->game = new t_game;
	conf->weapons = new std::vector < t_weapon * >;
	return (conf);
}

int	t_weapon::getWeaponIndex()
{
	for (unsigned int i = 0; i < G_conf->weapons->size(); i++)
	{
		if (G_conf->weapons->at(i) == this)
			return i;
	}
	return -1;
}
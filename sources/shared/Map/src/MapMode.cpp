#include	<cstring>
#include	"MapMode.hpp"
#include	"Player.hpp"
#include	"Map.hpp"
#include	"Bot.hpp"
#include	"Respawn.hpp"
#include	"ConfigParser.hpp"
#include	"Map.hpp"

extern t_config	*G_conf;
extern bool G_isOffline;
extern bool G_isServer;

MapMode::MapMode()
{
	_clock.restart();
	_property = new t_mode;
}

MapMode::~MapMode()
{
	delete _property;
}

void	MapMode::initFromConfig()
{
	_mode = findModeFromString(G_conf->game->mode);
	init();
}

void	MapMode::init()
{
	_lastRespawnHorde = sf::Time::Zero;
	_endRound = false;
	_roundNb = 1;

	std::memset(_property, 0, sizeof(*_property));
	_property->horde = false;
	if (_mode == mode_FFA)
	{
		_modeName = "FFA";
		_property->respawnTime = 3;
	}
	else if (_mode == mode_TEAM_DM)
	{
		_modeName = "TEAM_DM";
		_property->team = true;
		_property->killIncTeamScore = true;
		_property->respawnTime = 3;
	}
	else if (_mode == mode_CTF)
	{
		_modeName = "CTF";
		_property->flag = true;
		_property->team = true;
		_property->respawnTime = 3;
	}
	else if (_mode == mode_TEAM_SURVIVOR)
	{
		_modeName = "TEAM_SURVIVOR";
		_property->team = true;
		_property->respawnTime = -1;
	}
	else if (_mode == mode_SURVIVOR)
	{
		_modeName = "SURVIVOR";
		_property->respawnTime = -1;
	}
	else if (_mode == mode_CAPTURE)
	{
		_modeName = "CAPTURE";
		_property->respawnTime = 3;
		_property->capture = true;
		_property->team = true;
	}
}

bool	MapMode::update()
{
	if (G_isOffline || G_isServer)
	{
		handleHorde();
	}
	handleRounds();
	return true;
}

bool	MapMode::isHordeActivated()
{
	return _property->horde;
}

void	MapMode::activeHorde()
{
	ADD_EVENT_SIMPLE(ev_HORDE_ACTIVATE);

	_property->horde = true;

}

void	MapMode::desactiveHorde()
{
	ADD_EVENT_SIMPLE(ev_HORDE_DESACTIVATE);

	_property->horde = false;

	auto	it = S_Map->getElems()->begin();
	auto	end = S_Map->getElems()->end();
	while (it != end)
	{
		if ((*it)->getType() == BOT)
			ADD_EVENT(ev_DELETE, s_event((*it)));
		++it;
	}

}

void	MapMode::handleHorde()
{
	if (!_property->horde)
		return;

	if (_clock.getElapsedTime().asMilliseconds() - _lastRespawnHorde.asMilliseconds() > G_conf->horde->respawnTime * 1000)
	{
		spawnBot();
		_lastRespawnHorde = _clock.getElapsedTime();
	}
}

void	MapMode::spawnBot()
{
	std::shared_ptr<Bot>bot = std::make_shared<Bot>(0, 0, 0, 0);
	int	countRespawnAttempt = 0;

	while (countRespawnAttempt < 20)
	{
		auto it = S_Map->getElems()->begin();
		auto end = S_Map->getElems()->end();
		// Loop to check all respawn
		while (it != end)
		{
			if ((*it)->getType() == RESPAWN)
			{
				std::shared_ptr<Respawn>respawn = std::dynamic_pointer_cast<Respawn>(*it);
				if (respawn->getTeam() == 0)
				{
					if (respawn->respawnObject(bot.get()) == true)
					{
						if (!entityInSpawnRange(bot.get()))
						{
							bot->pushInMap();
							return;
						}
					}

				}
			}
			it++;
		}
		countRespawnAttempt++;
	}
	//delete bot;
}

bool	MapMode::entityInSpawnRange(Bot *bot)
{
	auto	it = S_Map->getElems()->begin();
	auto	end = S_Map->getElems()->end();

	while (it != end)
	{
		if ((*it)->getType() == PLAYER)
		{
			Player *player = dynamic_cast<Player *>((*it).get());
			if (player->getLife() > 0 &&
				std::sqrt(std::pow(player->getX() - bot->getX(), 2) + std::pow(player->getY() - bot->getY(), 2)) < G_conf->horde->spawn_range)
				return true;
		}
		if ((*it)->getType() == FLAG && _property->flag)
		{
			if (std::sqrt(std::pow((*it)->getX() - bot->getX(), 2) + std::pow((*it)->getY() - bot->getY(), 2)) < G_conf->horde->spawn_range)
				return true;
		}
		++it;
	}
	return false;
}

void	MapMode::handleRounds()
{
	if (_property->respawnTime != -1)
		return;

	if (!_endRound && checkRoundVictory())
	{
		_endRound = true;
		_endOfRoundTime = _clock.getElapsedTime();
	}
	if (_endRound &&
		_clock.getElapsedTime().asSeconds() - _endOfRoundTime.asSeconds() > END_ROUND_DURATION)
	{
		changeRound();
	}

}

bool	MapMode::checkRoundVictory()
{
	auto it = S_Map->getPlayers()->begin();
	auto end = S_Map->getPlayers()->end();
	std::shared_ptr<Player>one = NULL;
	std::shared_ptr<Player>two = NULL;
	int activeP = 0;
	while (it != end)
	{
		if ((*it)->getTeam() >= 1)
			activeP++;

		std::shared_ptr<Player>player = (*it);
		if (player->getLife() > 0)
		{
			// more than one player still alive in non team based game
			if (!_property->team)
			{
				if (one || two)
					return false;
			}
			if (player->getTeam() == 1)
				one = player;
			if (player->getTeam() == 2)
				two = player;
		}
		++it;
	}
	if (activeP < 2)
		return false;

	if (_property->team && one && two)
		return false;

	// Round victory detected, inc score
	if (_property->team)
	{
		if (one)
		{
			ADD_EVENT(ev_END_ROUND, s_event(one));
			S_Map->incScore(1, 0);
		}
		else
		{
			ADD_EVENT(ev_END_ROUND, s_event(two));
			S_Map->incScore(0, 1);
		}
	}
	else
	{
		if (one)
		{
			ADD_EVENT(ev_END_ROUND, s_event(one));
			one->setScore(one->getScore() + 1);
		}
		if (two)
		{
			ADD_EVENT(ev_END_ROUND, s_event(two));
			two->setScore(two->getScore() + 1);
		}
	}
	return true;
}

void	MapMode::changeMapMode(eMapMode mode)
{
	_mode = mode;
	init();
	
	Event::addEvent(__FUNCTION__, 213, ev_SWITCH_MAP_MODE);
	//ADD_EVENT(ev_SWITCH_MAP_MODE, s_event(NULL, (void *)mode));
}

eMapMode	MapMode::getModeEnum()
{
	return _mode;
}

s_mode	*MapMode::getProperty()
{
	return _property;
}

const std::string	&MapMode::getModeName()
{
	return _modeName;
}

void	MapMode::setRoundNb(int roundNb)
{
	_roundNb = roundNb;
}


eMapMode MapMode::findModeFromString(std::string &mode)
{
	if (mode == "FFA")
		return mode_FFA;
	else if (mode == "TEAM_DM")
		return mode_TEAM_DM;
	else if (mode == "CTF")
		return mode_CTF;
	else if (mode == "TEAM_SURVIVOR")
		return mode_TEAM_SURVIVOR;
	else if (mode == "SURVIVOR")
		return mode_SURVIVOR;
	else if (mode == "CAPTURE")
		return mode_CAPTURE;
	return mode_FFA;
}

void		MapMode::changeRound()
{
	_clock.restart();
	_endRound = false;
	_lastRespawnHorde = sf::Time::Zero;
	ADD_EVENT_SIMPLE(ev_START_ROUND);
	clearRound();
	_roundNb++;
	if (G_conf && G_conf->game->round_nb && _roundNb > G_conf->game->round_nb)
		S_Map->nextMap();
}


void	MapMode::clearRound()
{
	// clear elems
	{
		auto	it = S_Map->getElems()->begin();
		auto	end = S_Map->getElems()->end();

		while (it != end)
		{

			if ((*it)->getType() == PLAYER)
			{
				Player *player = dynamic_cast<Player *>((*it).get());
				player->respawn();
				player->resetKillsStreak();
			}
			else if ((*it)->getType() != RESPAWN && 
				(*it)->getType() != FLAG && 
				(*it)->getType() != SPEED_FIELD &&
				(*it)->getType() != CAPTURE)
				ADD_EVENT(ev_DELETE, s_event((*it)));
			++it;
		}
	}
}

int	MapMode::getRoundNumber()
{
	return _roundNb;
}

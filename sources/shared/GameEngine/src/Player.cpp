//
// Player.cpp for Player.cpp in /home/gegon/Void-Clash/client/src
// 
// Made by brunel guillaume
// Login   <brunel_g@epitech.net>
// 
// Started on  Sat Aug  3 21:23:38 2013 brunel guillaume
//

#include	<iostream>
#include	<sstream>
#include	<cstring>
#include	<cmath>
#include	"Player.hpp"
#include	"Wall.hpp"
#include	"Bullet.hpp"
#include	"Wall.hpp"
#include	"Bomb.hpp"
#include	"Flag.hpp"
#include	"Respawn.hpp"
#include	"main.hpp"
#include	"Map.hpp"
#include	"Defines.h"
#include	"ConfigParser.hpp"
#include	"Event.hpp"
#include	"Log.hpp"
#include	"AI.hpp"

extern bool	G_isServer;
extern bool	G_isOffline;
extern t_config *G_conf;

Player::Player(float X, float Y,
	float dirX, float dirY,
	bool controled) :
	AObject(PLAYER, X, Y, dirX, dirY)
{
	_clock.restart();
	_insideGravityField = false;
	_insideRespawn = false;
	_controled = controled;
	_flag = NULL;
	_aim.first = 1.0;
	_aim.second = 0.0;
	_radius = G_conf->player->size;
	_energy = G_conf->player->max_energy;
	_life = G_conf->player->life;
	_deaths = 0;
	_kills = 0;
	_killsStreak = 0;
	_team = 0;
	_shield = NULL;
	_playerVelocity = 0.0f;
	memset(&_actions, 0, sizeof(_actions));
	_latency = 0.0f;
	_killedTime = 0.0f;
	_score = 0;
	_respawnTime = sf::Time::Zero;
	_radius = G_conf->player->size;
	_dmg = 0;
	_active = true;

	std::stringstream	ss;
	ss << "Player";
	ss << _id;
	ss >> _nick;

	_ai = NULL;
	_weaponManager = new WeaponManager(this);

	// auto select base wpn
	auto it = G_conf->weapons->begin();
	auto end = G_conf->weapons->end();
	t_weapon *wpnOne = NULL;
	t_weapon *wpnTwo = NULL;
	while (it != end)
	{
		if ((*it)->name == "Tirs Richochés")
			wpnOne = *it;
		if ((*it)->name == "Bombe Explosive")
			wpnTwo = *it;
		++it;
	}
	setWeapons(wpnOne, NULL, wpnTwo, NULL);
}

Player::~Player()
{
	delete _ai;
}

///////////////////////////////////////////////
/////   Called each frame

bool	Player::update()
{
	// CAP SPEED GLOBAL
	_dir.first = std::min(_dir.first, G_conf->player->max_speed * GLOBAL_CAP_SPEED);
	_dir.first = std::max(_dir.first, -G_conf->player->max_speed * GLOBAL_CAP_SPEED);
	_dir.second = std::min(_dir.second, G_conf->player->max_speed * GLOBAL_CAP_SPEED);
	_dir.second = std::max(_dir.second, -G_conf->player->max_speed * GLOBAL_CAP_SPEED);

	// Input
	if (_ai)
		_ai->update();
	else
		gatherInput(); 	// Get player input
	if (G_isServer || G_isOffline || isControled())
	{
		_aim = std::pair<float, float>(_actions.aimX,
			_actions.aimY);
		calculateNormalizedAim();
	}

	// check Death
	checkDeath();

	_energy += G_conf->player->regen_energy * _coefDeltaTime;
	if (_energy > G_conf->player->max_energy)
		_energy = G_conf->player->max_energy;

	_weaponManager->update(_coefDeltaTime);

	checkMove();

	_insideRespawn = false;

	return (true);
}

void	Player::setAI()
{
	std::stringstream	ss;
	ss << "Bot";
	ss << _id;
	ss >> _nick;
	_ai = new AI(std::dynamic_pointer_cast<Player>(shared_from_this()));
}

AI *Player::getAI()
{
	return _ai;
}

void	Player::calculateNormalizedAim()
{
	float	factor = sqrt(std::pow(getX() - _aim.first, 2.0f) + std::pow(getY() - _aim.second, 2.0f));
	float	normalizedAimX = (_aim.first - getX()) / factor;
	float	normalizedAimY = (_aim.second - getY()) / factor;
	_normalizedAim = std::pair<float, float>(normalizedAimX, normalizedAimY);
}

//	Fill player input structure from PLAYER ACTION event created by input engine
//	If there is no ev_PLAYER_ACTION event, do nothing
//	Else, get ->begin(), on client side, we can only have one ev_PLAYER_ACTION event

void	Player::gatherInput()
{
	if (Event::getEventByType(ev_PLAYER_ACTION))
	{
		std::list<std::pair<eventType, s_event> >::const_iterator it =
			Event::getEventByType(ev_PLAYER_ACTION)->begin();
		std::list<std::pair<eventType, s_event> >::const_iterator end =
			Event::getEventByType(ev_PLAYER_ACTION)->end();

		while (it != end)
		{
			if (it->second.trigger.get() == this)
			{
				if (_team != 0)
					_actions = *((s_actions*)(it->second.data));
				break;
			}
			++it;
		}
	}
}

//	Death maangement func - x seconds respawn time when dead then call respawn()
bool	Player::checkDeath()
{
	_currentTime = _clock.getElapsedTime().asMilliseconds();

	if (G_isServer || G_isOffline)
	{
		_invul = false;
		if (_clock.getElapsedTime().asMilliseconds() - _respawnTime.asMilliseconds() <
			G_conf->player->invulnerable_time * 1000)
			_invul = true;
	}

	if (_life <= 0)
	{
		if (_killedTime == 0.0f)
			_killedTime = _clock.getElapsedTime().asMilliseconds();

		if (_flag)
		{
			ADD_EVENT(ev_DROP_FLAG, s_event(_flag, shared_from_this().get()));
			_flag->resetOwner();
			_flag = NULL;

		}
		if (_weaponManager->_bombOnHold)
		{
			ADD_EVENT(ev_DELETE, s_event(_weaponManager->_bombOnHold));
			_weaponManager->_bombOnHold = NULL;
		}

		if (S_Map->getMode()->getProperty()->respawnTime >= 0)
		{
			if (_currentTime - _killedTime > S_Map->getMode()->getProperty()->respawnTime * 1000)
				respawn();
		}
		return true;
	}
	_killedTime = 0.0f;
	return false;
}


void	Player::startRespawnSequence()
{
	_life = 0;
	_killedTime = _currentTime;
}

float	Player::getTimeBeforeRespawn()
{
	return _currentTime - _killedTime - S_Map->getMode()->getProperty()->respawnTime * 1000;
}
///////////////////////////////////////////////
/////   Move related functions

void	Player::checkMove()
{
	if (_actions.moveX == 100)
		_actions.moveX = 100;
	if (_actions.moveX == -100)
		_actions.moveX = -100;
	if (_actions.moveY == 100)
		_actions.moveY = 100;
	if (_actions.moveY == -100)
		_actions.moveY = -100;

	int accel = G_conf->player->acceleration;
	if (_shield && _shield->acceleration)
		accel = _shield->acceleration;

	float	projX = _actions.moveX * accel / 100.0f;
	float	projY = _actions.moveY * accel / 100.0f;
	int	dirNb = 0;

	// Smoothing multiple directions
	if (projX)
		dirNb++;
	if (projY)
		dirNb++;
	if (dirNb != 0)
	{
		projX = projX / dirNb;
		projY = projY / dirNb;
	}
	movePlayer(projX * _coefDeltaTime, projY * _coefDeltaTime);
}

void	Player::movePlayer(float projX, float projY)
{
	// Apply new direction

	_dir.first += projX;
	_dir.second += projY;

	int maxSpeed = G_conf->player->max_speed;
	int accel = G_conf->player->acceleration;
	if (_shield)
	{
		if (_shield->speed)
			maxSpeed = _shield->speed;
		if (_shield->acceleration)
			accel = _shield->acceleration;

		// if (_shield->speed > G_conf->player->max_speed)
		// 	_slow = 0;
	}
	if (maxSpeed == 0)
		maxSpeed++;

	// Cap speed outside speedfield
	if (_insideGravityField == false && maxSpeedReached(maxSpeed, _dir.first, _dir.second))
	{
		float curSpeed = std::sqrt(std::pow(_dir.first, 2.0f) + std::pow(_dir.second, 2.0f));
		// Standard cap speed when accelerating
		if (curSpeed <= maxSpeed + accel * _coefDeltaTime * 2)
		{
			_dir.first /= curSpeed;
			_dir.first *= maxSpeed;
			_dir.second /= curSpeed;
			_dir.second *= maxSpeed;
		}
		else // More than max speed + accel (ex: after speed field or addforce)
		{
			_dir.first /= maxSpeed;
			_dir.first *= (maxSpeed - accel * _coefDeltaTime);
			_dir.second /= maxSpeed;
			_dir.second *= (maxSpeed - accel * _coefDeltaTime);
		}
	}

	// Apply friction if no input and not in Gravity Field
	if (_insideGravityField == false)
		applyFriction(projX, projY);
	else 		// If inside gravity field and opposite direction of input -> apply 2 time friction
	{
		if (_dir.first * projX < 0.0f)
		{
			applyFriction(0, 1);
			applyFriction(0, 1);
		}
		if (_dir.second * projY < 0.0f)
		{
			applyFriction(1, 0);
			applyFriction(1, 0);
		}
	}

	// CAP SPEED GLOBAL
	_dir.first = std::min(_dir.first, maxSpeed * GLOBAL_CAP_SPEED);
	_dir.first = std::max(_dir.first, -maxSpeed * GLOBAL_CAP_SPEED);
	_dir.second = std::min(_dir.second, maxSpeed * GLOBAL_CAP_SPEED);
	_dir.second = std::max(_dir.second, -maxSpeed * GLOBAL_CAP_SPEED);
	_insideGravityField = false;
}

void	Player::applyFriction(float projX, float projY)
{
	if (_controled == false && !_ai)
		return;

	if (std::pow(_dir.first, 2.0f) + std::pow(_dir.second, 2.0f) < 0.001f)
		return;

	if (G_conf->player->friction > 0)
	{
		if (!projX)
			_dir.first /= (G_conf->player->friction * _coefDeltaTime / 100 + 1);
		if (!projY)
			_dir.second /= (G_conf->player->friction * _coefDeltaTime / 100 + 1);
	}
}

/////	Friction is desactivated while in Gravity Field

void	Player::setInsideGravityField()
{
	_insideGravityField = true;
}

void	Player::setInsideRespawn()
{
	_insideRespawn = true;
}

bool	Player::isInsideGravityField()
{
	return _insideGravityField;
}

bool	Player::isInsideRespawn()
{
	return _insideRespawn;
}

bool	Player::maxSpeedReached(float maxSpeed, float projX, float projY)
{
	if (std::pow(projX, 2.0f) + std::pow(projY, 2.0f) >= std::pow(maxSpeed, 2.0f))
		return (true);
	return (false);
}

///////////////////////////////////////////////
/////   Hit

void	Player::isHitBy(const std::shared_ptr<AObject>	&hitter, float dmg)
{
	if (_life <= 0.f)
		return;

	AWeapon *wpn = dynamic_cast<AWeapon *>(hitter.get());
	if (wpn && wpn->getProperty())
		applySlow(wpn->getProperty());

	if (isInvulnerable())
		return;

	if (_shield)
		dmg -= dmg * _shield->damage / 100.f;

	if (dmg == 0.f)
		return;
	if (dmg > 0.f)
	{
		ADD_EVENT(ev_PLAYER_HIT, s_event(hitter, this));
		if (wpn && wpn->getOwner())
			wpn->getOwner()->setDmgDone(wpn->getOwner()->getDmgDone() + dmg);
	}
	if (dmg < 0.f)
		ADD_EVENT(ev_PLAYER_HEAL, s_event(hitter, this));

	if (G_isServer || G_isOffline)
	{
		_life -= dmg;
		if (_life <= 0)
		{
			if (hitter->getOwner())
			{
				hitter->getOwner()->setScore(hitter->getOwner()->getScore() + 1);
				if (G_isOffline)
					ADD_EVENT(ev_KILL, s_event(hitter->getOwner(), this));
			}
			if (G_isServer)
				ADD_EVENT(ev_KILL, s_event(hitter, this));
		}
		if (_life > G_conf->player->life)
			_life = G_conf->player->life;
	}
}

void	Player::setDmgDone(sf::Uint32 dmg)
{
	_dmg = dmg;
}

sf::Uint32	Player::getDmgDone()
{
	return _dmg;
}

void	Player::isHealedFor(sf::Int16 heal)
{
	if (_life <= 0)
		return;
	_life += heal;
	if (_life > G_conf->player->life)
		_life = G_conf->player->life;
}

bool	Player::isInvulnerable() const
{
	return _invul;
}

void	Player::respawn()
{
	_respawnTime = _clock.getElapsedTime();
	if (!(G_isServer || G_isOffline))
		return;
	if (_ai && _life <= 0)
	{
		setWeapons(G_conf->weapons->at(std::rand() % G_conf->weapons->size()), NULL, NULL, NULL);
		if (_team == 1)
			_team = 2;
		if (_team == 2)
			_team = 1;
	}
	int	respawnChance = 10;

	while (respawnChance > 0)
	{
		if (respawnPosition() == false)
			respawnChance--;
		else
			break;
	}
	if (respawnChance == 0)
	{
		VC_WARNING_CRITICAL("Unable to respawn into spawn area of team " + std::to_string(_team) + ". Using default value (1500 1500)");
		_pos.first = 1500;
		_pos.second = 1500;
	}
	_dir.first = 0.0f;
	_dir.second = 0.0f;
	_life = G_conf->player->life;
	_energy = G_conf->player->max_energy;
	_killsStreak = 0;
	//if (_flag)
	//{
	//	ADD_EVENT(ev_DROP_FLAG, s_event(_flag, this));
	//	_flag->resetOwner();
	//	_flag = NULL;
	//}

	// Also need to NULL pointer on bombOnHold
}

bool	Player::respawnPosition()
{
	auto	it = S_Map->getElems()->begin();
	auto	end = S_Map->getElems()->end();
	int	countRespawnTeam = 0;

	int	team = 0;
	if (S_Map->getMode()->getProperty()->team)
		team = _team;
	// Loop to check all respawn
	while (it != end)
	{
		if ((*it)->getType() == RESPAWN)
		{
			Respawn	*respawn = dynamic_cast<Respawn *>((*it).get());
			if (respawn->getTeam() == team)
			{
				countRespawnTeam++;
				if (respawn->respawnObject(this) == true)
					return (true);
			}
		}
		it++;
	}
	// If no respawn is defined for his team, use respawn of team 0
	it = S_Map->getElems()->begin();
	if (countRespawnTeam == 0)
	{
		while (it != end)
		{
			if ((*it)->getType() == RESPAWN)
			{
				Respawn	*respawn = dynamic_cast<Respawn *>((*it).get());
				if (respawn->getTeam() == 0)
				{
					if (respawn->respawnObject(this) == true)
						return (true);
				}
			}
			it++;
		}
	}
	return (false);
}

///////////////////////////////////////////////
/////   Setterz

void		Player::setNick(const std::string &nick)
{
	_nick = nick.substr(0, 10);
}

void	Player::plusKills(int inc)
{
	_kills += inc;
}

void	Player::plusKillsStreak(int inc)
{
	_killsStreak += inc;
}

void	Player::plusDeaths()
{
	_deaths += 1;
}

void		Player::setEnergy(float energy)
{
	_energy = energy;
	if (_energy < 0)
		_energy = 0;
}

void		Player::resetKillsStreak()
{
	_killsStreak = 0;
}

void	Player::reset()
{
	_kills = 0;
	_killsStreak = 0;
	_deaths = 0;
	_dmg = 0;
	if (_flag)
		_flag->reset(false);
	if (_weaponManager->getBombOnHold())
	{
		_weaponManager->getBombOnHold()->setOwner(NULL);
		deleteBombOnHold();
	}
	_score = 0;
}

void		Player::setScore(sf::Int16 score)
{
	_score = score;
}

void		Player::setFlag(const std::shared_ptr<Flag>	 &flag)
{
	_flag = flag;
}

const std::shared_ptr<Flag>	 &Player::getFlag() const
{
	return _flag;
}

void		Player::setLatency(float latency)
{
	_latency = latency;
}

void	Player::setWeapons(t_weapon *primary, t_weapon *primaryAlt, t_weapon *secondary, t_weapon *secondaryAlt)
{
	_weaponManager->setWeapons(primary, primaryAlt, secondary, secondaryAlt);
}

// Set the shield when activated
void		Player::setShield(t_weapon *shield)
{
	_shield = shield;
}

void		Player::setAim(float aimX, float aimY)
{
	_aim.first = aimX;
	_aim.second = aimY;
	calculateNormalizedAim();
}


void	Player::setActive(bool status)
{
	_active = status;
}

///////////////////////////////////////////////
/////   Getterz

const std::string	&Player::getNick() const
{
	return (_nick);
}

bool	Player::isControled() const
{
	return (_controled);
}

const std::shared_ptr<Bomb>	&Player::getBombOnHold() const
{
	return _weaponManager->getBombOnHold();
}

const std::pair<float, float>	&Player::getAim() const
{
	return (_aim);
}

const std::pair<float, float>	&Player::getAimNormalized() const
{
	return _normalizedAim;
}

const int&	Player::getAccel(void) const
{
	return G_conf->player->acceleration;
}

const int&	Player::getRadius(void)	const
{
	return _radius;
}

const float&	Player::getEnergy(void)	const
{
	return _energy;
}

sf::Int16	Player::getLife(void) const
{
	return _life;
}

sf::Int16	Player::getScore() const
{
	return (_score);
}

sf::Int16	Player::getKills() const
{
	return (_kills);
}

sf::Int16	Player::getKillsStreak() const
{
	return (_killsStreak);
}

sf::Int16	Player::getDeaths() const
{
	return (_deaths);
}

float		Player::getPlayerVelocity()
{
	return (_playerVelocity);
}

float		Player::getLatency() const
{
	return (_latency);
}

bool		Player::isRespawning()
{
	if (_killedTime)
		return true;
	return false;
}

const sf::Time	&Player::getMapTime() const
{
	return _mapTime;
}

void		Player::setMapTime(sf::Time time)
{
	_mapTime = time;
}

const s_actions		&Player::getActions() const
{
	return _actions;
}


WeaponManager* Player::getWeaponsManager(void)
{
	return _weaponManager;
}

t_weapon	*Player::getWeapons(bool primary, bool alt)
{
	return _weaponManager->getWeapons(primary, alt);
}

bool	Player::isActive()
{
	return _active;
}

bool		Player::isWeaponActive(t_weapon *weaponToCheck)
{
	return _weaponManager->isWeaponActive(weaponToCheck);
}

// Return non NULL only if the t_weapon is activated by the player
t_weapon	*Player::getShield() const
{
	return _shield;
}

void	Player::addPlayerVelocity(float *dirX, float *dirY)
{
	_weaponManager->addPlayerVelocity(dirX, dirY);
}

void	Player::checkVelocity()
{
	_weaponManager->checkVelocity();
}


void		Player::deleteBombOnHold(void)
{
	_weaponManager->deleteBombOnHold();
}

void		Player::setBombOnHold(const std::shared_ptr<Bomb>	&bomb)
{
	_weaponManager->setBombOnHold(bomb);
}
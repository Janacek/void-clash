#include	<iostream>
#include	<cmath>
#include	"WeaponManager.hpp"
#include	"Defines.h"
#include	"Player.hpp"
#include	"Bullet.hpp"
#include	"GravityField.hpp"
#include	"Rocket.hpp"
#include	"Wall.hpp"
#include	"Bomb.hpp"
#include	"Turret.hpp"
#include	"ConfigParser.hpp"
#include	"Event.hpp"
#include	"Map.hpp"

extern t_config *G_conf;
extern bool g_crazyShoot;
extern bool G_isServer;
extern bool G_isOffline;

// WEAPON SELECTOR
// 
WeaponManager::WeaponManager(Player *player)
{
	_player = std::shared_ptr<Player>(player);
	_bombOnHold = NULL;
	_bombPrimary = NULL;
	_currentTime = 0;
	_lastFirePrimary = 0.0f;
	_lastFireSecondary = 0.0f;

	_primary = NULL;
	_primaryAlt = NULL;
	_primaryAltUsed = false;

	_secondary = NULL;
	_secondaryAlt = NULL;
	_secondaryAltUsed = false;
}

WeaponManager::~WeaponManager()
{

}

bool WeaponManager::update(float deltaTime)
{
	(void)deltaTime;

	_currentTime = _player->_clock.getElapsedTime().asSeconds();

	_shieldActivated = false;
	if (_player->isRespawning() || _player->isInsideRespawn())
	{
		if (_bombOnHold)
		{
			ADD_EVENT(ev_DELETE, s_event(_bombOnHold));
			_bombOnHold = NULL;
		}
		return true;
	}

	updateWeapons();
	if (!_shieldActivated)
		_player->_shield = NULL;
	return true;
}

// Check switch of weapon and call relative actions
void	WeaponManager::updateWeapons()
{
	// switch
	//if (_player->_actions.switchPrimary)
	//	_primaryAltUsed ^= 1;

	//if (_player->_actions.switchSecondary)
	//	_secondaryAltUsed ^= 1;

	// primary weapon
	t_weapon	*weaponUsed = _primary;
	if (_primaryAltUsed)
		weaponUsed = _primaryAlt;

	checkFire(weaponUsed, true);

	// secondary weapon
	weaponUsed = _secondary;
	if (_secondaryAltUsed)
		weaponUsed = _secondaryAlt;

	checkFire(weaponUsed, false);
}

void	WeaponManager::checkFire(t_weapon *weaponCfg, bool primary)
{
	if (weaponCfg == NULL)
		return;

	checkVelocity();

	bool	buttonPressed = _player->_actions.primary;
	if (!primary)
		buttonPressed = _player->_actions.secondary;
	float	lastFire = _lastFirePrimary;
	if (!primary)
		lastFire = _lastFireSecondary;

	if (weaponCfg->category.compare("BOMB") == 0)
	{
		checkBomb(weaponCfg, primary, buttonPressed);
		return;
	}

	if (_currentTime - lastFire > weaponCfg->fire_rate &&
		buttonPressed)
	{
		if (weaponCfg->category.compare("BULLET") == 0)
			fireBullet(weaponCfg, primary);
		else if (weaponCfg->category.compare("ROCKET") == 0)
			fireRocket(weaponCfg, primary);
		else if (weaponCfg->category.compare("SHIELD") == 0)
			fireShield(weaponCfg, primary);
		else if (weaponCfg->category.compare("TURRET") == 0)
			fireTurret(weaponCfg, primary);
		else if (weaponCfg->category.compare("GRAVITY") == 0)
			fireGravity(weaponCfg, primary);
	}
}

///////////////////////////////////////////////
/////   Set weapons used by _player

void	WeaponManager::setWeapons(t_weapon *primary, t_weapon *primaryAlt, t_weapon *secondary, t_weapon *secondaryAlt)
{
	_primary = primary;
	_primaryAlt = primaryAlt;
	_secondary = secondary;
	_secondaryAlt = secondaryAlt;
	ADD_EVENT(ev_WEAPON_SELECTION, s_event((_player)));

}

t_weapon	*WeaponManager::getWeaponSelected(bool primary)
{
	if (primary)
	{
		if (_primaryAltUsed)
			return _primaryAlt;
		return _primary;
	}
	else
	{
		if (_secondaryAltUsed)
			return _secondaryAlt;
		return _secondary;
	}
}

t_weapon	*WeaponManager::getWeapons(bool primary, bool alt)
{
	if (primary)
	{
		if (alt)
			return _primaryAlt;
		return _primary;
	}
	else
	{
		if (alt)
			return _secondaryAlt;
		return _secondary;
	}
}

bool		WeaponManager::isWeaponActive(t_weapon *weaponToCheck)
{
	if (_primary == weaponToCheck)
		return true;
	else if (_primaryAlt == weaponToCheck)
		return true;
	else if (_secondary == weaponToCheck)
		return true;
	else if (_secondaryAlt == weaponToCheck)
		return true;
	return false;
}

///////////////////////////////////////////////
/////   Bomb related functions

const std::shared_ptr<Bomb>	&WeaponManager::getBombOnHold(void) const
{
	return _bombOnHold;
}

void	WeaponManager::deleteBombOnHold(void)
{
	_bombOnHold = NULL;
}

void	WeaponManager::setBombOnHold(const std::shared_ptr<Bomb> &bomb)
{
	_bombOnHold = bomb;
}

void	WeaponManager::checkBomb(t_weapon *weaponCfg, bool primary, bool buttonPressed)
{
	// Prime bomb
	if (_player->_energy >= weaponCfg->energy_cost && _bombOnHold == NULL &&
		buttonPressed == true && buttonPressed && !_lastFramePressed)
	{
		// Cooldown
		float	lastFire = _lastFirePrimary;
		if (!primary)
			lastFire = _lastFireSecondary;
		if (_currentTime - lastFire > weaponCfg->fire_rate &&
			buttonPressed)
		{
			if (primary)
				_lastFirePrimary = _currentTime;
			else
				_lastFireSecondary = _currentTime;

			if (!(G_isServer || G_isOffline))
				return;
			_bombPrimary = primary;
			std::shared_ptr<Bomb> toPush = std::make_shared<Bomb>(_player->getX(), _player->getY(), _player->getDirX(), _player->getDirY(), _player);
			toPush->init(weaponCfg);
			toPush->pushInMap();
			ADD_EVENT(ev_BOMB_PRIMED, s_event(_player, toPush.get()));
			_bombOnHold = toPush;
			_player->_energy -= weaponCfg->energy_cost;
		}
	}
	else if (_bombOnHold != NULL && buttonPressed == false && _bombPrimary == primary)
	{
		float	dirX = _player->_normalizedAim.first * weaponCfg->speed;
		float	dirY = _player->_normalizedAim.second * weaponCfg->speed;

		addPlayerVelocity(&dirX, &dirY);
		_bombOnHold->setDirX(_bombOnHold->getDirX() + dirX);
		_bombOnHold->setDirY(_bombOnHold->getDirY() + dirY);
		ADD_EVENT(ev_BOMB_LAUNCHED, s_event(_bombOnHold));
		_bombOnHold->launch();
		_bombOnHold = NULL;
		_player->addForce(-_player->_normalizedAim.first * weaponCfg->pushback_fire, -_player->_normalizedAim.second * weaponCfg->pushback_fire);
	}
	_lastFramePressed = buttonPressed;
}

///////////////////////////////////////////////
/////   Shield related functions

void	WeaponManager::fireShield(t_weapon *weaponCfg, bool primary)
{
	if (!_player->_shield)
	{
		if (_player->_energy < weaponCfg->init_energy_cost)
			return;
		_player->_energy -= weaponCfg->init_energy_cost;

		if (primary)
			_lastFirePrimary = _currentTime;
		else
			_lastFireSecondary = _currentTime;
	}
	else
	{
		if (_player->_energy < weaponCfg->energy_cost  * _player->_coefDeltaTime)
			return;
		_player->_energy -= weaponCfg->energy_cost * _player->_coefDeltaTime;

		if (primary)
			_lastFirePrimary = _currentTime;
		else
			_lastFireSecondary = _currentTime;
	}

	_player->_shield = weaponCfg;
	_shieldActivated = true;
}

///////////////////////////////////////////////
/////   Turret related functions

void	WeaponManager::fireTurret(t_weapon *weaponCfg, bool primary)
{
	if (_player->_energy < weaponCfg->energy_cost)
		return;
	_player->_energy -= weaponCfg->energy_cost;
	if (primary)
		_lastFirePrimary = _currentTime;
	else
		_lastFireSecondary = _currentTime;

	if (weaponCfg->capacity)
	{
		// Delete previous turret
		std::list<std::shared_ptr<AObject>>::reverse_iterator	it = S_Map->getElems()->rbegin();
		std::list<std::shared_ptr<AObject>>::reverse_iterator	end = S_Map->getElems()->rend();
		int	nbOfTurret = 0;
		while (it != end)
		{
			if ((*it)->getType() == TURRET)
			{
				Turret *turret = dynamic_cast<Turret *>((*it).get());
				if (turret->getOwner() == _player && turret->getProperty() == weaponCfg)
				{
					nbOfTurret++;
					if (nbOfTurret >= weaponCfg->capacity)
						ADD_EVENT(ev_DELETE, s_event(*it));
				}
			}
			++it;
		}
	}

	if (!(G_isServer || G_isOffline))
		return;

	float	posX = _player->_pos.first + _player->_normalizedAim.first * (_player->_radius + weaponCfg->size);
	float	posY = _player->_pos.second + _player->_normalizedAim.second * (_player->_radius + weaponCfg->size);
	float	dirX = _player->_normalizedAim.first * weaponCfg->speed;
	float	dirY = _player->_normalizedAim.second * weaponCfg->speed;

	addPlayerVelocity(&dirX, &dirY);
	std::shared_ptr<Turret>toPush = std::make_shared<Turret>(posX, posY, dirX, dirY, _player);
	toPush->init(weaponCfg);
	toPush->pushInMap();
	ADD_EVENT(ev_TURRET_LAUNCHED, s_event(_player));
}

///////////////////////////////////////////////
/////   Rocket related functions

void	WeaponManager::fireRocket(t_weapon *weaponCfg, bool primary)
{
	if (_player->_energy < weaponCfg->energy_cost)
		return;
	_player->_energy -= weaponCfg->energy_cost;
	if (primary)
		_lastFirePrimary = _currentTime;
	else
		_lastFireSecondary = _currentTime;

	if (!(G_isServer || G_isOffline))
		return;

	float	posX = _player->_pos.first + _player->_normalizedAim.first * (_player->_radius + weaponCfg->size);
	float	posY = _player->_pos.second + _player->_normalizedAim.second * (_player->_radius + weaponCfg->size);
	float	dirX = _player->_normalizedAim.first * weaponCfg->speed;
	float	dirY = _player->_normalizedAim.second * weaponCfg->speed;

	addPlayerVelocity(&dirX, &dirY);
	std::shared_ptr<Rocket>toPush = std::make_shared<Rocket>(posX, posY, dirX, dirY, _player->_aim.first, _player->_aim.second, _player);
	toPush->init(weaponCfg);
	toPush->pushInMap();
	ADD_EVENT(ev_ROCKET_LAUNCHED, s_event(_player));

	_player->addForce(-_player->_normalizedAim.first * weaponCfg->pushback_fire, -_player->_normalizedAim.second * weaponCfg->pushback_fire);
}

///////////////////////////////////////////////
/////   Gravity related functions

void	WeaponManager::fireGravity(t_weapon *weaponCfg, bool primary)
{
	if (_player->_energy < weaponCfg->energy_cost)
		return;
	_player->_energy -= weaponCfg->energy_cost;
	if (primary)
		_lastFirePrimary = _currentTime;
	else
		_lastFireSecondary = _currentTime;


	if (!(G_isServer || G_isOffline))
		return;

	if (weaponCfg->capacity)
	{
		// Delete previous Gravity
		std::list<std::shared_ptr<AObject>>::reverse_iterator	it = S_Map->getElems()->rbegin();
		std::list<std::shared_ptr<AObject>>::reverse_iterator	end = S_Map->getElems()->rend();
		int	nbOfGrav = 0;
		while (it != end)
		{
			if ((*it)->getType() == GRAVITY_FIELD)
			{
				GravityField *g = dynamic_cast<GravityField *>((*it).get());
				if (g->getOwner() == _player && g->getProperty() == weaponCfg)
				{
					nbOfGrav++;
					if (nbOfGrav >= weaponCfg->capacity)
						ADD_EVENT(ev_DELETE, s_event(*it));
				}
			}
			++it;
		}
	}

	for (int i = 0; i < weaponCfg->shot_nb; i++)
	{
		float	dirX = _player->_normalizedAim.first * weaponCfg->speed;
		float	dirY = _player->_normalizedAim.second * weaponCfg->speed;
		float	speed = sqrt(pow(dirX, 2) + pow(dirY, 2));

		int spread = (i - weaponCfg->shot_nb / 2) * weaponCfg->angle;
		float angleInRadians = atan2(_player->_normalizedAim.second, _player->_normalizedAim.first) + spread / 180.f * 3.14f;
		dirX = speed * std::cos(angleInRadians);
		dirY = speed * std::sin(angleInRadians);
		float	posX = _player->_pos.first + std::cos(angleInRadians) * (_player->_radius + weaponCfg->size);
		float	posY = _player->_pos.second + std::sin(angleInRadians) * (_player->_radius + weaponCfg->size);
		addPlayerVelocity(&dirX, &dirY);
		std::shared_ptr<GravityField>toPush = std::make_shared<GravityField>(posX, posY, dirX, dirY, _player);
		toPush->init(weaponCfg);
		toPush->pushInMap();
		ADD_EVENT(ev_GRAVITY_LAUNCHED, s_event(_player));
	}
	_player->addForce(-_player->_normalizedAim.first * weaponCfg->pushback_fire, -_player->_normalizedAim.second * weaponCfg->pushback_fire);
}

///////////////////////////////////////////////
/////   Bullet related functions

void	WeaponManager::fireBullet(t_weapon *weaponCfg, bool primary)
{
	if (_player->_energy < weaponCfg->energy_cost)
		return;
	_player->_energy -= weaponCfg->energy_cost;
	if (primary)
		_lastFirePrimary = _currentTime;
	else
		_lastFireSecondary = _currentTime;

	if (!(G_isServer || G_isOffline))
		return;
	for (int i = 0; i < weaponCfg->shot_nb; i++)
	{
		float	dirX = _player->_normalizedAim.first * weaponCfg->speed;
		float	dirY = _player->_normalizedAim.second * weaponCfg->speed;
		float	speed = sqrt(pow(dirX, 2) + pow(dirY, 2));

		int spread = (i - weaponCfg->shot_nb / 2) * weaponCfg->angle;
		float angleInRadians = atan2(_player->_normalizedAim.second, _player->_normalizedAim.first) + spread / 180.f * 3.14f;
		dirX = speed * std::cos(angleInRadians);
		dirY = speed * std::sin(angleInRadians);
		float	posX = _player->_pos.first + std::cos(angleInRadians) * (_player->_radius + weaponCfg->size);
		float	posY = _player->_pos.second + std::sin(angleInRadians) * (_player->_radius + weaponCfg->size);
		addPlayerVelocity(&dirX, &dirY);

		std::shared_ptr<Bullet>toPush = std::make_shared<Bullet>(posX, posY, dirX, dirY, _player);
		toPush->init(weaponCfg);
		toPush->pushInMap();
		ADD_EVENT(ev_BULLET_LAUNCHED, s_event(_player));
	}
	_player->addForce(-_player->_normalizedAim.first * weaponCfg->pushback_fire, -_player->_normalizedAim.second * weaponCfg->pushback_fire);
}

// Other

void	WeaponManager::addPlayerVelocity(float *dirX, float *dirY)
{
	*dirX += _player->_dir.first * G_conf->player->velocity * std::abs(_player->_playerVelocity);
	*dirY += _player->_dir.second * G_conf->player->velocity * std::abs(_player->_playerVelocity);
}

void	WeaponManager::checkVelocity()
{
	float	angleAim = std::atan2(_player->_actions.aimX, _player->_actions.aimY);
	float	angleMove = std::atan2(_player->_dir.first, _player->_dir.second);

	_player->_playerVelocity = std::abs(angleAim - angleMove);
	_player->_playerVelocity = _player->_playerVelocity / (3.1416f);
	_player->_playerVelocity = (std::abs(1 - _player->_playerVelocity) * 2) - 1;
}

// Cooldown getters
// Cooldown between 0 and 1
// 0 not availaible - 1 availaible
float WeaponManager::getCurrentCooldownPrimary(void)
{
	if (_primary == NULL)
		return 0;

	float diff = _currentTime - _lastFirePrimary;
	return std::max(0.0f, (_primary->fire_rate - diff) / _primary->fire_rate);
}

float WeaponManager::getCurrentCooldownSeconday(void)
{
	if (_secondary == NULL)
		return 0;

	float diff = _currentTime - _lastFireSecondary;
	return std::max(0.0f, (_secondary->fire_rate - diff) / _secondary->fire_rate);
}

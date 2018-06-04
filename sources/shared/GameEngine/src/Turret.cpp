#include	<iostream>
#include	"Turret.hpp"
#include	"Bullet.hpp"
#include	"Rocket.hpp"
#include	"Wall.hpp"
#include	"Player.hpp"
#include	"Defines.h"
#include	"ConfigParser.hpp"
#include	"Event.hpp"
#include	"Map.hpp"

extern bool	G_isOffline;
extern bool	G_isServer;
extern t_config *G_conf;

Turret::Turret() :
AWeapon(TURRET, 0, 0, 0, 0)
{
	_owner = NULL;
	_clock.restart();
	_lastFire = 0.f;
	_dir.first = 0;
	_dir.second = 0;
	_ennemyLocked = NULL;
}

Turret::Turret(float X, float Y,
	float dirX, float dirY,
	const std::shared_ptr<Player> &owner) :
	AWeapon(TURRET, X, Y, dirX, dirY)
{
	_owner = owner;
	_clock.restart();
	_lastFire = 0.f;
	_ennemyLocked = NULL;
}

Turret::~Turret()
{
}

void Turret::init(t_weapon *weaponCfg)
{
	_life = weaponCfg->life;
	AWeapon::init(weaponCfg);
}

///////////////////////////////////////////////
/////   Called each frame

bool	Turret::update()
{
	if (!AWeapon::update())
		return false;

	if (!stillActive())
		return false;

	checkLockedEnnemy();
	t_weapon	*subWeapon = _property->subWeapon;
	if (_time - _lastFire > subWeapon->fire_rate)
	{
		if (G_isServer || G_isOffline)
		{
			if (checkFire())
				_lastFire = _time;
		}
	}
	return (true);
}

bool	Turret::stillActive()
{
	if (!_owner || !_owner->getTeam() || _owner->isRespawning() || !_owner->isWeaponActive(_property))
		return false;

	_time = _clock.getElapsedTime().asSeconds();
	if (_property->duration != 0.f && _clock.getElapsedTime().asSeconds() > _property->duration)
		return false;

	return true;
}
// Check if ennemy is in range
bool	Turret::checkFire()
{
	if (!_ennemyLocked)
		return false;

	float	factor = sqrt(std::pow(getX() - _ennemyLocked->getX(), 2.0f) + std::pow(getY() - _ennemyLocked->getY(), 2.0f));
	float	normalizedAimX = (_ennemyLocked->getX() - getX()) / factor;
	float	normalizedAimY = (_ennemyLocked->getY() - getY()) / factor;
	_normalizedAim = std::pair<float, float>(normalizedAimX, normalizedAimY);

	if (_property->subWeapon->category.compare("BULLET") == 0)
	{
		fireBullet(_property->subWeapon);
		return true;
	}
	else if (_property->subWeapon->category.compare("ROCKET") == 0)
	{
		fireRocket(_property->subWeapon, _ennemyLocked);
		return true;
	}
	//else if (_property->subWeapon->category.compare("TURRET") == 0)
	//{
	//	fireTurret(_property->subWeapon);
	//	return true;
	//}
	return false;
}

void	Turret::checkLockedEnnemy()
{
	if (_ennemyLocked && ennemyDetected(_ennemyLocked))
		return;

	{
		// Detected locked ennemy
		auto	it = S_Map->getPlayers()->begin();
		auto	end = S_Map->getPlayers()->end();

		while (it != end)
		{
			AObject *nmy = (*it).get();
			if (ennemyDetected(nmy))
			{
				_ennemyLocked = nmy;
				return;
			}
			++it;
		}
	}

	{
		// Detected locked bot
		auto	it = S_Map->getBots()->begin();
		auto	end = S_Map->getBots()->end();

		while (it != end)
		{
			AObject *nmy = (*it).get();
			if (ennemyDetected(nmy))
			{
				_ennemyLocked = nmy;
				return;
			}
			++it;
		}
	}

	_ennemyLocked = NULL;
}

bool	Turret::ennemyDetected(AObject *nmy)
{
	if (nmy->getType() == PLAYER)
	{
		Player *playerNmy = dynamic_cast<Player *>(nmy);

		if (playerNmy == NULL)
		  return false;

		if (playerNmy->getTeam() == 0 || playerNmy->isRespawning() || playerNmy == _owner.get())
			return false;

		if (S_Map->getMode()->getProperty()->team && playerNmy->getTeam() == _owner->getTeam())
			return false;
	}
	else if (nmy->getType() == BOT)
	{
		Bot *bot = dynamic_cast<Bot *>(nmy);
		if (bot->getLife() <= 0)
			return false;
	}
	else
		return false;

	// Is in range
	if (sqrt(pow(nmy->getX() - getX(), 2) + pow(nmy->getY() - getY(), 2)) > _property->detection_range + nmy->getRadius())
		return false;

	// No wall in the way
	if (checkSegmentSegment(nmy))
		return false;

	return true;
}

bool	Turret::checkSegmentSegment(AObject *nmy)
{
	Point A;
	Point B;
	Point O;
	Point P;

	O.x = _pos.first;
	O.y = _pos.second;
	P.x = nmy->getX();
	P.y = nmy->getY();

	auto	it = S_Map->getWalls()->begin();
	auto	end = S_Map->getWalls()->end();
	while (it != end)
	{
		float dist = std::sqrt(std::pow((*it)->getEndX() - (*it)->getX(), 2) + std::pow((*it)->getEndY() - (*it)->getY(), 2));
		if (dist == 0)
			dist = 1;
		int radius = (*it)->getRadius() + _property->subWeapon->size;

		A.x = (*it)->getX() - ((*it)->getEndX() - (*it)->getX()) / dist * radius;
		B.x = (*it)->getEndX() + ((*it)->getEndX() - (*it)->getX()) / dist * radius;
		A.y = (*it)->getY() - ((*it)->getEndY() - (*it)->getY()) / dist * radius;
		B.y = (*it)->getEndY() + ((*it)->getEndY() - (*it)->getY()) / dist * radius;

		if (CollisionSegSeg(A, B, O, P))
			return true;
		++it;
	}
	return false;
}

bool	Turret::CollisionSegSeg(Point A, Point B, Point O, Point P)
{
	if (CollisionDroiteSeg(A, B, O, P) == false)
		return false;  // inutile d'aller plus loin si le segment [OP] ne touche pas la droite (AB) 
	if (CollisionDroiteSeg(O, P, A, B) == false)
		return false;
	return true;
}

bool	Turret::CollisionDroiteSeg(Point A, Point B, Point O, Point P)
{
	Vecteur AO, AP, AB;
	AB.x = B.x - A.x;
	AB.y = B.y - A.y;
	AP.x = P.x - A.x;
	AP.y = P.y - A.y;
	AO.x = O.x - A.x;
	AO.y = O.y - A.y;
	if ((AB.x*AP.y - AB.y*AP.x)*(AB.x*AO.y - AB.y*AO.x) < 0)
		return true;
	else
		return false;
}


///////////////////////////////////////////////
/////   Bullet related functions

void	Turret::fireBullet(t_weapon *weaponCfg)
{
	for (int i = 0; i < weaponCfg->shot_nb; i++)
	{
		float	dirX = _normalizedAim.first * weaponCfg->speed;
		float	dirY = _normalizedAim.second * weaponCfg->speed;
		float	speed = sqrt(pow(dirX, 2) + pow(dirY, 2));

		int spread = (i - weaponCfg->shot_nb / 2) * weaponCfg->angle;
		float angleInRadians = atan2(_normalizedAim.second, _normalizedAim.first) + spread / 180.f * 3.14f;
		dirX = speed * std::cos(angleInRadians);
		dirY = speed * std::sin(angleInRadians);
		float	posX = _pos.first + std::cos(angleInRadians) * (_radius + weaponCfg->size + 30);
		float	posY = _pos.second + std::sin(angleInRadians) * (_radius + weaponCfg->size + 30);
		std::shared_ptr<Bullet>toPush = std::make_shared<Bullet>(posX, posY, dirX, dirY, _owner, this->getType());
		toPush->init(weaponCfg);
		toPush->pushInMap();
		ADD_EVENT(ev_TURRET_FIRE, s_event(_owner, toPush.get()));
	}
	//addForce(-_normalizedAim.first * weaponCfg->pushback_fire, -_normalizedAim.second * weaponCfg->pushback_fire);
}

///////////////////////////////////////////////
/////   Turret related functions

void	Turret::fireTurret(t_weapon *weaponCfg)
{
	float	posX = _pos.first + _normalizedAim.first * (_radius + weaponCfg->size);
	float	posY = _pos.second + _normalizedAim.second * (_radius + weaponCfg->size);
	float	dirX = _normalizedAim.first * weaponCfg->speed;
	float	dirY = _normalizedAim.second * weaponCfg->speed;
	std::shared_ptr<Turret>toPush = std::make_shared<Turret>(posX, posY, dirX, dirY, _owner);
	toPush->init(weaponCfg);
	toPush->pushInMap();
	ADD_EVENT(ev_TURRET_LAUNCHED, s_event(_owner));
	addForce(-_normalizedAim.first * weaponCfg->pushback_fire, -_normalizedAim.second * weaponCfg->pushback_fire);
}

///////////////////////////////////////////////
/////   Rocket related functions

void	Turret::fireRocket(t_weapon *weaponCfg, AObject *nmy)
{
	float	posX = _pos.first + _normalizedAim.first * (_radius + weaponCfg->size);
	float	posY = _pos.second + _normalizedAim.second * (_radius + weaponCfg->size);
	float	dirX = _normalizedAim.first * weaponCfg->speed;
	float	dirY = _normalizedAim.second * weaponCfg->speed;

	std::shared_ptr<Rocket>toPush = std::make_shared<Rocket>(posX, posY, dirX, dirY, nmy->getX(), nmy->getY(), _owner);
	toPush->init(weaponCfg);
	toPush->pushInMap();
	ADD_EVENT(ev_TURRET_FIRE, s_event(_owner, toPush.get()));

	addForce(-_normalizedAim.first * weaponCfg->pushback_fire, -_normalizedAim.second * weaponCfg->pushback_fire);
}

///////////////////////////////////////////////
/////   Hit

void	Turret::isHitBy(AWeapon *hitter, float dmg)
{
	if (_life <= 0.f)
		return;

	if (hitter && hitter->getProperty())
		applySlow(hitter->getProperty());

	_life -= dmg;
	ADD_EVENT(ev_TURRET_HIT, s_event(shared_from_this(), hitter));
	if (_life <= 0)
	{
		ADD_EVENT(ev_TURRET_DESTROYED, s_event(shared_from_this(), hitter));
		ADD_EVENT(ev_DELETE, s_event(shared_from_this()));

		// Check for deleted turret as turret target
		auto	it = S_Map->getElems()->begin();
		auto	end = S_Map->getElems()->end();

		while (it != end)
		{
			if ((*it)->getType() == TURRET)
			{
				Turret *turret = dynamic_cast<Turret *>((*it).get());
				if (turret->getEnnemyLocked() == this)
					turret->resetEnnemyLocked();
			}
			++it;
		}
	}
}

///////////////////////////////////////////////
/////   Getterz

sf::Int16	Turret::getLife(void) const
{
	return _life;
}

AObject		*Turret::getEnnemyLocked() const // NULL if no ennemy locked
{
	return _ennemyLocked;
}

void		Turret::resetEnnemyLocked()
{
	_ennemyLocked = NULL;
}

#pragma once

#include	<utility>
#include	<SFML/System.hpp>
#include	"AWeapon.hpp"
#include	"Defines.h"

class	GameEngine;
class	Player;

class	Turret : public AWeapon
{
public:
	Turret();
	Turret(float, float, float, float, const std::shared_ptr<Player> &);
	virtual ~Turret();

	virtual bool	update();
	virtual void	init(t_weapon *weaponCfg);

	// Get hit
	void	isHitBy(AWeapon *hitter, float dmg);

	// Getterz
	sf::Int16	getLife(void) const;
	AObject		*getEnnemyLocked() const;

	// Setterz
	void		resetEnnemyLocked();

	friend sf::Packet& operator >>(sf::Packet& packet, Turret &m);

private:
	// Subweapon fire
	bool	checkFire();
	void	fireBullet(t_weapon *weaponCfg);
	void	fireRocket(t_weapon *weaponCfg, AObject *nmy);
	void	fireTurret(t_weapon *weaponCfg);

	// Ennemy lock
	void	checkLockedEnnemy();
	bool	ennemyDetected(AObject *nmy);
	bool	checkSegmentSegment(AObject *nmy);
	bool	CollisionSegSeg(Point A, Point B, Point O, Point P);
	bool	CollisionDroiteSeg(Point A, Point B, Point O, Point P);

	bool	stillActive();

protected:
	sf::Int16	_life;

	float		_time;
	float		_lastFire;
	sf::Clock	_clock;
	std::pair<float, float>	_normalizedAim;

	AObject		*_ennemyLocked;
};

sf::Packet& operator >>(sf::Packet& packet, Turret &m);
#ifndef		WEAPON_MANAGER_HPP_
# define	WEAPON_MANAGER_HPP_

#include	<utility>
#include "Bomb.hpp"

class Player;


//////////////////////////////////////////////////////////////////////
/////	This class is in charge of:
/////	Weapon switching
/////	Weapon firing

class	WeaponManager
{
public:
	WeaponManager(Player *player);
	~WeaponManager();

	virtual bool update(float	deltaTime);

	// Update weapons
	void	updateWeapons();
	void	checkFire(t_weapon *weaponCfg, bool primary);

	// Primary / Secondary selector
	void		setWeapons(t_weapon *primary, t_weapon *primaryAlt, t_weapon *secondary, t_weapon *secondaryAlt);
	t_weapon	*getWeaponSelected(bool primary);
	t_weapon	*getWeapons(bool primary, bool alt);
	bool		isWeaponActive(t_weapon *weaponToCheck);

	// Bullet
	void	fireBullet(t_weapon *weaponCfg, bool primary);

	// Bomb
	const std::shared_ptr<Bomb>	&getBombOnHold(void) const;
	void	deleteBombOnHold(void);
	void	setBombOnHold(const std::shared_ptr<Bomb>&);
	void	checkBomb(t_weapon *weaponCfg, bool primary, bool buttonPressed);
	
	// Shield
	void	fireShield(t_weapon *weaponCfg, bool primary);

	// Rocket
	void	fireRocket(t_weapon *weaponCfg, bool primary);

	// Turret
	void	fireTurret(t_weapon *weaponCfg, bool primary);

	// Gravity
	void	fireGravity(t_weapon *weaponCfg, bool primary);

	// Other
	void	addPlayerVelocity(float *dirX, float *dirY);
	void	checkVelocity();

	// Cooldown
	float getCurrentCooldownPrimary(void);
	float getCurrentCooldownSeconday(void);

protected:
	float		_lastFirePrimary;
	float		_lastFireSecondary;
	float		_currentTime;

	std::shared_ptr<Bomb>		_bombOnHold;
	bool		_bombPrimary;
	bool		_lastFramePressed;

	bool		_shieldActivated;

	std::shared_ptr<Player>	_player;

	t_weapon	*_primary;
	t_weapon	*_primaryAlt;
	bool		_primaryAltUsed;

	t_weapon	*_secondary;
	t_weapon	*_secondaryAlt;
	bool		_secondaryAltUsed;

	friend class Player;
};

#endif

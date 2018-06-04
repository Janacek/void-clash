#ifndef		AWEAPON_HPP_
# define	AWEAPON_HPP_

#include	<iostream>
#include	"AObject.hpp"
#include	"ConfigParser.hpp"

class AWeapon : public AObject
{
public:
	AWeapon(void);
	AWeapon(eObjectType type, float X, float Y, float dirX, float dirY);
	virtual ~AWeapon(void);

	virtual void		init(t_weapon *weaponCfg);
	virtual bool		update();
	virtual int			getWeaponIndex();

	virtual bool		checkCollisionWithWalls(float &exploX, float &exploY);
	virtual bool		checkCollisionWithWalls();

	t_weapon	*getProperty();

protected:
	t_weapon	*_property;
	int			_index;
};

#endif
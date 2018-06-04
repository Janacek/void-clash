#include	"AWeapon.hpp"
#include	"Event.hpp"

extern t_config *G_conf;

AWeapon::AWeapon()
{}

AWeapon::AWeapon(eObjectType type, float X, float Y, float dirX, float dirY) :
	AObject(type, X, Y, dirX, dirY)
{
	_property = NULL;
}

AWeapon::~AWeapon(void)
{
	_property = NULL;
}

void AWeapon::init(t_weapon *weaponCfg)
{
	_property = weaponCfg;
	_radius = weaponCfg->size;

	// Get index
	for (unsigned int i = 0; i < G_conf->weapons->size(); i++)
	{
		if (G_conf->weapons->at(i) == weaponCfg)
			_index = i;
	}
}

bool	AWeapon::update()
{
	if (!_owner)
		return false;
	return true;
}
int	AWeapon::getWeaponIndex()
{
	return _index;
}

t_weapon	*AWeapon::getProperty()
{
	return _property;
}

// Check if a wall collision event has been raised by the physX engine for this object
bool	AWeapon::checkCollisionWithWalls(float &exploX, float &exploY)
{
	std::list<std::pair<eventType, s_event> >::const_iterator it;

	for (it = Event::getMainEventList()->begin(); it != Event::getMainEventList()->end(); ++it)
	{
		if (it->first == ev_WALL_COLLISION && it->second.trigger.get() == this)
		{
			exploX = getX();
			exploY = getY();
			return true;
		}
	}
	return false;
}

// Check if a wall collision event has been raised by the physX engine for this object
bool	AWeapon::checkCollisionWithWalls()
{
	std::list<std::pair<eventType, s_event> >::const_iterator it;

	for (it = Event::getMainEventList()->begin(); it != Event::getMainEventList()->end(); ++it)
	{
		if (it->first == ev_WALL_COLLISION && it->second.trigger.get() == this)
			return true;
	}
	return false;
}

#include	<iostream>
#include	"Rocket.hpp"
#include	"Wall.hpp"
#include	"Player.hpp"
#include	"Explosion.hpp"
#include	"Defines.h"
#include	"ConfigParser.hpp"
#include  "Event.hpp"

extern bool	G_isOffline;
extern bool	G_isServer;
extern t_config *G_conf;

Rocket::Rocket() :
AWeapon(ROCKET, 0, 0, 0, 0)
{
	_owner = NULL;
	_dir.first = 0;
	_dir.second = 0;
	_impact.first = 0;
	_impact.second = 0;
}

Rocket::Rocket(float X, float Y,
	float dirX, float dirY,
	float impactX, float impactY,
	const std::shared_ptr<Player> &owner) :
	AWeapon(ROCKET, X, Y, dirX, dirY)
{
	_owner = owner;
	_dir.first = dirX;
	_dir.second = dirY;
	_impact.first = impactX;
	_impact.second = impactY;
}

Rocket::~Rocket()
{
}

///////////////////////////////////////////////
/////   Called each frame

bool	Rocket::update()
{
	if (!AWeapon::update())
		return false;

	float	exploX = 0.0f;
	float	exploY = 0.0f;
	if (checkCollisionWithWalls(exploX, exploY) || checkImpact(exploX, exploY))
	{
		Explosion	*toPush = new Explosion(exploX, exploY, 0, 0, _owner, this->getType());
		toPush->init(_property);
		toPush->onStart();
		toPush->pushInMap();
		return false;
	}

	return (true);
}

bool	Rocket::checkImpact(float &exploX, float &exploY)
{
	int	count = 0;
	if ((int)_dir.first >= 0 && _pos.first >= _impact.first)
		count++;
	else if ((int)_dir.first <= 0 && _pos.first <= _impact.first)
		count++;

	if ((int)_dir.second >= 0 && _pos.second >= _impact.second)
		count++;
	else if ((int)_dir.second <= 0 && _pos.second <= _impact.second)
		count++;

	if (count == 2)
	{
		exploX = _impact.first;
		exploY = _impact.second;
		return true;
	}

	return false;
}

const std::pair<float, float>	&Rocket::getImpact(void) const
{
	return (_impact);
}
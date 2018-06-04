#include	"Bot.hpp"
#include	"Map.hpp"
#include	"ConfigParser.hpp"

extern t_config	*G_conf;
extern bool	G_isServer;
extern bool	G_isOffline;

Bot::Bot(float X, float Y,
	float dirX, float dirY) :
	AObject(BOT, X, Y, dirX, dirY)
{
	_clock.restart();
	_target = NULL;
	_life = G_conf->horde->life;
	_radius = G_conf->horde->size;
}

Bot::Bot()
{
	_type = BOT;
	_clock.restart();
	_target = NULL;
	_life = G_conf->horde->life;
	_radius = G_conf->horde->size;
}

Bot::~Bot()
{}

bool	Bot::update()
{
	if (_clock.getElapsedTime().asMilliseconds() > G_conf->horde->depopTime * 1000)
		return false;

	if (checkHitPlayers())
		return false;

	_target = lockTarget();
	move();
	return true;
}

///////////////////////////////////////////////
/////   Hit related functions

bool	Bot::checkHitPlayers()
{
	auto	it = S_Map->getPlayers()->begin();
	auto	end = S_Map->getPlayers()->end();

	while (it != end)
	{
		Player *player = (*it).get();
		if (player->getTeam() != 0 && !player->isRespawning())
		{
			if (isInsideHitbox(player->getX(), player->getY(),
				player->getRadius() + _radius) == true)
			{
				if (G_isServer || G_isOffline)
				{
					player->isHitBy(std::dynamic_pointer_cast<AWeapon>(shared_from_this()), G_conf->horde->damage);
					return (true);
				}
			}
		}
		++it;
	}
	return (false);
}

void	Bot::move()
{
	if (!_target)
	{
		_dir.first = 0.f;
		_dir.second = 0.f;
		return;
	}

	float dist = std::sqrt(std::pow(_target->getX() - _pos.first, 2) + std::pow(_target->getY() - _pos.second, 2));

	// Standard cap speed when accelerating

	_dir.first += (_target->getX() - getX()) / dist * G_conf->horde->acceleration * _coefDeltaTime;
	_dir.second += (_target->getY() - getY()) / dist * G_conf->horde->acceleration * _coefDeltaTime;
	float curSpeed = std::sqrt(std::pow(_dir.first, 2.0f) + std::pow(_dir.second, 2.0f));
	if (curSpeed > G_conf->horde->speed)
	{
		_dir.first /= curSpeed;
		_dir.first *= G_conf->horde->speed;
		_dir.second /= curSpeed;
		_dir.second *= G_conf->horde->speed;
	}
	//_dir.first = (_target->getX() - _pos.first) / dist * G_conf->horde->speed;
	//_dir.second = (_target->getY() - _pos.second) / dist * G_conf->horde->speed;
}

AObject		*Bot::lockTarget()
{
	auto	it = S_Map->getElems()->begin();
	auto	end = S_Map->getElems()->end();
	std::shared_ptr<AObject>target = NULL;
	float	closestDist = 99999999999999999.f;

	while (it != end)
	{
		if ((*it)->getType() == PLAYER)
		{
			std::shared_ptr<Player>	player = std::dynamic_pointer_cast<Player>(*it);
			if (!player->isRespawning() && player->getTeam() != 0)
			{
				float dist = checkDistance((*it).get());
				if (dist < closestDist)
				{
					closestDist = dist;
					target = *it;
				}
			}
		}
		++it;
	}
	return target.get();
}

bool	Bot::isInsideHitbox(int X, int Y, float radius)
{
	if ((std::pow(X - getX(), 2.0f) + std::pow(Y - getY(), 2.0f))
		< (std::pow(radius, 2.0f)))
		return (true);
	return (false);
}

///////////////////////////////////////////////
/////   Hit

void	Bot::isHitBy(std::shared_ptr<AWeapon> hitter, float dmg)
{
	if (_life <= 0.f)
		return;

	if (hitter && hitter->getProperty())
		applySlow(hitter->getProperty());

	_life -= dmg;
	ADD_EVENT(ev_BOT_HIT, s_event(hitter, (this)));
	if (_life <= 0)
	{
		ADD_EVENT(ev_BOT_DESTROYED, s_event(hitter, this));
		ADD_EVENT(ev_DELETE, s_event(shared_from_this()));

		// Check for deleted bots as turret target
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

float	Bot::checkDistance(AObject *target)
{
	return (std::pow(target->getX() - _pos.first, 2) + std::pow(target->getY() - _pos.second, 2));
}

///////////////////////////////////////////////
/////   Getterz 

sf::Int16	Bot::getLife(void) const
{
	return _life;
}
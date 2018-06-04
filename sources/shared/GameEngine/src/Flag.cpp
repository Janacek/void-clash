//
// Flag.cpp for Void Clash in /home/gegon/pfa/sources/client/Common/src
// 
// Made by Gegon
// Login   <gegon@epitech.net>
// 
// Started on  Fri Dec 13 21:38:54 2013 Gegon
// Last update Fri Feb  7 06:05:56 2014 Gegon
//

#include	<cmath>
#include	<iostream>
#include	"Flag.hpp"
#include	"Map.hpp"
#include	"Respawn.hpp"
#include	"ConfigParser.hpp"

extern t_config	*G_conf;
extern bool	G_isServer;
extern bool G_isOffline;

Flag::Flag(float X, float Y,
	float dirX, float dirY, sf::Uint16 team) :
	AObject(FLAG, X, Y, dirX, dirY),
	_initPos(std::pair<float, float>(X, Y))
{
	_radius = 100;
	_team = team;
	_clock.restart();
	_time = 0.0f;
	_owner = NULL;
	_flagAtSpawn = true;
}

Flag::Flag() :
AObject(FLAG, 0, 0, 0, 0),
_initPos(std::pair<float, float>(0.0f, 0.0f))

{
	_radius = 100;
	_team = 0;
	_clock.restart();
	_time = 0.0f;
	_owner = NULL;
	_flagAtSpawn = true;
}

Flag::~Flag()
{
}

///////////////////////////////////////////////
/////   Update

bool		Flag::update()
{
	if (!S_Map->getMode()->getProperty()->flag)
		return true;

	if (checkTimer())
		reset();

	if (checkCapture() && _owner)
	{
			// +1 to team
		if (_owner->getTeam() == 1)
				S_Map->incScore(1, 0);
		if (_owner->getTeam() == 2)
				S_Map->incScore(0, 1);

		_owner->setScore(_owner->getScore() + 5);
		ADD_EVENT(ev_CAPTURE_FLAG, s_event(shared_from_this(), _owner.get()));
		reset(true);
	}
	checkIfPlayerTook();
	if (checkIfOwnerDisconnected())
		_owner = NULL;
	if (_owner)
	{
		setPosition(_owner->getX(), _owner->getY());
		if (_owner->getTeam() == 0)
			reset();
	}
	// CHECK IF THROW FLAG
	return true;
}

bool	Flag::checkTimer()
{
	if ((_initPos.first == _pos.first &&
		_initPos.second == _pos.second) ||
		_owner)
		_clock.restart();
	else
		_time = _clock.getElapsedTime().asSeconds();

	if (_time > DELAY_RESPAWN_FLAG)
		return true;
	return false;
}

bool	Flag::checkIfOwnerDisconnected()
{
	if (_owner)
	{
		if (Event::getEventByType(ev_DELETE))
		{
			std::list<std::pair<eventType, s_event> >*	events = Event::getEventByType(ev_DELETE);
			std::list<std::pair<eventType, s_event> >::const_iterator it = events->begin();
			std::list<std::pair<eventType, s_event> >::const_iterator end = events->end();
			while (it != end)
			{
				if ((*it).second.trigger == _owner)
					return true;
				++it;
			}
		}
	}
	return false;
}

bool	Flag::checkCapture()
{
	if (!(G_isServer || G_isOffline))
		return false;

	auto	it = S_Map->getElems()->begin();
	auto	end = S_Map->getElems()->end();

	while (it != end)
	{
		if ((*it)->getType() == RESPAWN)
		{
			Respawn *respawn = dynamic_cast<Respawn *>((*it).get());
			if (respawn->getTeam() != 0 &&
				_team != 0 &&
				respawn->getTeam() != _team &&
				respawn->isInsideRespawn(_pos.first, _pos.second))
				return true;
		}
		it++;
	}
	return false;
}

void	Flag::checkIfPlayerTook()
{
	auto	it = S_Map->getPlayers()->begin();
	auto	end = S_Map->getPlayers()->end();

	if (!(_owner))
	{
		while (it != end)
		{
			if ((*it)->getTeam() != _team && (*it)->isRespawning() == false)
			{
				if ((std::pow(_pos.first - (*it)->getX(), 2.0f) + std::pow(_pos.second - (*it)->getY(), 2.0f))
					<= 2 * (std::pow((*it)->getRadius(), 2.0f) + std::pow(_radius, 2.0f)))
				{
					_clock.restart();
					_time = 0.0f;
					_owner = *it;
					_owner->setFlag(std::dynamic_pointer_cast<Flag>(shared_from_this()));
					ADD_EVENT(ev_TOUCH_FLAG, s_event(shared_from_this(), _owner.get()));
					_flagAtSpawn = false;
				}
			}
			it++;
		}
	}
}
void	Flag::resetOwner()
{
	_owner = NULL;
}

void	Flag::reset(bool afterCapture)
{
	// Don't send ev_RESPAWN_FLAG when ev_CAPTURE_FLAG (In Game Messages)
	if (afterCapture != true)
		ADD_EVENT(ev_RESPAWN_FLAG, s_event(shared_from_this()));
	
	_clock.restart();
	_time = 0.0f;
	_pos.first = _initPos.first;
	_pos.second = _initPos.second;
	_dir.first = 0.0f;
	_dir.second = 0.0f;
	if (_owner)
		_owner->setFlag(NULL);
	resetOwner();

	_flagAtSpawn = true;
}

void		Flag::setOwner(std::shared_ptr<Player>	p)
{
	_owner = p;
}

std::shared_ptr<Player>	Flag::getOwner()
{
	return _owner;
}

const float&	Flag::getInitX() const
{
	return _initPos.first;
}

const float&	Flag::getInitY() const
{
	return _initPos.second;
}

bool Flag::flagAtSpawn(void)
{
	return _flagAtSpawn;
}

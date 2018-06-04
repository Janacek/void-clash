#include	<algorithm>
#include	<cstdlib>
#include	<cstring>
#include	"AI.hpp"
#include	"Player.hpp"
#include	"Map.hpp"
#include	"config.hpp"
#include	"Flag.hpp"
#include	"Capture.hpp"
#include	"Respawn.hpp"

extern t_config *G_conf;

AI::AI(std::shared_ptr<Player> player)
{
	_player = player;
	_target = NULL;
	_goToGoal = false;
	selectWeapons();
	//_player->setWeapons(G_conf->weapons->at(std::rand() % G_conf->weapons->size()), NULL, NULL, NULL);
}

void	AI::selectWeapons()
{
	int count = 0;
	t_weapon *primary = nullptr;
	t_weapon *secondary = nullptr;
	// Main weapon (bullet only)
	for (const auto& weapon : *G_conf->weapons)
	{
		if (weapon->category == "BULLET")
			++count;
	}
	int randRes = std::rand() % count;
	count = 0;
	for (const auto& weapon : *G_conf->weapons)
	{
		if (weapon->category == "BULLET")
		{
			if (count == randRes)
				primary = weapon;
			++count;
		}
	}
	// Secondary weapon (All except bullet)
	count = 0;
	for (const auto& weapon : *G_conf->weapons)
	{
		if (weapon->category != "BULLET")
			++count;
	}
	randRes = std::rand() % count;
	count = 0;
	for (const auto& weapon : *G_conf->weapons)
	{
		if (weapon->category != "BULLET")
		{
			if (count == randRes)
				secondary = weapon;
			++count;
		}
	}
	_player->setWeapons(primary, NULL, secondary, NULL);
}

AI::~AI(void)
{}

bool	AI::update(void)
{
	std::memset(&(_player->_actions), 0, sizeof(_player->_actions));
	if (_player->isRespawning() || !S_Map->isMapActive())
		return true;

	// Find the most interesting point
	_target = findTarget();
	if (!_target)
		return false;

	if (!checkSegmentSegment(_target)) // If no wall with target
	{
		//if (_target->getType() == RESPAWN)
		//{
		//	std::shared_ptr<Respawn> r = std::dynamic_pointer_cast<Respawn>(_target);
		//	setMoveInputs(r->getX() + r->getWidth() / 2, r->getY() + r->getHeight() / 2);
		//}
		//else
			setMoveInputs(_target->getX(), _target->getY());
	}
	else
	{
		// INDIRECT
		moveIndirect();
		setMoveInputs(_goal.first, _goal.second);
	}
	setShootInputs();
	//std::cout << _player->_actions.moveY << std::endl;
	return true;
}

// If target in sight -> go target
// Check if other target in sight closest

// If no target in sight get closest ennemy and raycast on 180�

std::shared_ptr<AObject>	AI::findTarget()
{
	////// check if current target is still in sight
	//if (_target && _target->getType() == PLAYER && checkSegmentSegment(_target)) {
	//	_goToGoal = true;
	//	_goal.first = _target->getPrevFramePosition().first;
	//	_goal.second = _target->getPrevFramePosition().second;
	//	return NULL;
	//}

	std::shared_ptr<AObject>	ret = NULL;
	float	distance = 99999999.f;
	float	closestDistance = 90000000.f;
	auto	it = S_Map->getElems()->begin();
	auto	end = S_Map->getElems()->end();
	while (it != end)
	{
		if ((*it)->getType() == PLAYER)
		{
			std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(*it);
			if (player && player != _player)
			{
				if (player->getTeam() != 0 && !player->isRespawning() &&
					(!S_Map->getMode()->getProperty()->team || player->getTeam() != _player->getTeam()))
				{
					checkDistanceAndCollision(*it, distance);
					if (distance < closestDistance)
					{
						closestDistance = distance;
						ret = *it;
					}
				}
			}
		}
		if ((*it)->getType() == FLAG)
		{
			std::shared_ptr<Flag> flag = std::dynamic_pointer_cast<Flag>(*it);
			if (flag && S_Map->getMode()->getProperty()->flag && flag->getTeam() != _player->getTeam() && flag != _player->getFlag())
			{
				checkDistanceAndCollision(*it, distance);
				//if (distance < closestDistance)
				//{
				//	closestDistance = distance;
				//	ret = *it;
				//}
				return flag;
			}
		}
		if ((*it)->getType() == RESPAWN)
		{
			std::shared_ptr<Respawn> r = std::dynamic_pointer_cast<Respawn>(*it);
			if (r && S_Map->getMode()->getProperty()->flag && _player->getFlag() && r->getTeam() == _player->getTeam())
				return r;
		}
		++it;
	}
	return ret;
}

void	AI::setShootInputs()
{
	auto	it = S_Map->getPlayers()->begin();
	auto	end = S_Map->getPlayers()->end();
	while (it != end)
	{
		std::shared_ptr<Player> p = (*it);
		if (p && p != _player)
		{
			if (checkDistance(p) < AI_DISTANCE_TO_SHOOT && 	!checkSegmentSegment(p))
			{
				_player->_actions.aimX = p->getX() - p->getDirX() / 6 + std::rand() % AI_SHOOT_INACCURACY - AI_SHOOT_INACCURACY / 2;
				_player->_actions.aimY = p->getY() - p->getDirY() / 6 + std::rand() % AI_SHOOT_INACCURACY - AI_SHOOT_INACCURACY / 2;
				_player->_actions.primary = true;
				if (checkDistance(p) < AI_DISTANCE_TO_SHOOT / 2)
					_player->_actions.secondary = true;
			}
		}
		++it;
	}
}

void	AI::setMoveInputs(float goalX, float goalY)
{
	// PLAYER
	int resX = goalX - _player->getX();
	int resY = goalY - _player->getY();
	int distance = std::sqrt(std::pow(resX, 2) + std::pow(resY, 2));
	if (_target && _target->getType() == PLAYER && distance < 1000 && !_isTooClose)
	{
		_dirX = 100;
		_dirY = 100;
		int randRes = std::rand() % 4;
		if (randRes == 0)
		{
			_dirX = -100;
			_dirY = -100;
		}
		else if (randRes == 1)
		{
			_dirX = 100;
			_dirY = -100;
		}
		else if (randRes == 2)
		{
			_dirX = -100;
			_dirY = 100;
		}
		_isTooClose = true;
		_tooCloseTime = _player->_clock.getElapsedTime();
	}
	else if (distance > 1100 || _player->_clock.getElapsedTime() - _tooCloseTime > sf::seconds(1))
		_isTooClose = false;

	if (_isTooClose)
	{
		resX = _dirX;
		resY = _dirY;
	}
	if (std::abs(resX) > std::abs(resY) * AI_LINEAR_MOVE_FACTOR)
		resY = 0.f;
	else if (std::abs(resY) > std::abs(resX) * AI_LINEAR_MOVE_FACTOR)
		resX = 0.f;
	resX = std::min(100, std::max(-100, resX));
	resY = std::min(100, std::max(-100, resY));
	_player->_actions.moveX = resX;
	_player->_actions.moveY = resY;
	// Collision on previous frame
	for (auto it = Event::getMainEventList()->begin(); it != Event::getMainEventList()->end(); ++it)
	{
		if (it->first == ev_WALL_COLLISION && it->second.trigger == _player)
		{
			sf::Int16 tmp = _player->_actions.moveX;
			_player->_actions.moveX = _player->_actions.moveY;
			_player->_actions.moveY = tmp;
		}
	}
}

void	AI::moveIndirect()
{
	if (_goToGoal)
	{
		// Unstuck
		float	dirX = _goal.first - _player->getX();
		float	dirY = _goal.second - _player->getY();
		int		distDir = std::sqrt(std::pow(_goal.first - _player->getX(), 2) + std::pow(_goal.second - _player->getY(), 2));
		dirX /= distDir;
		dirY /= distDir;

		float playerDirX = _player->getDirX();
		float playerDirY = _player->getDirY();
		int		distDirPlayer = std::sqrt(std::pow(playerDirX, 2) + std::pow(playerDirY, 2));
		float angle = std::atan2(playerDirY - dirY, playerDirX - dirX);
		if (std::abs(angle) > AI_UNSTUCK_ANGLE)
		{
			_goToGoal = false;
			return;
		}
		float	angleInRadians = atan2(dirY, dirX);
		float	goalX = _player->_pos.first + std::cos(angleInRadians) * _indirectDist;
		float	goalY = _player->_pos.second + std::sin(angleInRadians) * _indirectDist;
		if (checkSegmentSegment(_player->getX(), _player->getY(), goalX, goalY))
		{
			//std::cout << "WALL AHEAD" << std::endl;
			_goToGoal = false;
		}
		else
		{
			setMoveInputs(_goal.first, _goal.second);
			return;
		}
	}

	_goToGoal = true;
	int threshold = 100000;
	float maxDistance = 0.f;
	float	dirX = _target->getX() - _player->getX();
	float	dirY = _target->getY() - _player->getY();
	for (int i = 0; i < AI_RAYCAST_NB; i++)
	{
		int spread = (i - AI_RAYCAST_NB / 2) * AI_RAYCAST_ANGLE;
		float angleInRadians = std::atan2(dirY, dirX) + spread / 180.f * 3.14f;
		float	goalX = _player->_pos.first + std::cos(angleInRadians) * threshold;
		float	goalY = _player->_pos.second + std::sin(angleInRadians) * threshold;
		checkSegmentSegment(_player->getX(), _player->getY(), goalX, goalY);

		float distance = std::sqrt(checkDistance(_raycastedPoint.first, _raycastedPoint.second)) + std::rand() % AI_RANDOM_DISTANCE;
		if (distance > maxDistance)
		{
			maxDistance = distance;
			_goal.first = _raycastedPoint.first;
			_goal.second = _raycastedPoint.second;
		}
	}
	_indirectDist = AI_INDIRECT_COLLISION_DIST + std::rand() % AI_RANDOM_INDIRECT_COLLISION_DIST;
}

///////////////////////////////////////
// Collision with walls functions
bool	AI::checkSegmentSegment(const std::shared_ptr<AObject> &nmy)
{
	Point A;
	Point B;
	Point O;
	Point P;

	O.x = _player->getPosition().first;
	O.y = _player->getPosition().second;
	P.x = nmy->getX();
	P.y = nmy->getY();

	auto	it = S_Map->getWalls()->begin();
	auto	end = S_Map->getWalls()->end();
	while (it != end)
	{
		float dist = std::sqrt(std::pow((*it)->getEndX() - (*it)->getX(), 2) + std::pow((*it)->getEndY() - (*it)->getY(), 2));
		if (dist == 0)
			dist = 1;
		float radius = (*it)->getRadius() + _player->getRadius() * 2;

		A.x = (*it)->getX() - std::abs((*it)->getEndX() - (*it)->getX()) / dist * radius;
		B.x = (*it)->getEndX() - std::abs((*it)->getEndX() - (*it)->getX()) / dist * radius;
		A.y = (*it)->getY() - std::abs((*it)->getEndY() - (*it)->getY()) / dist * radius;
		B.y = (*it)->getEndY() - std::abs((*it)->getEndY() - (*it)->getY()) / dist * radius;

		if (CollisionSegSeg(A, B, O, P))
			return true;

		A.x = (*it)->getX() + std::abs((*it)->getEndX() - (*it)->getX()) / dist * radius;
		B.x = (*it)->getEndX() + std::abs((*it)->getEndX() - (*it)->getX()) / dist * radius;
		A.y = (*it)->getY() + std::abs((*it)->getEndY() - (*it)->getY()) / dist * radius;
		B.y = (*it)->getEndY() + std::abs((*it)->getEndY() - (*it)->getY()) / dist * radius;

		if (CollisionSegSeg(A, B, O, P))
			return true;

		A.x = (*it)->getX();
		B.x = (*it)->getEndX();
		A.y = (*it)->getY();
		B.y = (*it)->getEndY();

		if (CollisionSegSeg(A, B, O, P))
			return true;
		++it;
	}
	return false;
}

bool	AI::checkSegmentSegment(float startX, float startY, float goalX, float goalY)
{
	Point A;
	Point B;
	Point O;
	Point P;

	O.x = startX;
	O.y = startY;
	P.x = goalX;
	P.y = goalY;

	float recordDistance = 9999999999999.f;
	auto	it = S_Map->getWalls()->begin();
	auto	end = S_Map->getWalls()->end();
	float resX = 0;
	float resY = 0;
	bool collisionDetected = false;
	while (it != end)
	{
		float dist = std::sqrt(std::pow((*it)->getEndX() - (*it)->getX(), 2) + std::pow((*it)->getEndY() - (*it)->getY(), 2));
		if (dist == 0)
			dist = 1;
		int radius = (*it)->getRadius() + _player->getRadius();

		A.x = (*it)->getX() - ((*it)->getEndX() - (*it)->getX()) / dist * radius;
		B.x = (*it)->getEndX() + ((*it)->getEndX() - (*it)->getX()) / dist * radius;
		A.y = (*it)->getY() - ((*it)->getEndY() - (*it)->getY()) / dist * radius;
		B.y = (*it)->getEndY() + ((*it)->getEndY() - (*it)->getY()) / dist * radius;

		if (CollisionSegSeg(A, B, O, P))
		{
			collisionDetected = true;
			float distance = checkDistance(_raycastedPoint.first, _raycastedPoint.second);
			if (distance < recordDistance)
			{
				resX = _raycastedPoint.first;
				resY = _raycastedPoint.second;
				recordDistance = distance;
			}
		}
		++it;
	}
	_raycastedPoint.first = resX;
	_raycastedPoint.second = resY;
	return collisionDetected;
}


bool	AI::CollisionSegSeg(Point A, Point B, Point O, Point P)
{
	//if (CollisionDroiteSeg(A, B, O, P) == false)
	//	return false;  // inutile d'aller plus loin si le segment [OP] ne touche pas la droite (AB)
	//if (CollisionDroiteSeg(O, P, A, B) == false)
	//	return false;
	////Vecteur u, AC;
	////u.x = B.x - A.x;
	////u.y = B.y - A.y;
	////AC.x = P.x - A.x;
	////AC.y = P.y - A.y;
	////float ti = (u.x*AC.x + u.y*AC.y) / (u.x*u.x + u.y*u.y);
	////_raycastedPoint.first = A.x + ti*u.x;
	////_raycastedPoint.second = A.y + ti*u.y;
	//_raycastedPoint.first = P.x;
	//_raycastedPoint.second = P.y;
	//return true;

	if (CollisionDroiteSeg(A, B, O, P) == false)
		return false;  // inutile d'aller plus loin si le segment [OP] ne touche pas la droite (AB)
	Vecteur AB, OP;
	AB.x = B.x - A.x;
	AB.y = B.y - A.y;
	OP.x = P.x - O.x;
	OP.y = P.y - O.y;
	float k = -(A.x*OP.y - O.x*OP.y - OP.x*A.y + OP.x*O.y) / (AB.x*OP.y - AB.y*OP.x);
	if (k < 0 || k>1)
		return false;
	//_raycastedPoint.first = P.x;
	//_raycastedPoint.second = P.y;
	_raycastedPoint.first = B.x * k + A.x * (1 - k);
	_raycastedPoint.second = B.y * k + A.y * (1 - k);
	//std::cout << "Player " << _player->getX() << "   -   " << _player->getY() << std::endl;
	//std::cout << "Ray1 " << _raycastedPoint.first << "   -   " << _raycastedPoint.second << std::endl;
	//_raycastedPoint.first = (_raycastedPoint.first * 99 + O.x) / 100;
	//_raycastedPoint.second = (_raycastedPoint.second * 99 + O.y) / 100;
	//std::cout << "Ray2 " << _raycastedPoint.first << "   -   " << _raycastedPoint.second << std::endl;
	return true;
}

bool	AI::CollisionDroiteSeg(Point A, Point B, Point O, Point P)
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

float	AI::checkDistance(const std::shared_ptr<AObject>& target)
{
	return std::sqrt(std::pow(target->getX() - _player->getPosition().first, 2) + std::pow(target->getY() - _player->getPosition().second, 2));
}

float	AI::checkDistance(float X, float Y)
{
	return std::sqrt((std::pow(X - _player->getPosition().first, 2) + std::pow(Y - _player->getPosition().second, 2)));
}

// Ret false if collision with wall
// Else fill distance and ret true
bool	AI::checkDistanceAndCollision(const std::shared_ptr<AObject>& obj, float &distance)
{
	distance = checkDistance(obj);
	if (!checkSegmentSegment((obj)))
	{
		if (obj == _target)
			distance -= AI_TARGET_LOCK_DIST_BONUS;
		return true;
	}
	distance += std::rand() % AI_TARGET_DIRECT_DIST_BONUS;
	return false;
}

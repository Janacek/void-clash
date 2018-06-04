//
// PhysicEngine.cpp for pfa in /home/volent/dev/pfa/client
// 
// Made by florent espanet
// Login   <espane_f@epitech.net>
// 
// Started on  Thu Oct  3 14:38:51 2013 florent espanet
// Last update Tue Feb 25 19:40:55 2014 Gegon
//

#include <cmath>
#include <glm/gtx/transform.hpp>
#include "PhysicEngine.hpp"
#include "ConfigParser.hpp"
#include "Map.hpp"

extern t_config *G_conf;

PhysicEngine	*GPhysicEngine;

void	simulateUpdatePhysObject(const std::shared_ptr<AObject> &obj)
{
	GPhysicEngine->setDelta(S_Map->getCurrentPlayer()->getLatency() / 4.0f / 20000.0f / G_conf->game->speed);

	if (GPhysicEngine->updatePhysObject(obj, 0))
		obj->setPosition(obj->getX() + obj->getDirX() * GPhysicEngine->getDelta(),
		obj->getY() + obj->getDirY() * GPhysicEngine->getDelta());

}

//////////////////////////////////////////////////////////////////////

PhysicEngine::PhysicEngine() :
_delta(0.0)
{
}

PhysicEngine::~PhysicEngine()
{
}

//////////////////////////////////////////////////////////////////////

void		PhysicEngine::start(void)
{
	GPhysicEngine = this;
}

void		PhysicEngine::stop(void)
{
	GPhysicEngine = NULL;
}

eGameState	PhysicEngine::update(const sf::Time &deltaTime)
{
	if (G_conf == NULL)
		return RUN;

	_delta = deltaTime.asMicroseconds() / 20000.0f / G_conf->game->speed;
	if (_delta > 1.f)
		_delta = 1.f;
	for (std::list<std::shared_ptr<AObject>>::iterator it = S_Map->getElems()->begin(); it != S_Map->getElems()->end(); ++it)
	{
		regenerateOptiWalls(*it);

		AWeapon *weapon = dynamic_cast<AWeapon *>((*it).get());
		if (weapon || (*it)->getType() == PLAYER || (*it)->getType() == BOT)
		{
			if (updatePhysObject(*it, 0))
			{
				(*it)->setPrevFramePosition((*it)->getX(), (*it)->getY());
				(*it)->setPosition((*it)->getX() + (*it)->getDirX() * _delta * (100 - (*it)->getSlow()) / 100,
					(*it)->getY() + (*it)->getDirY() * _delta * (100 - (*it)->getSlow()) / 100);
			}
			else if (weapon)
				ADD_EVENT(ev_DELETE, s_event(*it));
		}
	}
	return RUN;
}

///////////////////////////////////////////////
/////   Optimizing func used to check only walls close to object

void	PhysicEngine::regenerateOptiWalls(const std::shared_ptr<AObject> & obj)
{
	_optiWalls.clear();

	auto	it = S_Map->getWalls()->begin();
	auto	end = S_Map->getWalls()->end();
	float speed = sqrt(obj->getDirX() * obj->getDirX() + obj->getDirY() * obj->getDirY()) * _delta + 1.f;
	while (it != end)
	{
		std::shared_ptr<Wall>wall = (*it);
		int margin = obj->getRadius() + wall->getRadius() + speed;
		// Set X walls
		int minX = wall->getX() - margin;
		int maxX = wall->getEndX() + margin;
		if (wall->getEndX() < wall->getX())
		{
			minX = wall->getEndX() - margin;
			maxX = wall->getX() + margin;
		}
		// Set Y walls
		int minY = wall->getY() - margin;
		int maxY = wall->getEndY() + margin;
		if (wall->getEndY() < wall->getY())
		{
			minY = wall->getEndY() - margin;
			maxY = wall->getY() + margin;
		}

		// Fill optiWalls when obj is within rectangle
		if (obj->getX() > minX && obj->getX() < maxX &&
			obj->getY() > minY && obj->getY() < maxY)
			_optiWalls.push_back(wall);
		++it;
	}
}


//---------------------------------//
// Recursive function to check if bounce doesn't stuck

// If still stuck at level 2 -> invert direction (happens mostly when hit wall extremity)
// emergency unstuck at level 3 -> lower direction (bounce redirect to another wall)
// At level 4 object will be deleted

bool	PhysicEngine::initUpdatePhysObject(const std::shared_ptr<AObject> & obj)
{
	if (_optiWalls.empty())
		return true;

	return updatePhysObject(obj, 0);
}

bool	PhysicEngine::updatePhysObject(const std::shared_ptr<AObject> &obj, int level)
{
	if (std::pow(obj->getDirX(), 2.0f) + std::pow(obj->getDirY(), 2.0f) < 0.001f)
		return true;

	AWeapon *weapon = dynamic_cast<AWeapon *>(obj.get());
	if (weapon && weapon->getProperty()->collide_walls == false)
		return true;

	if (obj->getTimeSinceCreation() == 0.f && checkSegmentCircleCurrentPos(obj))
		return false;

	if (level == 2)		// Invert direction
	{
		obj->setDirX(-obj->getDirX());
		obj->setDirY(-obj->getDirY());
	}
	if (level == 3)		// Set at previous frame position
		obj->setPosition(obj->getPrevFramePosition().first, obj->getPrevFramePosition().second);
	if (level == 4)		// Delete object
		return false;

	if (detectCollision(obj))
	{
		level++;
		return updatePhysObject(obj, level);
	}

	return true;
}

//---------------------------------//

bool	PhysicEngine::detectCollision(const std::shared_ptr<AObject> &obj)
{
	if (checkSegmentCircle(obj))
		return true;
	if (checkSegmentSegment(obj))
		return true;



	// check segment - segment on both extremities of the objet
	// |O|
	return (false);
}

///////////////////////////////////////////////
/////   Apply bounce

void	PhysicEngine::bounce(const std::shared_ptr<AObject> &obj, const std::shared_ptr<Wall> &wall)
{
	Point A;
	Point B;

	A.x = wall->getX();
	A.y = wall->getY();
	B.x = wall->getEndX();
	B.y = wall->getEndY();
	Vecteur normale = GetNormale(A, B, wall->getLen());
	Vecteur direction;
	direction.x = obj->getDirX() * _delta;
	direction.y = obj->getDirY() * _delta;
	Vecteur vAngle = CalculerVecteurV2(direction, normale);
	float angle = std::atan2(vAngle.y, vAngle.x);

	float speed = sqrt(std::pow(obj->getDirX(), 2.0f) + std::pow(obj->getDirY(), 2.0f));
	obj->setDirX(speed * std::cos(angle));
	obj->setDirY(speed * std::sin(angle));
}

Vecteur PhysicEngine::GetNormale(Point A, Point B, int len)
{
	glm::mat4 transform = glm::rotate(90.0f, glm::vec3(0, 0, 1));
	glm::vec4 res = transform * glm::vec4(A.x - B.x, A.y - B.y, 0.0f, 1.0f);

	res.x = res.x / len;
	res.y = res.y / len;
	if (std::abs(res.x) < 0.01f)
		res.x = 0.0f;
	if (std::abs(res.y) < 0.01f)
		res.y = 0.0f;

	Vecteur normale;
	normale.x = res.x;
	normale.y = res.y;
	return (normale);
}

Vecteur PhysicEngine::CalculerVecteurV2(Vecteur v, Vecteur N)
{
	Vecteur v2;
	float pscal = (v.x*N.x + v.y*N.y);
	v2.x = v.x - 2 * pscal*N.x;
	v2.y = v.y - 2 * pscal*N.y;
	return v2;
}


///////////////////////////////////////////////
/////   Segment Circle related funcs

bool	PhysicEngine::checkSegmentCircleCurrentPos(const std::shared_ptr<AObject> &obj)
{
	Point A;
	Point B;
	Cercle C;

	C.x = obj->getX();
	C.y = obj->getY();
	auto	it = _optiWalls.begin();
	auto	end = _optiWalls.end();
	while (it != end)
	{
		A.x = (*it)->getX();
		A.y = (*it)->getY();
		B.x = (*it)->getEndX();
		B.y = (*it)->getEndY();
		C.rayon = obj->getRadius() + (*it)->getRadius();
		if (CollisionSegment(A, B, C, obj, *it))
		{
			bounce(obj, *it);
			return true;
		}
		++it;
	}
	return false;
}

bool	PhysicEngine::checkSegmentCircle(const std::shared_ptr<AObject> &obj)
{
	Point A;
	Point B;
	Cercle C;

	C.x = obj->getX() + obj->getDirX() * _delta;
	C.y = obj->getY() + obj->getDirY() * _delta;
	auto	it = _optiWalls.begin();
	auto	end = _optiWalls.end();
	while (it != end)
	{
		A.x = (*it)->getX();
		A.y = (*it)->getY();
		B.x = (*it)->getEndX();
		B.y = (*it)->getEndY();
		C.rayon = obj->getRadius();
		if (CollisionSegment(A, B, C, obj, *it))
		{
			bounce(obj, *it);
			return true;
		}
		++it;
	}
	return false;
}

bool PhysicEngine::CollisionSegment(Point A, Point B, Cercle C, const std::shared_ptr<AObject> &obj, const std::shared_ptr<Wall> &wall)
{
	if (CollisionDroite(A, B, C) == false)
		return false;  // si on ne touche pas la droite, on ne touchera jamais le segment
	Vecteur AB, AC, BC;
	AB.x = B.x - A.x;
	AB.y = B.y - A.y;
	AC.x = C.x - A.x;
	AC.y = C.y - A.y;
	BC.x = C.x - B.x;
	BC.y = C.y - B.y;
	float pscal1 = AB.x*AC.x + AB.y*AC.y;  // produit scalaire
	float pscal2 = (-AB.x)*BC.x + (-AB.y)*BC.y;  // produit scalaire
	if (pscal1 >= 0.0f && pscal2 >= 0.0f)
	{
		// Going to the impact point
		Point CenterCircle;
		CenterCircle.x = obj->getX();
		CenterCircle.y = obj->getY();
		Point impact = ProjectionI(A, B, CenterCircle);
		t_impact *toSend = new t_impact;
		toSend->wall = wall;
		toSend->pos = std::pair<float, float>(impact.x, impact.y);
		ADD_EVENT(ev_WALL_COLLISION, s_event(obj, toSend));

		float diff = std::sqrt(std::pow(obj->getX() - impact.x, 2) + std::pow(obj->getY() - impact.y, 2)) - C.rayon;
		float speed = std::sqrt(std::pow(obj->getDirX(), 2) + std::pow(obj->getDirY(), 2));
		if (speed == 0.f)
			speed = 1.f;
		obj->setPosition(obj->getX() + obj->getDirX() / speed * diff,
			obj->getY() + obj->getDirY() / speed * diff);
		return true;   // I entre A et B, ok.
	}
	// dernière possibilité, A ou B dans le cercle
	if (CollisionPointCercle(A, C))
	{
		t_impact *toSend = new t_impact;
		toSend->wall = wall;
		toSend->pos = std::pair<float, float>(A.x, A.y);
		ADD_EVENT(ev_WALL_COLLISION, s_event(obj, toSend));
		return true;
	}
	if (CollisionPointCercle(B, C))
	{
		t_impact *toSend = new t_impact;
		toSend->wall = wall;
		toSend->pos = std::pair<float, float>(B.x, B.y);
		ADD_EVENT(ev_WALL_COLLISION, s_event(obj, toSend));
		return true;
	}
	return false;
}

Point PhysicEngine::ProjectionI(Point A, Point B, Point C)
{
	Vecteur u, AC;
	u.x = B.x - A.x;
	u.y = B.y - A.y;
	AC.x = C.x - A.x;
	AC.y = C.y - A.y;
	float ti = (u.x*AC.x + u.y*AC.y) / (u.x*u.x + u.y*u.y);
	Point I;
	I.x = A.x + ti*u.x;
	I.y = A.y + ti*u.y;
	return I;
}

bool	PhysicEngine::CollisionPointCercle(Point A, Cercle C)
{
	if (std::sqrt(std::pow(C.x - A.x, 2.0f) + std::pow(C.y - A.y, 2.0f)) - C.rayon <= 0.0f)
		return true;
	return false;
}

bool PhysicEngine::CollisionDroite(Point A, Point B, Cercle C)
{
	Vecteur u;
	u.x = B.x - A.x;
	u.y = B.y - A.y;
	Vecteur AC;
	AC.x = C.x - A.x;
	AC.y = C.y - A.y;
	float numerateur = u.x*AC.y - u.y*AC.x;   // norme du vecteur v
	if (numerateur < 0)
		numerateur = -numerateur;   // valeur absolue ; si c'est négatif, on prend l'opposé.
	float denominateur = sqrt(u.x*u.x + u.y*u.y);  // norme de u
	float CI = numerateur / denominateur;
	if (CI < C.rayon)
		return true;
	return false;
}

///////////////////////////////////////////////
/////   Segment Segment related funcs

bool	PhysicEngine::checkSegmentSegment(const std::shared_ptr<AObject> &obj)
{
	Point O;
	Point P;

	O.x = obj->getX();
	O.y = obj->getY();
	P.x = obj->getX() + obj->getDirX() * _delta;
	P.y = obj->getY() + obj->getDirY() * _delta;
	auto	it = _optiWalls.begin();
	auto	end = _optiWalls.end();
	while (it != end)
	{
		if (CollisionSegSeg(O, P, obj, *it))
		{
			bounce(obj, *it);
			return true;
		}
		++it;
	}
	return false;
}

bool	PhysicEngine::CollisionSegSeg(Point O, Point P, const std::shared_ptr<AObject> &obj, const std::shared_ptr<Wall> &wall)
{
	//float dist = std::sqrt(std::pow(wall->getX() - wall->getEndX(), 2) + std::pow(wall->getY() - wall->getEndY(), 2));
	//if (dist == 0)
	//	dist = 1;
	//float objRadius;

	//AWeapon *weapon = dynamic_cast<AWeapon *>(obj);
	//if (weapon)
	//	objRadius = weapon->getProperty()->size;
	//else
	//	objRadius = G_conf->player->size;

	//int radius = wall->getRadius() + objRadius;

	//float	factorX = (wall->getEndX() - wall->getX()) / dist;
	//float	factorY = (wall->getEndY() - wall->getY()) / dist;

	float p0_x, p0_y, p1_x, p1_y;
	p0_x = wall->getX();
	p0_y = wall->getY();
	p1_x = wall->getEndX();
	p1_y = wall->getEndY();
	//if (i == 0)
	//{
	//	p0_x = wall->getX() + factorY * radius;
	//	p0_y = wall->getY() - factorX * radius;
	//	p1_x = wall->getEndX() + factorY * radius;
	//	p1_y = wall->getEndY() - factorX * radius;
	//}
	//if (i == 1)
	//{
	//	p0_x = wall->getX() - factorY * radius;
	//	p0_y = wall->getY() + factorX * radius;
	//	p1_x = wall->getEndX() - factorY * radius;
	//	p1_y = wall->getEndY() + factorX * radius;
	//}
	//if (i == 2)
	//{
	//	p0_x = wall->getX() - factorY * radius - factorX * radius;
	//	p0_y = wall->getY() + factorX * radius + factorY * radius;
	//	p1_x = wall->getX() + factorY * radius - factorX * radius;
	//	p1_y = wall->getY() - factorX * radius + factorY * radius;
	//}
	//if (i == 3)
	//{
	//	p0_x = wall->getEndX() - factorY * radius - factorX * radius;
	//	p0_y = wall->getEndY() + factorX * radius + factorY * radius;
	//	p1_x = wall->getEndX() + factorY * radius - factorX * radius;
	//	p1_y = wall->getEndY() - factorX * radius + factorY * radius;
	//}


	float p2_x = O.x;
	float p2_y = O.y;
	float p3_x = P.x;
	float p3_y = P.y;
	float s1_x, s1_y, s2_x, s2_y;
	s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
	s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

	float s, t;
	s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
	t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		float i_x;
		float i_y;
		// Collision detected
		i_x = p0_x + (t * s1_x);
		i_y = p0_y + (t * s1_y);

		float diff = std::sqrt(std::pow(obj->getX() - i_x, 2) + std::pow(obj->getY() - i_y, 2)) - obj->getRadius();
		float speed = std::sqrt(std::pow(obj->getDirX(), 2) + std::pow(obj->getDirY(), 2));
		if (speed == 0.f)
			speed = 1.f;
		obj->setPosition(obj->getX() + obj->getDirX() / speed * diff,
			obj->getY() + obj->getDirY() / speed * diff);

		t_impact *toSend = new t_impact;
		toSend->wall = wall;
		toSend->pos = std::pair<float, float>(i_x, i_y);
		ADD_EVENT(ev_WALL_COLLISION, s_event(obj, toSend));
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

void	PhysicEngine::setDelta(const float &delta)
{
	_delta = delta;
}

const float &PhysicEngine::getDelta()
{
	return (_delta);
}


//
// Respawn.cpp for Void Clash in /home/gegon/pfa/client/GameEngine/inc
// 
// Made by Gegon
// Login   <gegon@epitech.net>
// 
// Started on  Thu Oct  3 17:45:15 2013 Gegon
// Last update Fri Feb 21 18:07:07 2014 Gegon
//

#include	<iostream>
#include	<cmath>
#include	"Respawn.hpp"
#include	"Wall.hpp"
#include	"Map.hpp"
#include	"Player.hpp"
#include	"Defines.h"
#include	"ConfigParser.hpp"
#include	"Event.hpp"

extern t_config *G_conf;

Respawn::Respawn(float X, float Y,
	float dirX, float dirY,
	int width,
	int height,
	sf::Uint16 team) :
	AObject(RESPAWN, X, Y, dirX, dirY)
{
	_width = width;
	_height = height;
	_team = team;
}


Respawn::Respawn() :
AObject(RESPAWN, 0, 0, 0, 0)
{
	_width = 0;
	_height = 0;
	_team = 0;
}


Respawn::~Respawn()
{
}

///////////////////////////////////////////////
/////   Called each frame

bool	Respawn::update()
{
	//std::list<AObject *>::iterator	it = S_Map->getElems()->begin();
	//std::list<AObject *>::iterator	end = S_Map->getElems()->end();

	//while (it != end)
	//{
	//	// Weapon check
	//	AWeapon *weapon = dynamic_cast<AWeapon *>(*it);
	//	if (S_Map->getMode()->getProperty()->team && _team != 0 && weapon)
	//	{
	//		if (isInsideRespawn((*it)->getX(), (*it)->getY()) == true)
	//		{
	//			// Delete bomb if player prime bomb inside resup
	//			if ((*it)->getType() == BOMB && (*it)->getOwner()->getBombOnHold() == *it)
	//				(*it)->getOwner()->deleteBombOnHold();
	//			ADD_EVENT(ev_DELETE, s_event((*it)));
	//		}
	//	}

	//	// Player check
	//	if (S_Map->getMode()->getProperty()->team && _team != 0 &&
	//		(*it)->getType() == PLAYER)
	//	{
	//		Player	*player = dynamic_cast<Player *>(*it);
	//		if (player && isInsideRespawn((*it)->getX(), (*it)->getY()) &&
	//			!player->isRespawning())
	//		{
	//			if (_team == player->getTeam())
	//				player->setInsideRespawn();
	//			else if (_team != 0)
	//				player->isHitBy(NULL, G_conf->player->life);
	//		}
	//	}
	//	++it;
	//}
	return (true);
}

bool	Respawn::isInsideRespawn(float X, float Y)
{
	if (X > _pos.first && X < _pos.first + _width &&
		Y > _pos.second && Y < _pos.second + _height)
		return (true);
	return (false);
}

///////////////////////////////////////////////
/////	Respawn related funcs
bool	Respawn::respawnObject(AObject *obj)
{
	sf::Int32	randX = 0;
	sf::Int32	randY = 0;
	if (_width)
		randX = std::rand() % _width;
	if (_height)
		randY = std::rand() % _height;
	obj->setPosition(_pos.first + randX, _pos.second + randY);
	return checkCollisionWithWall(obj);
}

bool	Respawn::checkCollisionWithWall(AObject *obj)
{
	sf::Vector2f A;
	sf::Vector2f B;
	Circle C;

	C.x = obj->getX();
	C.y = obj->getY();
	auto	it = S_Map->getWalls()->begin();
	auto	end = S_Map->getWalls()->end();
	while (it != end)
	{
		A.x = (*it)->getX();
		A.y = (*it)->getY();
		B.x = (*it)->getEndX();
		B.y = (*it)->getEndY();
		C.rayon = obj->getRadius() + (*it)->getRadius();
		if (CollisionSegment(A, B, C))
			return false;
		++it;
	}
	return true;
}

bool Respawn::CollisionSegment(sf::Vector2f A, sf::Vector2f B, Circle C)
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
	if (pscal1 >= 0 && pscal2 >= 0)
		return true;   // I entre A et B, ok.
	// dernière possibilité, A ou B dans le cercle
	if (CollisionPointCercle(A, C))
		return true;
	if (CollisionPointCercle(B, C))
		return true;
	return false;
}

bool	Respawn::CollisionPointCercle(sf::Vector2f A, Circle C)
{
	if (std::sqrt(std::pow(C.x - A.x, 2.0f) + std::pow(C.y - A.y, 2.0f)) <= C.rayon)
		return true;
	return false;
}

bool Respawn::CollisionDroite(sf::Vector2f A, sf::Vector2f B, Circle C)
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
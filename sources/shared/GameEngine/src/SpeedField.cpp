//
// SpeedField.cpp for Void Clash in /home/gegon/PFA/client/GameEngine/src
// 
// Made by brunel guillaume
// Login   <brunel_g@epitech.net>
// 
// Started on  Sun Sep  1 22:28:47 2013 brunel guillaume
// Last update Tue Dec 10 21:47:21 2013 Gegon
//

#include	<iostream>
#include	<cmath>
#include	"SpeedField.hpp"
#include	"Map.hpp"
#include	"Player.hpp"
#include	"Defines.h"
#include  "Event.hpp"

SpeedField::SpeedField() :
AObject(SPEED_FIELD, 0, 0, 0, 0)
{
	_width = 0;
	_height = 0;
}

SpeedField::SpeedField(float X, float Y,
	float dirX, float dirY,
	float width,
	float height) :
	AObject(SPEED_FIELD, X, Y, dirX, dirY)
{
	_width = width;
	_height = height;
}

SpeedField::~SpeedField()
{
}

///////////////////////////////////////////////
/////   Called each frame

bool	SpeedField::update()
{
	auto	it = S_Map->getElems()->begin();
	auto	end = S_Map->getElems()->end();

	while (it != end)
	{
		// Checking if AObject is in range of gravity field
		if ((*it)->getX() > getX() && (*it)->getX() < getX() + _width &&
			(*it)->getY() > getY() && (*it)->getY() < getY() + _height)
		{
			// Desactivate friction for player inside Gravity Field
			Player *player;

			player = dynamic_cast<Player *>((*it).get());
			if (player)
			{
				player->setInsideGravityField();
				ADD_EVENT(ev_IN_SPEEDFIELD, s_event(*it));
			}
		}
		++it;
	}

	return (true);
}
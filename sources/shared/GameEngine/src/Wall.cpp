//
// Wall.cpp for Wall.cpp in /home/gegon/PFA/client/src
// 
// Made by brunel guillaume
// Login   <brunel_g@epitech.net>
// 
// Started on  Mon Aug 12 11:26:04 2013 brunel guillaume
// Last update Fri Jan 17 22:05:49 2014 Gegon
//

#include	<iostream>
#include	<cmath>
#include	"Wall.hpp"
#include	"Defines.h"

Wall::Wall() :
  AObject(WALL, 0, 0, 0, 0)
{
	_end = std::pair<float, float>(0.0f, 0.0f);
  calculateAngle();
  _len = 0;
  _orientation = NO_DIR;
  _radius = 10;
}


Wall::Wall(float X, float Y,
	   int len,
	   enum eOrientation orientation) :
  AObject(WALL, X, Y, 0, 0)
{
	_end = std::pair<float, float>(0.0f, 0.0f);
  if (orientation == VERTICAL)
    {
      _end.first = X;
      _end.second = Y + len;
    }
  else if (orientation == HORIZONTAL)
    {
      _end.first = X + len;
      _end.second = Y;
    }
  calculateAngle();
  _len = std::abs(len);
  _orientation = orientation;
  _radius = 10;
}

Wall::Wall(float X, float Y,
	   float endX, float endY) :
  AObject(WALL, X, Y, 0, 0)

{
	_end = std::pair<float, float>(endX, endY);
  _len = std::sqrt(std::pow(endX - X, 2.0f) + std::pow(endY - Y, 2.0f));
  calculateAngle();
  _orientation = NO_DIR;
  _radius = 10;
}

Wall::~Wall()
{}

void	Wall::recompute()
{
	_len = std::sqrt(std::pow(_end.first - _pos.first, 2.0f) + std::pow(_end.second - _pos.second, 2.0f));
	calculateAngle();
	_orientation = NO_DIR;
}

void		Wall::calculateAngle()
{
  float radian = std::atan2(_end.second - _pos.second, _end.first - _pos.first);
  _angle = radian / 3.14159265359f * 180.0f;  
}

enum eOrientation	Wall::getOrientation() const
{
  return (_orientation);
}

int	Wall::getLen() const
{
  return (_len);
}

void	Wall::setOrientation(enum eOrientation orientation)
{
  _orientation = orientation;
}

const float&		Wall::getAngle() const
{
  return _angle;
}

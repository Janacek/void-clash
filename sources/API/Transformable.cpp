/********************************************************************
**
**              Transformable.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include <cstring>
#include "Transformable.hpp"

using namespace ogl;

/////////////////////////////////////////////////////////////////////
/////	Ctor/Dtor
/////////////////////////////////////////////////////////////////////

Transformable::Transformable(void)
{
	resetTransform();
}

Transformable::~Transformable(void)
{

}

/////////////////////////////////////////////////////////////////////
/////	Create an orthogonal projection
/////////////////////////////////////////////////////////////////////

void Transformable::ortho(float left, float top, float width, float height)
{
	_transform = glm::ortho(left, top, width, height);
}

/////////////////////////////////////////////////////////////////////
/////	Reset matrices to identity
/////////////////////////////////////////////////////////////////////

void Transformable::resetTransform(void)
{
	_transform = glm::mat4(1.0f);
	_MVP = glm::mat4(1.0f);
}

/////////////////////////////////////////////////////////////////////
/////	Translation transformation
/////////////////////////////////////////////////////////////////////

void Transformable::translate(const glm::vec3 &moves)
{
	_transform = glm::translate(moves) * _transform;
}

void Transformable::translate(float x, float y, float z)
{
	_transform = glm::translate(glm::vec3(x, y, z)) * _transform;
}

/////////////////////////////////////////////////////////////////////
/////	Rotation transformation
/////////////////////////////////////////////////////////////////////

void Transformable::rotate(const float &angle, Axis axis)
{
	glm::vec3 rotationAxis;
	glm::mat4 rotation;

	switch (axis)
	{
		case Xaxis:
		rotationAxis = glm::vec3(1, 0, 0);
		break;

		case Yaxis:
		rotationAxis = glm::vec3(0, 1, 0);
		break;

		case Zaxis:
		rotationAxis = glm::vec3(0, 0, 1);
		break;

		default:
		rotationAxis = glm::vec3(0, 0, 0);
	}

	_transform = glm::rotate(angle, rotationAxis) * _transform;
}

void Transformable::rotate(const float &angle, const glm::vec3 &axis)
{
	_transform = glm::rotate(angle, axis) * _transform;
}

/////////////////////////////////////////////////////////////////////
/////	Scale transformation
/////////////////////////////////////////////////////////////////////

void Transformable::scale(const glm::vec3 &scales)
{
	_transform = glm::scale(scales) * _transform;
}

void Transformable::scale(float x, float y, float z)
{
	_transform = glm::scale(glm::vec3(x, y, z)) * _transform;
}

/////////////////////////////////////////////////////////////////////
/////	Getters
/////////////////////////////////////////////////////////////////////

const glm::mat4& Transformable::getTransform(void)
{
	return _transform;
}

const glm::mat4& Transformable::getMVP(const glm::mat4& camera)
{
	_MVP = camera * _transform;
	return _MVP;
}

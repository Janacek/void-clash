/********************************************************************
**
**              Camera.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "Camera.hpp"

using namespace ogl;

/////////////////////////////////////////////////////////////////////
/////	Ctor/Dtor
/////////////////////////////////////////////////////////////////////

Camera::Camera(void) : Transformable()
{
}

Camera::~Camera(void)
{

}

/////////////////////////////////////////////////////////////////////
/////	Create hud camera (default camera)
/////////////////////////////////////////////////////////////////////

void Camera::setupOrthographic(float sizeX, float sizeY, bool reverse)
{
	if (reverse == true)
		_projection = glm::ortho(0.0f, sizeX, sizeY, 0.0f);
	else
		_projection = glm::ortho(0.0f, sizeX, 0.0f, sizeY);

	_isOrtho = true;
}

/////////////////////////////////////////////////////////////////////
/////	Initialize camera matrix
/////////////////////////////////////////////////////////////////////

void Camera::setupPerspective(const GLfloat &fovY, const GLfloat &aspect,
	const GLfloat &zNear, const GLfloat &zFar)
{
	_projection = glm::perspective(fovY, aspect, zNear, zFar);
	_isOrtho = false;
}

/////////////////////////////////////////////////////////////////////
/////	Set camera position/direction
/////////////////////////////////////////////////////////////////////

void Camera::lookAt(const glm::vec3 &eye, const glm::vec3 &center,
	const glm::vec3 &up)
{
	_eye = eye;
	_view = glm::lookAt(eye, center, up);
}

/////////////////////////////////////////////////////////////////////
/////	Precompute matrix calculation
/////////////////////////////////////////////////////////////////////

void Camera::update(void)
{
	_final = _projection * _view * getTransform();
}

/////////////////////////////////////////////////////////////////////
/////	Getters
/////////////////////////////////////////////////////////////////////

const glm::mat4& Camera::getProjectionMatrix(void) const
{
	return _projection;
}

const glm::mat4& Camera::getView(void) const
{
	return _view;
}

const glm::mat4& Camera::getFinal(void) const
{
	return _final;
}

const glm::vec3& Camera::getEye(void) const
{
	return _eye;
}

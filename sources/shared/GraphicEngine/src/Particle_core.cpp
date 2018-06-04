/********************************************************************
**
**              Particle_core.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "Particle.hpp"
#include "GraphicHelper.hpp"

namespace VoidClashGraphics
{
    /////////////////////////////////////////////////////////////////////
    /////	Ctor/Dtor
    /////////////////////////////////////////////////////////////////////

    Particle::Particle(void)
    {
    }

    Particle::~Particle(void)
    {
    }

    /////////////////////////////////////////////////////////////////////
    /////	Move a particle into another this slot
    /////////////////////////////////////////////////////////////////////

    void Particle::set(const Particle &particle)
    {
		std::memcpy(&_desc, &particle._desc, sizeof(ParticleAttributes));
    }

    /////////////////////////////////////////////////////////////////////
    /////	Create
    /////////////////////////////////////////////////////////////////////

    void Particle::create(void)
    {
		std::memset(&_desc, 0, sizeof(ParticleAttributes));
	}

    /////////////////////////////////////////////////////////////////////
    /////	Update
    /////////////////////////////////////////////////////////////////////

    bool Particle::update(float deltatime)
    {
		_desc.currentLife += deltatime;

        // Particle is dead
        if (isAlive() == false)
            return false;

        // Update position
		applyVelocity(deltatime);

		// Update velocity
		applyConstantFriction(deltatime);
		applyRatioFriction(deltatime);

        // Calulate matrix
		calculateMatrix();

		// Calulate the color
		calculateColor(deltatime);

		return true;
    }

	/////////////////////////////////////////////////////////////////////
	/////	Calculate the new particle geometry
	/////////////////////////////////////////////////////////////////////

	void Particle::calculateMatrix(void)
	{
		bool orientedByVelocity = true;

		resetTransform();
		scale(_desc.size, _desc.size);

		//if (_desc.orientationBase.x != 0)
		//{
		//	rotate(_desc.orientationBase.x + _desc.currentLife * 90, ogl::Xaxis);
		//	orientedByVelocity = false;
		//}
		//if (_desc.orientationBase.y != 0)
		//{
		//	rotate(_desc.orientationBase.y + _desc.currentLife * 90, ogl::Yaxis);
		//	orientedByVelocity = false;
		//}
		if (_desc.orientationBase.z != 0)
		{
			rotate(_desc.orientationBase.z - _desc.currentLife * 720, ogl::Zaxis);
			orientedByVelocity = false;
		}

		if (orientedByVelocity == true && _desc.oriented2D == true)
		{
			rotate(90 + G_TO_DEGREE(std::atan2(_desc.velocity.z, _desc.velocity.x)), ogl::Zaxis);
		}
		else if (orientedByVelocity == true && _desc.oriented3D == true)
		{
			// FUCK IT TODO
		}

		translate(_desc.position);
	}

	/////////////////////////////////////////////////////////////////////
	/////	Is the particle still alive
	/////////////////////////////////////////////////////////////////////

	bool Particle::isAlive(void)
	{
		if (_desc.currentLife > _desc.lifetime)
			return false;

		if (_desc.lifeByVelocity == true)
		{
			float current = std::sqrt(
				std::pow(_desc.velocity.x, 2.0f) +
				std::pow(_desc.velocity.y, 2.0f) +
				std::pow(_desc.velocity.z, 2.0f)
			);

			if (current == 0)
				return false;
		}

		return true;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Apply the velocity
	/////////////////////////////////////////////////////////////////////

	void Particle::applyVelocity(float deltatime)
	{
		if (_desc.velocityEnabled == false)
			return;

		_desc.position.x += _desc.velocity.x * deltatime;
		_desc.position.z += _desc.velocity.y * deltatime;
		_desc.position.y += _desc.velocity.z * deltatime;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Apply the friction
	/////////////////////////////////////////////////////////////////////

	void Particle::applyConstantFriction(float deltatime)
	{
		if (_desc.constantFrictionEnabled == false)
			return;

		glm::vec3 previousVelocity(_desc.velocity);
		glm::vec3 sign(1, 1, 1);

		if (_desc.frictionCareAboutSign == true)
		{
			sign.x = (_desc.velocityInit.x < 0) ? (-1) : (1);
			sign.y = (_desc.velocityInit.y < 0) ? (-1) : (1);
			sign.z = (_desc.velocityInit.z < 0) ? (-1) : (1);
		}

		_desc.velocity.x -= _desc.constantFriction.x * deltatime * sign.x;
		_desc.velocity.y -= _desc.constantFriction.y * deltatime * sign.y;
		_desc.velocity.z -= _desc.constantFriction.z * deltatime * sign.z;

		clampVelocity(previousVelocity);
	}

	void Particle::applyRatioFriction(float deltatime)
	{
		if (_desc.ratioFrictionEnabled == false)
			return;

		glm::vec3 previousVelocity(_desc.velocity);

		_desc.velocity.x -= _desc.velocityInit.x * _desc.ratioFriction.x * deltatime;
		_desc.velocity.y -= _desc.velocityInit.y * _desc.ratioFriction.y * deltatime;
		_desc.velocity.z -= _desc.velocityInit.z * _desc.ratioFriction.z * deltatime;

		clampVelocity(previousVelocity);
	}

	/////////////////////////////////////////////////////////////////////
	/////	Clamp the velocity
	/////////////////////////////////////////////////////////////////////

	void Particle::clampVelocity(const glm::vec3 &previousVelocity)
	{
		if (_desc.velocityClamped == true)
		{
			if (std::fabs(_desc.velocity.x) < std::fabs(_desc.velocityInit.x) * _desc.velocityClamp.x)
				_desc.velocity.x = 0;
			if (std::fabs(_desc.velocity.x) > std::fabs(_desc.velocityInit.x) * _desc.velocityClamp.y)
				_desc.velocity.x = previousVelocity.x;

			if (std::fabs(_desc.velocity.y) < std::fabs(_desc.velocityInit.y) * _desc.velocityClamp.x)
				_desc.velocity.y = 0;
			if (std::fabs(_desc.velocity.y) > std::fabs(_desc.velocityInit.y) * _desc.velocityClamp.y)
				_desc.velocity.y = previousVelocity.y;

			if (std::fabs(_desc.velocity.z) < std::fabs(_desc.velocityInit.z) * _desc.velocityClamp.x)
				_desc.velocity.z = 0;
			if (std::fabs(_desc.velocity.z) > std::fabs(_desc.velocityInit.z) * _desc.velocityClamp.y)
				_desc.velocity.z = previousVelocity.z;
		}
	}

	/////////////////////////////////////////////////////////////////////
	/////	Calculate the color
	//////////////////////////////////////////////	///////////////////////

	void Particle::calculateColor(float deltatime)
	{
		if (_desc.colorByLife == true)
			_desc.color.a = 1.0f - (_desc.currentLife / _desc.lifetime);
	
		if (_desc.alphaByVelocity == true)
		{
			float current = std::sqrt(
				std::pow(_desc.velocity.x, 2.0f) +
				std::pow(_desc.velocity.y, 2.0f) +
				std::pow(_desc.velocity.z, 2.0f));

			float base = std::sqrt(
				std::pow(_desc.velocityInit.x, 2.0f) +
				std::pow(_desc.velocityInit.y, 2.0f) +
				std::pow(_desc.velocityInit.z, 2.0f));

			_desc.color.a = _desc.colorInit.a * (current / base);
		}
	}
}

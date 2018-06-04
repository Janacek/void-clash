/********************************************************************
**
**              Particle_utils.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "glm/gtc/random.hpp"
#include "Particle.hpp"

namespace VoidClashGraphics
{
	/////////////////////////////////////////////////////////////////////
	/////	Getters
	/////////////////////////////////////////////////////////////////////

	const glm::vec4& Particle::getColor(void)
	{
		return _desc.color;
	}

	const glm::mat4& Particle::getMatrix(void)
	{
		return getTransform();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Position
	/////////////////////////////////////////////////////////////////////

	void Particle::setPosition(const glm::vec3 &position, eRandomType random)
	{
		if (random == PS_SPHERE_RANDOM)
		{
			_desc.position = glm::sphericalRand(position.x);
		}
		else if (random == PS_DISC_RANDOM)
		{
			const glm::vec2 &tmp(glm::diskRand(position.x));
			_desc.position.x = tmp.x;
			_desc.position.z = tmp.y;
		}
		else
		{
			_desc.position = position;
		}
	}

	/////////////////////////////////////////////////////////////////////
	/////	Rotation
	/////////////////////////////////////////////////////////////////////

	void Particle::setRotation(eRandomType random)
	{
		if (random == PS_STD_RANDOM)
		{
			_desc.orientationBase.x = std::rand() % 360;
			_desc.orientationBase.y = std::rand() % 360;
			_desc.orientationBase.z = std::rand() % 360;
		}
	}

	/////////////////////////////////////////////////////////////////////
	/////	Velocity
	/////////////////////////////////////////////////////////////////////

	void Particle::clampVelocity(const glm::vec2 &clamp, bool constant)
	{
		_desc.velocityClamped = true;

		if (constant == true)
			_desc.velocityClamp = clamp;
		else
			_desc.velocityClamp = glm::vec2(clamp.x * 0.01f, clamp.y * 0.01f);
	}

	void Particle::setVelocity(const glm::vec3 &velocity, eRandomType random, bool absolute)
	{
		if (random == PS_SPHERE_RANDOM)
		{
			_desc.velocity = glm::sphericalRand(velocity.x);
		}
		else if (random == PS_BALL_RANDOM)
		{
			_desc.velocity = glm::ballRand(velocity.x);
		}
		else if (random == PS_DISC_RANDOM)
		{
			const glm::vec2 &tmp(glm::diskRand(velocity.x));
			_desc.velocity.x = tmp.x;
			_desc.velocity.z = tmp.y;
		}
		else if (random == PS_STD_RANDOM)
		{
			_desc.velocity.x = (std::fmod(std::rand(), velocity.x * 500) + 500) * 0.001;
			_desc.velocity.y = (std::fmod(std::rand(), velocity.y * 500) + 500) * 0.001;
			_desc.velocity.z = (std::fmod(std::rand(), velocity.z * 500) + 500) * 0.001;
		}
		else
		{
			_desc.velocity = velocity;
		}

		if (absolute == true)
		{
			_desc.velocity.x = std::fabs(_desc.velocity.x);
			_desc.velocity.y = std::fabs(_desc.velocity.y);
			_desc.velocity.z = std::fabs(_desc.velocity.z);
		}

		_desc.velocityInit = _desc.velocity;
		_desc.velocityEnabled = true;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Friction
	/////////////////////////////////////////////////////////////////////

	void Particle::setFriction(const glm::vec3 &friction, eRandomType random,
		bool constant, bool absolute)
	{
		glm::vec3 &toModify(
			(constant == true) ? (_desc.constantFriction) : (_desc.ratioFriction));

		if (random == PS_SPHERE_RANDOM)
		{
			toModify = glm::sphericalRand(friction.x);
		}
		else if (random == PS_BALL_RANDOM)
		{
			toModify = glm::ballRand(friction.x);
		}
		else if (random == PS_STD_RANDOM)
		{
			float tmp((std::fmod(std::rand(), friction.x * 2000) - 1000) * 0.001);
			toModify = glm::vec3(tmp, tmp, tmp);
		}
		else
		{
			toModify = friction;
		}

		if (absolute == true)
		{
			toModify.x = std::fabs(toModify.x);
			toModify.y = std::fabs(toModify.y);
			toModify.z = std::fabs(toModify.z);
		}

		_desc.constantFrictionEnabled = constant;
		_desc.ratioFrictionEnabled = !constant;
	}

	void Particle::setFrictionCareAboutSign(bool careAboutSign)
	{
		_desc.frictionCareAboutSign = careAboutSign;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Size
	/////////////////////////////////////////////////////////////////////

	void Particle::setSize(float size, eRandomType random)
	{
		if (random == PS_STD_RANDOM)
		{
			float tmp = size * 500;
			_desc.size = (std::fmod(std::rand(), tmp) + tmp) * 0.001;
		}
		else
		{
			_desc.size = size;
		}
	}

	/////////////////////////////////////////////////////////////////////
	/////	Color
	/////////////////////////////////////////////////////////////////////

	void Particle::setColorByLife(bool enabled)
	{
		_desc.colorByLife = enabled;
	}

	void Particle::setAlphaByVelocity(bool enabled)
	{
		_desc.alphaByVelocity = enabled;
	}

	void Particle::setColor(const glm::vec4 &color, eRandomType random)
	{
		_desc.colorInit = color;
		_desc.color = color;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Lifetime
	/////////////////////////////////////////////////////////////////////

	void Particle::setLifetime(float lifetime, eRandomType random)
	{
		if (random == PS_STD_RANDOM)
		{
			unsigned int tmp = lifetime * 500; // (1000 * 0.5)
			_desc.lifetime = (std::rand() % tmp + tmp) * 0.001;
		}
		else
		{
			_desc.lifetime = lifetime;
		}
	}

	void Particle::setLifeByVelocity(bool enabled)
	{
		_desc.lifeByVelocity = enabled;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Geometry
	/////////////////////////////////////////////////////////////////////

	void Particle::set2DOriented(bool enabled)
	{
		_desc.oriented2D = true;
	}

	void Particle::set3DOriented(bool enabled)
	{
		_desc.oriented3D = true;
	}
}
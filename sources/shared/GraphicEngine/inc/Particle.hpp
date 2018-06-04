/********************************************************************
**
**              Particle.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_PARTICLE
#define VOID_CLASH_PARTICLE

#include "Transformable.hpp"
#include "ParticleSystemFactory.hpp"

namespace VoidClashGraphics
{
	#define CLAMP_UNINIT_VALUE 9999999

	struct ParticleAttributes
	{
		// How the particle evolves

		bool lifeByVelocity;

		bool constantFrictionEnabled;
		bool ratioFrictionEnabled;
		bool frictionCareAboutSign;

		bool velocityEnabled;
		bool velocityClamped;

		bool alphaByVelocity;
		bool colorByLife;

		// Attributes

		glm::vec4 colorInit;
		glm::vec3 position;

		glm::vec3 constantFriction;
		glm::vec3 ratioFriction;

		glm::vec3 velocityInit;
		glm::vec2 velocityClamp;

		float lifetime;
		float size;

		// Geometry

		bool oriented2D;
		bool oriented3D;
		glm::vec3 orientationBase;

		// Current state

		float currentLife;
		glm::vec3 velocity;
		glm::vec4 color;
	};

    class Particle : public ogl::Transformable
    {
    public:
        Particle(void);
        virtual ~Particle(void);

        // Management
        virtual void create(void);
        virtual bool update(float deltatime);
        virtual void set(const Particle& particle);

        // Basic attributes
        void setPosition(const glm::vec3 &position, eRandomType random = PS_NO_RANDOM);
		void setSize(float size, eRandomType = PS_NO_RANDOM);

		// Life
		void setLifetime(float lifetime, eRandomType random = PS_NO_RANDOM);
		void setLifeByVelocity(bool enabled);

		// Color
		void setColor(const glm::vec4 &color, eRandomType random = PS_NO_RANDOM);
		void setAlphaByVelocity(bool enabled);
		void setColorByLife(bool enabled);

		// Velocity
		void setVelocity(const glm::vec3 &velocity, eRandomType random = PS_NO_RANDOM, bool absolute = false);
		void clampVelocity(const glm::vec2 &clamp, bool constant = true);

		// Base orientation
		void setRotation(eRandomType random = PS_NO_RANDOM);

		// Friction
		void setFriction(const glm::vec3 &friction, eRandomType random, bool constant = true, bool absolute = false);
		void setFrictionCareAboutSign(bool careAboutSign);

		// Geometry
		void set2DOriented(bool enabled);
		void set3DOriented(bool enabled);

		// Getters
        const glm::vec4& getColor(void);
        const glm::mat4& getMatrix(void);

	private:
		bool isAlive(void);

		// Wrappers
		void applyVelocity(float deltatime);
		void applyConstantFriction(float deltatime);
		void applyRatioFriction(float deltatime);
		void clampVelocity(const glm::vec3 &previousVelocity);
		void calculateColor(float deltatime);
		void calculateMatrix(void);

    private:
		ParticleAttributes _desc;
    };
}

#endif /* end of include guard: VOID_CLASH_PARTICLE */

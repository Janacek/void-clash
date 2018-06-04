/********************************************************************
**
**              ParticleSystemFactory.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "ParticleSystemFactory.hpp"
#include "ParticleSystem.hpp"
#include "Particle.hpp"
#include "GraphicHelper.hpp"

#include "Player.hpp"
#include "Explosion.hpp"

namespace VoidClashGraphics
{
    /////////////////////////////////////////////////////////////////////
    /////	Ctor/Dtor
    /////////////////////////////////////////////////////////////////////

    CParticleSystemFactory::CParticleSystemFactory(void)
    {
    }

    CParticleSystemFactory::~CParticleSystemFactory(void)
    {
    }

    /////////////////////////////////////////////////////////////////////
    /////	Generate particles according to system type
    /////////////////////////////////////////////////////////////////////

    ParticleSystem* CParticleSystemFactory::setupParticleSystem(
        std::pair<eventType, s_event> &event)
    {
        switch (event.first)
        {
            case ev_EXPLOSION: return particleSystemExplosion(event);
            default: return NULL;
        }
    }

    /////////////////////////////////////////////////////////////////////
    /////	Explosion
    /////////////////////////////////////////////////////////////////////

    ParticleSystem* CParticleSystemFactory::particleSystemExplosion(
        std::pair<eventType, s_event> &event)
    {
		// Check pointers
		std::shared_ptr<Explosion> explosion = std::dynamic_pointer_cast<Explosion>(event.second.trigger);
		if (explosion == NULL || explosion->getOwner() == NULL)
			return NULL;
		
		// Create
        ParticleSystem *s = new ParticleSystem(event.first);
        s->create();
		s->setTexture("Particle_explosion");

        // Resize the container
        s->setParticleNumber(250);

        // Initialize particles
        std::vector<Particle>& particles(s->getParticles());
        unsigned int i = 0;
        unsigned int last = particles.size();

        glm::vec3 position(50, 0, 0);
        glm::vec3 velocity(5000, 0, 0);
		glm::vec3 friction(1.25, 1.25, 1.25);
		glm::vec2 clampVelocity(5, 100);
		float lifetime = 1.25f;
		float size = 600;

        while (i != last)
        {
            particles[i].create();
            particles[i].setPosition(position, PS_BALL_RANDOM);
			particles[i].setVelocity(velocity, PS_BALL_RANDOM);
			particles[i].setFriction(friction, PS_NO_RANDOM, false, true);
			particles[i].setRotation(PS_STD_RANDOM);
			particles[i].clampVelocity(clampVelocity, false);
			particles[i].setColorByLife(true);
			particles[i].setLifetime(lifetime, PS_STD_RANDOM);
			particles[i].setSize(size, PS_STD_RANDOM);

			// First group of particles OR explosion owner is spectator
			if (i < last / 2 || explosion->getOwner()->getTeam() == 0)
				particles[i].setColor(COLOR_WHITE);
			else if (explosion->getOwner()->getTeam() == 1)
				particles[i].setColor(COLOR_GREEN);
			else if (explosion->getOwner()->getTeam() == 2)
				particles[i].setColor(COLOR_RED);

            ++i;
        }

        s->setPosition(glm::vec3(
            event.second.trigger->getX(),
            -event.second.trigger->getY(),
            0.0f
        ));

        return s;
    }
}

/********************************************************************
**
**              ParticleSystemModule.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "ParticleSystemModule.hpp"
#include "ParticleSystem.hpp"

namespace VoidClashGraphics
{
    /////////////////////////////////////////////////////////////////////
    /////	Ctor/Dtor
    /////////////////////////////////////////////////////////////////////

    ParticleSystemModule::ParticleSystemModule(void)
    {

    }

    ParticleSystemModule::~ParticleSystemModule(void)
    {

    }

    /////////////////////////////////////////////////////////////////////
    /////	Start
    /////////////////////////////////////////////////////////////////////

    void ParticleSystemModule::start(void)
    {

    }

    /////////////////////////////////////////////////////////////////////
    /////	Stop
    /////////////////////////////////////////////////////////////////////

    void ParticleSystemModule::stop(void)
    {
        auto it = _systems.begin();
        auto end = _systems.end();

        while (it != end)
        {
            (*it)->destroy();
            delete (*it);
            ++it;
        }

        _systems.clear();
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update
    /////////////////////////////////////////////////////////////////////

    void ParticleSystemModule::update(float deltatime)
    {
        // Check events
        checkEvents();

        // Update particle systems
        auto it = _systems.begin();
        auto end = _systems.end();

        while (it != end)
        {
            // Update and display the particle system
            if ((*it)->update(deltatime) == false)
            {
                // Particle system is dead
                (*it)->destroy();
                delete (*it);
                it = _systems.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    /////////////////////////////////////////////////////////////////////
    /////	Check events
    /////////////////////////////////////////////////////////////////////

    void ParticleSystemModule::checkEvents(void)
    {
        if (Event::getMainEventList() != NULL)
        {
            auto ev = Event::getMainEventList()->begin();
            auto end = Event::getMainEventList()->end();

            while (ev != end)
            {
                if (ev->first == ev_EXPLOSION)
                {
                    addChild(*ev);
                }
                ++ev;
            }
        }
    }

    /////////////////////////////////////////////////////////////////////
    /////	Add a new particle system
    /////////////////////////////////////////////////////////////////////

    void ParticleSystemModule::addChild(std::pair<eventType, s_event> &event)
    {
        ParticleSystem *newPs =
            S_ParticleSystemFactory->setupParticleSystem(event);

        if (newPs != NULL)
        {
            _systems.push_back(newPs);
        }
    }
}

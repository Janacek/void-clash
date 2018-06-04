/********************************************************************
**
**              ParticleSystemFactory.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_PARTICLE_SYSTEM_FACTORY
#define VOID_CLASH_PARTICLE_SYSTEM_FACTORY

#include <vector>
#include "SingletonBase.hpp"
#include "Event.hpp"

namespace VoidClashGraphics
{
    enum eRandomType
    {
        PS_NO_RANDOM,
        PS_STD_RANDOM,
        PS_CIRCLE_RANDOM,
        PS_DISC_RANDOM,
        PS_SPHERE_RANDOM,
		PS_BALL_RANDOM,
    };

    class Particle;
    class ParticleSystem;

    class CParticleSystemFactory
    {
    public:
        CParticleSystemFactory(void);
        ~CParticleSystemFactory(void);

        ParticleSystem* setupParticleSystem(
            std::pair<eventType, s_event> &event);

    private:
        ParticleSystem* particleSystemExplosion(
            std::pair<eventType, s_event> &event);
    };
}

typedef SingletonBase<VoidClashGraphics::CParticleSystemFactory> ParticleSystemFactorySingleton;
#define S_ParticleSystemFactory ParticleSystemFactorySingleton::self()

#endif /* end of include guard: VOID_CLASH_PARTICLE_SYSTEM_FACTORY */

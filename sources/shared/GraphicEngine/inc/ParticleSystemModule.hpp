/********************************************************************
**
**              ParticleSystemModule.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_PARTICLE_SYSTEM_MODULE
#define VOID_CLASH_PARTICLE_SYSTEM_MODULE

#include "Module.hpp"

namespace VoidClashGraphics
{
    class ParticleSystem;

    class ParticleSystemModule : public AModule
    {
    public:
        ParticleSystemModule(void);
        virtual ~ParticleSystemModule(void);

        virtual void start(void);
        virtual void update(float deltatime);
        virtual void stop(void);
        virtual void postRendering(float deltatime, int renderPass) { (void)deltatime; (void)renderPass; }

        // Setters
        virtual void setActive(bool active) { (void)active; }

        // Types
		virtual void addType(gObjectType type) { (void)type; }
		virtual std::vector<gObjectType>& getTypeChild(void) { return _types; }
		virtual void addType(eventType type) { (void)type; }
		virtual std::vector<eventType>& getTypeEvent(void) { return _evTypes; }

    private:
        void checkEvents(void);
        void addChild(std::pair<eventType, s_event> &event);

    private:
        // Empty
        std::vector<gObjectType> _types;
        std::vector<eventType> _evTypes;

        // Particle systems
        std::list<ParticleSystem*> _systems;
    };
}

#endif /* end of include guard: VOID_CLASH_PARTICLE_SYSTEM_MODULE */

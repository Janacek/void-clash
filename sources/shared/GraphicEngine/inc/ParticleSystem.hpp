/********************************************************************
**
**              ParticleSystem.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_PARTICLE_SYSTEM
#define VOID_CLASH_PARTICLE_SYSTEM

#include <vector>
#include "Transformable.hpp"
#include "Instanced.hpp"
#include "ParticleSystemFactory.hpp"
#include "AObject.hpp"
#include "Quad.hpp"
#include "Event.hpp"

namespace VoidClashGraphics
{
    class Particle;

    class ParticleSystem
    {
    public:
        ParticleSystem(eventType type);
        virtual ~ParticleSystem(void);

        virtual void create(void);
        virtual bool update(float deltatime);
        virtual void destroy(void);

        virtual void setParticleNumber(unsigned int size);

        // Emitter
        virtual void setFollowEmitter(bool follow);
        virtual void setPosition(const glm::vec3 &position);
        virtual void setGameObject(std::shared_ptr<AObject> gameObject);

		// Assets
		virtual void setTexture(const std::string &texture);

        virtual std::vector<Particle>& getParticles(void);

    private:
        eventType _type;
        unsigned int _nextParticle;
        std::vector<Particle> _particles;

        // Emitter
        std::shared_ptr<AObject> _gameObject;
        glm::vec3 _position;
        bool _followEmitter;

        // Instancing
        ogl::Quad *_quad;
        std::vector<glm::mat4> _matrices;
        std::vector<glm::vec4> _colors;

		// Assets
		std::string _texture;
    };
}

#endif /* end of include guard: VOID_CLASH_PARTICLE_SYSTEM */

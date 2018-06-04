/********************************************************************
**
**              ParticleSystem.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "ParticleSystem.hpp"
#include "Particle.hpp"
#include "GraphicHelper.hpp"
#include "ShaderManager.hpp"
#include "TextureManager.hpp"

namespace VoidClashGraphics
{
    extern ogl::Camera globalCamera;

    /////////////////////////////////////////////////////////////////////
    /////	Ctor/Dtor
    /////////////////////////////////////////////////////////////////////

    ParticleSystem::ParticleSystem(eventType type)
    {
        _type = type;
        _followEmitter = false;
        _gameObject = NULL;
        _position = glm::vec3(0, 0, 0);
		_texture = "Particle";
    }

    ParticleSystem::~ParticleSystem(void)
    {
    }

    /////////////////////////////////////////////////////////////////////
    /////	Create
    /////////////////////////////////////////////////////////////////////

    void ParticleSystem::create(void)
    {
        _nextParticle = 0;

        // Create the rendering quad
        _quad = new ogl::Quad();
        _quad->setInstanced(true);
        _quad->setMatrixBuffer(1);
        _quad->setVec4Buffer(5);
        _quad->create();

        GL_CHECK;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update
    /////////////////////////////////////////////////////////////////////

    bool ParticleSystem::update(float deltatime)
    {
        unsigned int i = 0;
        unsigned int last = _nextParticle;
        unsigned int alive = 0;

        // Prepare instancing
        _matrices.clear();
        _matrices.resize(_nextParticle);
        _colors.clear();
        _colors.resize(_nextParticle);

        while (i != last)
        {
            // Update the particle
            if (_particles[i].update(deltatime) == false)
            {
                // Particle is dead
                if (_nextParticle > 0)
                {
                    // Copy the last particle here
                    _particles[i].set(_particles[_nextParticle - 1]);
                }
                --_nextParticle;
            }
            else
            {
                // Get particle attributes
                _matrices[alive] = _particles[i].getMatrix();
                _colors[alive] = _particles[i].getColor();

                ++alive;
            }
            ++i;
        }

        // I don't have child anymore :(
        if (alive == 0)
        {
            _particles.clear();
            return false;
        }

        // Update the geometry
        ogl::Transformable matrix;
        matrix.translate(_position);

        // Update instances
        _quad->updateMatrixBuffer(_matrices);
        _quad->updateVec4Buffer(_colors);

        // Bind the shader and the texture (if any)
        ogl::Shader *s = S_Shader->bindShader("Particle");
        s->setParameter("Camera", globalCamera.getFinal());
        s->setParameter("ParticleSystemModel", matrix.getTransform());

        S_Texture->bindTexture(_texture);

        // Setup some OpenGL states
        glDisable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        _quad->draw();

        // Clean
        s->unbind();

        GL_CHECK;

        return true;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Destroy
    /////////////////////////////////////////////////////////////////////

    void ParticleSystem::destroy(void)
    {
        _quad->destroy();
        safeDelete(_quad);
    }

    /////////////////////////////////////////////////////////////////////
    /////	Getters / Setters
    /////////////////////////////////////////////////////////////////////

	void ParticleSystem::setTexture(const std::string &texture)
	{
		_texture = texture;
	}

    void ParticleSystem::setParticleNumber(unsigned int size)
    {
        _particles.resize(size);
        _nextParticle = size;
    }

    void ParticleSystem::setFollowEmitter(bool follow)
    {
        _followEmitter = follow;
    }

    void ParticleSystem::setPosition(const glm::vec3 &position)
    {
        _position = position;
    }

    void ParticleSystem::setGameObject(std::shared_ptr<AObject> gameObject)
    {
        _gameObject = gameObject;
    }

    std::vector<Particle>& ParticleSystem::getParticles(void)
    {
        return _particles;
    }
}

/********************************************************************
**
**              CModuleFactory.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "ModuleFactory.hpp"
#include "Module.hpp"
#include "GraphicConfiguration.hpp"
#include "Log.hpp"
#include "GraphicHelper.hpp"

// Templates
#include "Templates.hpp"

// AModule which doesn't need templating
#include "Environment.hpp"
#include "ParticleSystemModule.hpp"

namespace VoidClashGraphics
{
    /////////////////////////////////////////////////////////////////////
    /////	Ctor/Dtor
    /////////////////////////////////////////////////////////////////////

    CModuleFactory::CModuleFactory(void)
    {
    }

    CModuleFactory::~CModuleFactory(void)
    {
    }

    /////////////////////////////////////////////////////////////////////
    /////	Create a new module
    /////////////////////////////////////////////////////////////////////

    AModule* CModuleFactory::createModule(gObjectType type)
    {
        // Bullet
        if (type == G_BULLET)
        {
            Module<EntityBullet> *m = new Module<EntityBullet>();
            m->resize(GCONFIG.maxBullet);
            m->addType(G_BULLET);
            m->setShader("Bullet");
            m->setMatrixBuffer(1);
            m->setVec4Buffer(5);
            m->setFloatBuffer(6);
			m->disableDepthTest();
			return m;
        }

        else if (type == G_RESPAWN)
        {
            Module<EntityRespawn> *m = new Module<EntityRespawn>();
            m->resize(GCONFIG.maxRespawn);
            m->addType(G_RESPAWN);
            m->setShader("Respawn");
            m->addTexture(1, "Respawn");
            m->addTexture(2, "Respawn2");
            m->setMatrixBuffer(1);
            m->setVec4Buffer(5);
            m->setFloatBuffer(6);
			m->disableDepthTest();
            return m;
        }

        else if (type == G_SPEEDFIELD)
        {
            Module<EntitySpeedfield> *m = new Module<EntitySpeedfield>();
            m->resize(GCONFIG.maxSpeedfield);
            m->addType(G_SPEEDFIELD);
            m->setShader("Speedfield");
            m->addTexture(COMMON_ASSET_ID, "Speedfield");
            m->setMatrixBuffer(1);
            m->setVec4Buffer(5);
            m->setVec2Buffer(6);
            m->setFloatBuffer(7);
			m->disableDepthTest();
			m->enableCumulativeBlend();
			return m;
        }

        else if (type == G_WALL)
        {
            Module<EntityWall> *m = new Module<EntityWall>();
            m->resize(GCONFIG.maxWall);
            m->addType(G_WALL);
            m->setShader("Model_Wall");
            m->addModel(COMMON_ASSET_ID, "Wall/Wall.obj");
            m->setVec4Buffer(3);
            m->setMatrixBuffer(4);
            return m;
        }

        else if (type == G_PLAYER)
        {
            Module<EntityPlayer> *m = new Module<EntityPlayer>();
            m->resize(GCONFIG.maxPlayer);
            m->addType(G_PLAYER);
            m->setShader("Model_Player");
            m->injectCameraPositionUniform();
            m->addModel(1, "ShipConcept/ShipOne.obj");
            m->addModel(2, "ShipTwo/ShipTwo.obj");
            m->setVec4Buffer(3);
            m->setMatrixBuffer(4);
            return m;
        }

        else if (type == G_GRAVITYFIELD)
        {
            Module<EntityGravity> *m = new Module<EntityGravity>();
            m->resize(GCONFIG.maxGravityfield);
            m->addType(G_GRAVITYFIELD);
            m->setShader("GravityField");
            m->addTexture(COMMON_ASSET_ID, "Gravity");
            m->setMatrixBuffer(1);
            m->setVec4Buffer(5);
			m->disableDepthTest();
			return m;
        }

        else if (type == G_FLAG)
        {
            Module<EntityFlag> *m = new Module<EntityFlag>();
            m->resize(GCONFIG.maxFlag);
            m->addType(G_FLAG);
            m->setShader("Flag");
            m->addTexture(COMMON_ASSET_ID, "Flag");
            m->setMatrixBuffer(1);
            m->setVec4Buffer(5);
			m->disableDepthTest();
			return m;
        }

        else if (type == G_BOMB)
        {
            Module<EntityBomb> *m = new Module<EntityBomb>();
            m->resize(GCONFIG.maxBomb);
            m->addType(G_BOMB);
            m->setShader("Bomb");
            m->setMatrixBuffer(1);
            m->setVec4Buffer(5);
            m->setFloatBuffer(6);
			m->disableDepthTest();
			return m;
        }

        else if (type == G_LIGHT)
        {
            Module<EntityLight> *m = new Module<EntityLight>();
            m->resize(
                GCONFIG.maxBullet +
                GCONFIG.maxBomb +
                GCONFIG.maxSpeedfield +
                GCONFIG.maxPlayer
            );
            m->addType(G_PLAYER);
            m->addType(G_BOMB);
            m->addType(G_GRAVITYFIELD);
            m->addType(G_BULLET);
            m->setShader("Light");
            m->setMatrixBuffer(1);
            m->setVec4Buffer(5);
			m->disableDepthTest();
			return m;
        }

		else if (type == G_FLASH)
		{
			Module<EntityFlash> *m = new Module<EntityFlash>();
			m->resize(GCONFIG.maxBullet);
			m->setShader("Flash");
			m->addEventType(ev_EXPLOSION);
			m->addEventType(ev_WALL_COLLISION);
			m->setMatrixBuffer(1);
			m->setVec4Buffer(5);
			m->disableDepthTest();
			return m;
		}

		else if (type == G_CAPTURE)
        {
            Module<EntityCapture> *m = new Module<EntityCapture>();
            m->resize(GCONFIG.maxCapture);
            m->addType(G_CAPTURE);
            m->setShader("Capture");
            m->setMatrixBuffer(1);
            m->setVec4Buffer(5);
            m->setVec2Buffer(7);
			m->disableDepthTest();
			return m;
        }

        else if (type == G_ENVIRONMENT)
        {
            Environment *m = new Environment();
            m->addType(G_ENVIRONMENT);
            return m;
        }

        else if (type == G_PARTICLE_SYSTEM)
        {
            ParticleSystemModule *m = new ParticleSystemModule();
            m->addType(G_PARTICLE_SYSTEM);
            return m;
        }

        VC_WARNING_CRITICAL("ModuleFactory::Unable to create module of unknown type " + toStdString(type));

        return NULL;
    }
}

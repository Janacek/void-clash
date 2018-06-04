/********************************************************************
**
**              EntityPlayer.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_ENTITY_PLAYER
#define VOID_CLASH_ENTITY_PLAYER

#include <list>
#include <string>
#include "AEntity.hpp"
#include "Text.hpp"
#include "Quad.hpp"
#include "FontManager.hpp"
#include "GraphicHelper.hpp"

class Player;

namespace VoidClashGraphics
{
    // Handle the display of PV lost
    struct PVLost
    {
        /////////////////////////////////////////////////////////////////////
        /////	Contruction
        /////////////////////////////////////////////////////////////////////

        PVLost(float damageIN, ogl::Camera *camera)
        {
            damage = damageIN;
            timer = 0;
            pvLost = new ogl::Text();
            pvLost->create(S_Font->getFont("UnispaceRegular.ttf"),
                std::to_string(damage), camera);
        }

        /////////////////////////////////////////////////////////////////////
        /////	Destruction
        /////////////////////////////////////////////////////////////////////

        ~PVLost(void)
        {
            pvLost->destroy();
            safeDelete(pvLost);
        }

        /////////////////////////////////////////////////////////////////////
        /////	Copy (when the Player:set function is called)
        /////////////////////////////////////////////////////////////////////

        PVLost* copy(ogl::Camera *camera)
        {
            PVLost *newPvLost = new PVLost(this->damage, camera);
            newPvLost->timer = this->timer;
            newPvLost->position = this->position;
            return newPvLost;
        }

    	float timer;
        int damage;
        ogl::Text *pvLost;
        glm::vec2 position;
    };

    class EntityPlayer : public AEntity
    {
    public:
        EntityPlayer(void);
        virtual ~EntityPlayer(void);

        virtual void start(void);
        virtual void update(float deltatime);
        virtual void postRendering(float deltatime, float renderPass);
        virtual void stop(void);

        // Copy the content of the AEntity passed in parameter to this
        virtual void set(AEntity *entity);

        // Return the AssetId to use
        virtual unsigned int getAssetId(void);

    private:
        void updateGeometry(float deltatime);
        void updateColor(void);
        void updateRotationAnimation(float deltatime);

        // Post rendering functions
        void updateLife(float deltatime);
        void updateNickname(void);
        void updatePVLost(float deltatime);
        void updateLevelsBar(float deltatime);

        // Utils
        void deletePVLost(void);

    private:
        std::shared_ptr<Player> _player;

        // life
        float _realLife;
        float _smoothLife;
        float _lifeDelay;

        // Rotation animation
        float _rotationTarget;
        float _currentRotation;
        float _lastDiff;

        // Life and energy
        ogl::Quad *_levelsBar;

        // Nickname
        ogl::Text *_nickname;

        // PV lost
        std::list<PVLost*> _pvLost;
    };
}

#endif /* end of include guard: VOID_CLASH_ENTITY_PLAYER */

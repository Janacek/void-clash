/********************************************************************
**
**              EntityRespawn.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_ENTITY_RESPAWN
#define VOID_CLASH_ENTITY_RESPAWN

#include "AEntity.hpp"

class Respawn;

namespace VoidClashGraphics
{
    class EntityRespawn : public AEntity
    {
    public:
        EntityRespawn(void);
        virtual ~EntityRespawn(void);

        virtual void start(void);
        virtual void update(float deltatime);
        virtual void stop(void);

        // Copy the content of the AEntity passed in parameter to this
        virtual void set(AEntity *entity);

        // Return the AssetId to use
        virtual unsigned int getAssetId(void);

    private:
        void updateColor(void);

    private:
        std::shared_ptr<Respawn> _respawn;
    };
}

#endif /* end of include guard: VOID_CLASH_ENTITY_RESPAWN */

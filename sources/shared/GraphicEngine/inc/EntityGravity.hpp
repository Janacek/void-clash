/********************************************************************
**
**              EntityGravity.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_ENTITY_GRAVITY
#define VOID_CLASH_ENTITY_GRAVITY

#include "AEntity.hpp"

class GravityField;

namespace VoidClashGraphics
{
    class EntityGravity : public AEntity
    {
    public:
        EntityGravity(void);
        virtual ~EntityGravity(void);

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
        std::shared_ptr<GravityField> _gravity;
    };
}

#endif /* end of include guard: VOID_CLASH_ENTITY_GRAVITY */

/********************************************************************
**
**              EntityLight.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_ENTITY_LIGHT
#define VOID_CLASH_ENTITY_LIGHT

#include "AEntity.hpp"

namespace VoidClashGraphics
{
    class EntityLight : public AEntity
    {
    public:
        EntityLight(void);
        virtual ~EntityLight(void);

        virtual void start(void);
        virtual void update(float deltatime);
        virtual void stop(void);

        // Copy the content of the AEntity passed in parameter to this
        virtual void set(AEntity *entity);

    private:
        void updateColor(void);
    };
}

#endif /* end of include guard: VOID_CLASH_ENTITY_LIGHT */

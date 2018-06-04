/********************************************************************
**
**              EntitySpeedfield.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_ENTITY_SPEEDFIELD
#define VOID_CLASH_ENTITY_SPEEDFIELD

#include "AEntity.hpp"

class SpeedField;

namespace VoidClashGraphics
{
    class EntitySpeedfield : public AEntity
    {
    public:
        EntitySpeedfield(void);
        virtual ~EntitySpeedfield(void);

        virtual void start(void);
        virtual void update(float deltatime);
        virtual void stop(void);

        // Copy the content of the AEntity passed in parameter to this
        virtual void set(AEntity *entity);

    private:
        void updateColor(void);

    private:
        std::shared_ptr<SpeedField> _speedfield;
    };
}

#endif /* end of include guard: VOID_CLASH_ENTITY_SPEEDFIELD */

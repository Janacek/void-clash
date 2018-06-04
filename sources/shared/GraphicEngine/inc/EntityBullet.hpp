/********************************************************************
**
**              EntityBullet.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_ENTITY_BULLET
#define VOID_CLASH_ENTITY_BULLET

#include "AEntity.hpp"

class Bullet;

namespace VoidClashGraphics
{
    class EntityBullet : public AEntity
    {
    public:
        EntityBullet(void);
        virtual ~EntityBullet(void);

        virtual void start(void);
        virtual void update(float deltatime);
        virtual void stop(void);

        // Copy the content of the AEntity passed in parameter to this
        virtual void set(AEntity *entity);

    private:
        void updateColor(void);

    private:
        std::shared_ptr<Bullet> _bullet;
    };
}

#endif /* end of include guard: VOID_CLASH_ENTITY_BULLET */

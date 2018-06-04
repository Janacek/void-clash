/********************************************************************
**
**              EntityBomb.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_ENTITY_BOMB
#define VOID_CLASH_ENTITY_BOMB

#include "AEntity.hpp"

class Bomb;

namespace VoidClashGraphics
{
    class EntityBomb : public AEntity
    {
    public:
        EntityBomb(void);
        virtual ~EntityBomb(void);

        virtual void start(void);
        virtual void update(float deltatime);
        virtual void stop(void);

        // Copy the content of the AEntity passed in parameter to this
        virtual void set(AEntity *entity);

    private:
        void updateColor(void);

    private:
        std::shared_ptr<Bomb> _bomb;
    };
}

#endif /* end of include guard: VOID_CLASH_ENTITY_BOMB */

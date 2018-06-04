/********************************************************************
**
**              EntityWall.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_ENTITY_WALL
#define VOID_CLASH_ENTITY_WALL

#include "AEntity.hpp"

class Wall;

namespace VoidClashGraphics
{
    class EntityWall : public AEntity
    {
    public:
        EntityWall(void);
        virtual ~EntityWall(void);

        virtual void start(void);
        virtual void update(float deltatime);
        virtual void stop(void);

        // Copy the content of the AEntity passed in parameter to this
        virtual void set(AEntity *entity);

    private:
        void updateColor(void);

    private:
        std::shared_ptr<Wall> _wall;
    };
}

#endif /* end of include guard: VOID_CLASH_ENTITY_WALL */

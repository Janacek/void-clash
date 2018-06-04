/********************************************************************
**
**              EntityFlag.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_ENTITY_FLAG
#define VOID_CLASH_ENTITY_FLAG

#include <list>
#include "AEntity.hpp"

class Flag;

namespace VoidClashGraphics
{
    class EntityFlag : public AEntity
    {
    public:
        EntityFlag(void);
        virtual ~EntityFlag(void);

        virtual void start(void);
        virtual void update(float deltatime);
        virtual void stop(void);

        // Copy the content of the AEntity passed in parameter to this
        virtual void set(AEntity *entity);

    private:
        void updateColor(void);

    private:
        std::shared_ptr<Flag> _flag;

        // Make the flag follow the Player
        float _timer;
        bool _gotOwnerLastFrame;
        glm::vec2 _initialPosition;
        std::list<glm::vec2> _ownerPosition;
    };
}

#endif /* end of include guard: VOID_CLASH_ENTITY_FLAG */

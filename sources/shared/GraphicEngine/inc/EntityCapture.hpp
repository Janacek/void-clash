/********************************************************************
**
**              EntityCapture.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_ENTITY_CAPTURE
#define VOID_CLASH_ENTITY_CAPTURE

#include "AEntity.hpp"
#include "Quad.hpp"

class Capture;

namespace VoidClashGraphics
{
    class EntityCapture : public AEntity
    {
    public:
        EntityCapture(void);
        virtual ~EntityCapture(void);

        virtual void start(void);
        virtual void update(float deltatime);
        virtual void postRendering(float deltatime, float renderPass);
        virtual void stop(void);

        // Copy the content of the AEntity passed in parameter to this
        virtual void set(AEntity *entity);

    private:
        void updateColor(void);

    private:
        std::shared_ptr<Capture> _capture;

        // Progress bar
        ogl::Quad *_progress;
    };
}

#endif /* end of include guard: VOID_CLASH_ENTITY_CAPTURE */

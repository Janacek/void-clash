/********************************************************************
**
**              Module.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_MODULE
#define VOID_CLASH_MODULE

#include "ComponentManager.hpp"
#include "ChildManager.hpp"

namespace VoidClashGraphics
{
    class AModule
    {
    public:
        AModule(void) {}
        virtual ~AModule(void) {}

        virtual void start(void) = 0;
        virtual void update(float deltatime) = 0;
        virtual void postRendering(float deltatime, int renderPass) = 0;
        virtual void stop(void) = 0;

        // Setters
        virtual void setActive(bool active) = 0;

        // Types
		virtual void addType(gObjectType type) = 0;
		virtual std::vector<gObjectType>& getTypeChild(void) = 0;
		virtual void addType(eventType type) = 0;
		virtual std::vector<eventType>& getTypeEvent(void) = 0;

        // Camera
        virtual void setCamera(ogl::Camera *camera)
        {
            _camera = camera;
        }

    protected:
        // Camera use to display childs
        ogl::Camera *_camera;

    };

    template<class T>
    class Module : public AModule, public ComponentManager, public ChildManager<T>
    {
    public:
        Module(void);
        virtual ~Module(void);

        virtual void start(void);
        virtual void update(float deltatime);
        virtual void postRendering(float deltatime, int renderPass);
        virtual void stop(void);

        // Setters
        virtual void setActive(bool active);

        // Types
		virtual void addType(gObjectType type);
		virtual std::vector<gObjectType>& getTypeChild(void);
		virtual void addType(eventType type);
		virtual std::vector<eventType>& getTypeEvent(void);

        // Wrappers
        virtual void setSize(unsigned int size);

    protected:
        virtual void render(unsigned int assetId);
		virtual bool updateEntity(T &entity, float deltatime, unsigned int index);

    protected:
        bool _active;
    };
}

#endif /* end of include guard: VOID_CLASH_MODULE */

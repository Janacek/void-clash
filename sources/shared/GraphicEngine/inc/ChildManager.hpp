/********************************************************************
**
**              ChildManager.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_CHILD_MANAGER
#define VOID_CLASH_CHILD_MANAGER

#include <map>
#include "AEntity.hpp"
#include "GraphicHelper.hpp"

namespace VoidClashGraphics
{
    template<class T>
    class ChildManager
    {
    public:
        ChildManager(void);
        virtual ~ChildManager(void);

        virtual void start(void);
        virtual void update(void);
        virtual void stop(void);

        virtual void resize(unsigned int minSize);

		// Child type
		virtual std::vector<gObjectType>& getChildType(void);
        virtual void addChildType(gObjectType type);

		// Event type
		virtual std::vector<eventType>& getEventType(void);
		virtual void addEventType(eventType type);

        // Check events type
        virtual bool isConcernedBy(std::shared_ptr<AObject> gameObject);
		virtual bool isConcernedBy(eventType ev);

    protected:
        // Childs handler
		virtual void lookForGameObjects(void);
		virtual void addChild(std::shared_ptr<AObject> gameObject);
		virtual void addChild(std::pair<eventType, s_event> *event);
		virtual void removeChild(std::shared_ptr<AObject> gameObject);
        virtual void removeChildAt(unsigned int index);

        // Wrappers
        std::vector<T>& getChilds(void);
        unsigned int getNextSlot(void);
        void setAModuleCamera(ogl::Camera *camera);

    protected:
        // Keep the last active entity of the container
        unsigned int _nextSlot;

        // Children
        std::vector<T> _childs;

        // The children types
        std::vector<gObjectType> _types;

		// The child events
		std::vector<eventType> _evTypes;

        // Camera pointer from AModule
        ogl::Camera *_aModuleCamera;
    };
}

#endif /* end of include guard: VOID_CLASH_CHILD_MANAGER */

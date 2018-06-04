/********************************************************************
**
**              Scene.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_SCENE
#define VOID_CLASH_SCENE

#include <list>
#include <string>
#include "Camera.hpp"
#include "Module.hpp"
#include "ModuleFactory.hpp"

namespace VoidClashGraphics
{
    class Scene
    {
    public:
        Scene(const std::string &name);
        virtual ~Scene(void);

        virtual void start(void);
        virtual void update(float deltatime, int renderPass);
        virtual void stop(void);

        // Modules management
        virtual void addModule(gObjectType type);

        // Camera
        virtual void setCamera(ogl::Camera *camera);
        virtual void updateCamera(float deltatime);
        virtual void disableCameraAutoUpdate(void);

    private:
        // Scene camera
        ogl::Camera *_camera;
        bool _autoUpdateCamera;

        // Name of the Scene
        std::string _name;

        // Modules
        std::list<AModule*> _childs;

        // State
        bool _active;
    };
}

#endif /* end of include guard: VOID_CLASH_SCENE */

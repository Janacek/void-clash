/********************************************************************
**
**              Environment.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_ENVIRONMENT
#define VOID_CLASH_ENVIRONMENT

#include "Module.hpp"
#include "ShaderManager.hpp"
#include "ModelManager.hpp"
#include "Cubemap.hpp"

namespace VoidClashGraphics
{
    class Environment : public AModule
    {
    public:
        Environment(void);
        virtual ~Environment(void);

        virtual void start(void);
        virtual void update(float deltatime);
        virtual void postRendering(float deltatime, int renderPass);
        virtual void stop(void);

        // Setters
        virtual void setActive(bool active);

        // Types
		virtual void addType(gObjectType type);
		virtual std::vector<gObjectType>& getTypeChild(void);
		virtual void addType(eventType type) { (void)type; };
		virtual std::vector<eventType>& getTypeEvent(void) { return _evTypes; };

    private:
        // Refresh the camera
        virtual void updateCamera(void);

    private:
        bool _refreshCamera;

		std::vector<eventType> _evTypes;
		std::vector<gObjectType> _types;

        bool _active;
        gObjectType _type;

        // Skybox
        float _angle;
        ogl::Shader *_skyboxShader;
        ogl::Model *_skybox;
        ogl::Cubemap *_cubemap;
    };
}

#endif /* end of include guard: VOID_CLASH_ENVIRONMENT */

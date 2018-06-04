/********************************************************************
**
**              AEntity.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_A_ENTITY
#define VOID_CLASH_A_ENTITY

#include "Transformable.hpp"
#include "AObject.hpp"
#include "ComponentManager.hpp"
#include "Camera.hpp"
#include "Log.hpp"
#include "Event.hpp"

namespace VoidClashGraphics
{
    class AEntity : public ogl::Transformable
    {
    public:
        AEntity(void);
        virtual ~AEntity(void);

        virtual void start(void);
        virtual void update(float deltatime);
        virtual void postRendering(float deltatime, float renderPass);
        virtual void stop(void);

        // Check the state of the entity
        virtual bool good(void);
        virtual bool skipThisFrame(void);

        // Game object
        virtual void setGameObject(const std::shared_ptr<AObject> &gameObject);
        virtual const std::shared_ptr<AObject>& getGameObject(void);

		// Event
		virtual void setEvent(std::pair<eventType, s_event> *event);

        // Set the scene camera
        virtual void setCamera(ogl::Camera *camera);

        // Get instances
        virtual float getFloat(void);
        virtual const glm::mat4& getMatrix(void);
        virtual const glm::vec4& getVec4(void);
        virtual const glm::vec2& getVec2(void);

        // Copy the content of the AEntity passed in parameter to this
        virtual void set(AEntity *entity);

        // Return the AssetId to use
        virtual unsigned int getAssetId(void);

    protected:
        // Check if the gameObject still exists
        virtual bool checkParent(void);
        // Check if the gameObject owner still exists
        virtual bool checkOwner(void);

    public: // Public here to be able to read attributes from set() functions
        // Current status
        bool _good;
        bool _skipThisFrame;

        // Game object
        std::shared_ptr<AObject> _gameObject;

		// Event (can only be used one time to fetch datas,
		// will be NULL otherwise !)
		std::pair<eventType, s_event> *_event;

			// Camera
        ogl::Camera *_camera;

        // Components
        float _float;
        glm::mat4 _matrix;
        glm::vec4 _vec4;
        glm::vec2 _vec2;
    };
}

#endif /* end of include guard: VOID_CLASH_A_ENTITY */

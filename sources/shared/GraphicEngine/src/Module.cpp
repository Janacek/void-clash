
/********************************************************************
**
**              Module.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "Module.hpp"
#include "Log.hpp"
#include "GraphicConfiguration.hpp"
#include "ShaderManager.hpp"
#include "TextureManager.hpp"
#include "Templates.hpp"

namespace VoidClashGraphics
{
    /////////////////////////////////////////////////////////////////////
    /////	Ctor/Dtor
    /////////////////////////////////////////////////////////////////////

    template<class T>
    Module<T>::Module(void) : ComponentManager(), ChildManager<T>()
    {
        _active = false;
		_shader = NULL;
    }

    template<class T>
    Module<T>::~Module(void)
    {

    }

    /////////////////////////////////////////////////////////////////////
    /////	Start
    /////////////////////////////////////////////////////////////////////

    template<class T>
    void Module<T>::start(void)
    {
        for (auto it = ChildManager<T>::_types.begin();
            it != ChildManager<T>::_types.end(); ++it)
        {
            VC_INFO("Module::Starting " + toStdString(*it));
        }

        ComponentManager::start();
        ChildManager<T>::setAModuleCamera(_camera);
        ChildManager<T>::start();

        _active = true;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Stop
    /////////////////////////////////////////////////////////////////////

    template<class T>
    void Module<T>::stop(void)
    {
        for (auto it = ChildManager<T>::_types.begin();
            it != ChildManager<T>::_types.end(); ++it)
        {
            VC_INFO("Module::Stopping " + toStdString(*it));
        }

        ComponentManager::stop();
        ChildManager<T>::stop();

        _active = false;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update
    /////////////////////////////////////////////////////////////////////

    template<class T>
    void Module<T>::update(float deltatime)
    {
        if (_active == false)
            return;

        // Refresh containers based on deletion/creation events
        ChildManager<T>::update();

        // Empty instances
        ComponentManager::emptyInstances();

        // Update all entities
        unsigned int current = 0;
		unsigned int entityToDisplay = 0;

        while (current < this->getNextSlot() && current < this->getChilds().size())
        {
			if (updateEntity(this->getChilds()[current], deltatime, current) == true)
			{
				++entityToDisplay;
			}
            ++current;
        }

        // We have no entities to display, we are done
		if (entityToDisplay == 0)
        {
            return;
        }

        // ----- Render all entites grouped by assetId ----- //

        // We defined some assets
        if (_assetsId.size() != 0)
        {
            for (auto it = _assetsId.begin(); it != _assetsId.end(); ++it)
            {
                render(*it);
            }
        }

        // We don't have assets, use the COMMON_ASSET_ID group
        else
        {
            render(COMMON_ASSET_ID);
        }
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update and entity and save its instanced attributes
    /////////////////////////////////////////////////////////////////////

    template<class T>
    bool Module<T>::updateEntity(T &entity, float deltatime, unsigned int index)
    {
        // Update the entity
        entity.update(deltatime);

        // Check the state of the entity
        if (entity.good() == false)
        {
			// Remove the entity
			this->removeChildAt(index);
			return false;
        }

		// Skip the display for this frame
		if (entity.skipThisFrame() == true)
		{
			return false;
		}

        // Get the id to use
        unsigned int id = entity.getAssetId();

        _floats[id].push_back(entity.getFloat());
        _matrices[id].push_back(entity.getMatrix());
        _vec4s[id].push_back(entity.getVec4());
        _vec2s[id].push_back(entity.getVec2());

		return true;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Render instances of a group base of their assetId
    /////////////////////////////////////////////////////////////////////

    template<class T>
    void Module<T>::render(unsigned int assetId)
    {
        // Update the shader parameters
		if (_shader == NULL)
		{
			return;
		}
        _shader->bind();
        _shader->setParameter("Camera", _camera->getFinal());

        // Inject camera Position
        if (_shaderNeedCameraPosition == true)
        {
            _shader->setParameter("CameraPos", _camera->getEye());
        }

		// Depth test
		if (_depthTest == false)
		{
			glDisable(GL_DEPTH_TEST);
		}
		else
		{
			glEnable(GL_DEPTH_TEST);
		}

		// Blend mode
		if (_accumBlend == true)
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		}
		else
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		// ---------- 2D Sprite ---------- //

		if (_isTexture == true ||
            (_isTexture == false && _isModel == false))
        {
            // Do we really use a texture or is it fully shader based ?
            if (_textures.size() != 0)
            {
                _textures[assetId]->bind();
            }

            // Update instances
            // (ogl::Instanced is responsible for 0 sized or disabled attributes)
            _quad->updateFloatBuffer(_floats[assetId]);
            _quad->updateMatrixBuffer(_matrices[assetId]);
            _quad->updateVec4Buffer(_vec4s[assetId]);
            _quad->updateVec2Buffer(_vec2s[assetId]);

            // Draw
			_quad->draw();
        }

        // ---------- 3D Model ---------- //

        else
        {
            // Update instances
            // (ogl::Instanced is responsible for 0 sized or disabled attributes)
            _models[assetId]->updateFloatBuffer(_floats[assetId]);
            _models[assetId]->updateMatrixBuffer(_matrices[assetId]);
            _models[assetId]->updateVec4Buffer(_vec4s[assetId]);
            _models[assetId]->updateVec2Buffer(_vec2s[assetId]);

			_models[assetId]->draw();
        }

        // Clean
        S_Shader->unbindShader();
        S_Texture->unbindTexture();

        GL_CHECK;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Post rendering
    /////////////////////////////////////////////////////////////////////

    template<class T>
    void Module<T>::postRendering(float deltatime, int pass)
    {
        unsigned int current = 0;

        // Call post rendering function for all childs
        while (current < this->getNextSlot() && current < this->getChilds().size())
        {
            this->getChilds()[current].postRendering(deltatime, pass);
            ++current;
        }
    }

    /////////////////////////////////////////////////////////////////////
    /////	Setters
    /////////////////////////////////////////////////////////////////////

    template<class T>
    void Module<T>::setActive(bool active)
    {
        _active = active;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Wrappers
    /////////////////////////////////////////////////////////////////////

    template<class T>
    void Module<T>::addType(gObjectType type)
    {
        this->addChildType(type);
    }

    template<class T>
    std::vector<gObjectType>& Module<T>::getTypeChild(void)
    {
        return this->getChildType();
    }

	template<class T>
	void Module<T>::addType(eventType type)
	{
		this->addEventType(type);
	}

	template<class T>
	std::vector<eventType>& Module<T>::getTypeEvent(void)
	{
		return this->getEventType();
	}

	template<class T>
    void Module<T>::setSize(unsigned int size)
    {
        this->resize(size);
    }

    /////////////////////////////////////////////////////////////////////
    /////	Template intances
    /////////////////////////////////////////////////////////////////////

    template class Module<EntityBullet>;
    template class Module<EntityRespawn>;
    template class Module<EntitySpeedfield>;
    template class Module<EntityWall>;
    template class Module<EntityPlayer>;
    template class Module<EntityGravity>;
    template class Module<EntityFlag>;
    template class Module<EntityBomb>;
    template class Module<EntityLight>;
    template class Module<EntityCapture>;
	template class Module<EntityFlash>;
}

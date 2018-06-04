/********************************************************************
**
**              ComponentManager.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "ComponentManager.hpp"
#include "ModelManager.hpp"
#include "TextureManager.hpp"
#include "Log.hpp"
#include "GraphicHelper.hpp"

namespace VoidClashGraphics
{
    /////////////////////////////////////////////////////////////////////
    /////	Ctor/Dtor
    /////////////////////////////////////////////////////////////////////

    ComponentManager::ComponentManager(void)
    {
        _isModel = false;
        _isTexture = false;
        _shaderNeedCameraPosition = false;
		_depthTest = true;
		_accumBlend = false;

		// Default asset
        _quad = new ogl::Quad();
    }

    ComponentManager::~ComponentManager(void)
    {
        if (_quad != NULL)
        {
            safeDelete(_quad);
        }
    }

    /////////////////////////////////////////////////////////////////////
    /////	Start
    /////////////////////////////////////////////////////////////////////

    void ComponentManager::start(void)
    {
        // Create quad instance
        if (_isTexture == true ||
            (_isTexture == false && _isModel == false))
        {
            _quad->create();
        }

        // Load models and create instance
        else
        {
            for (auto it = _models.begin(); it != _models.end(); ++it)
            {
                it->second->create();
            }
        }
    }

    /////////////////////////////////////////////////////////////////////
    /////	Stop
    /////////////////////////////////////////////////////////////////////

    void ComponentManager::stop(void)
    {
        // Clear models, textures, and assetId cache
        _models.clear();
        _textures.clear();
        _assetsId.clear();

        // Delete instanced component
        if (_quad != NULL)
        {
            _quad->destroy();
        }

        _isModel = false;
        _isTexture = true;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Display type (don't mix both !)
    /////////////////////////////////////////////////////////////////////

    void ComponentManager::addModel(unsigned int assetId, const std::string &modelPath)
    {
        ogl::Model *model = S_Model->addModel(modelPath);

        // Fail to load the model
        if (model == NULL)
        {
            VC_WARNING("ComponentManager::Unable to add model " + modelPath);
            return ;
        }

        // Check if we didn't already added the model
        if (_models.find(assetId) != _models.end())
        {
            VC_INFO("ComponentManager::Model " + modelPath + " has already been added");
            return;
        }

        // Add the model to the map
        _models[assetId] = model;

        // We will use 3D model
        if (_isTexture == true)
            VC_WARNING_CRITICAL("ComponentManager::Trying to mix texture and models, you fucked up");

        _isModel = true;
        _isTexture = false;

        cacheAssetIdUnique(assetId);
    }

    void ComponentManager::addTexture(unsigned int assetId, const std::string &texturePath)
    {
        ogl::Texture *texture = S_Texture->addTexture(texturePath);

        // Fail to load the texture
        if (texture == NULL)
        {
            VC_WARNING("ComponentManager::Unable to add texture " + texturePath);
            return;
        }

        // Check if we didn't already added the texture
        if (_textures.find(assetId) != _textures.end())
        {
            VC_INFO("ComponentManager::Texture " + texturePath + " has already been added");
            return;
        }

        // Add the model to the map
        _textures[assetId] = texture;

        // We will use 2D texture
        if (_isModel == true)
            VC_WARNING_CRITICAL("ComponentManager::Trying to mix texture and models, you fucked up");

        _isModel = false;
        _isTexture = true;

        cacheAssetIdUnique(assetId);
    }

    /////////////////////////////////////////////////////////////////////
    /////	Empty all instances
    /////////////////////////////////////////////////////////////////////

    void ComponentManager::emptyInstances(void)
    {
        for (auto it = _floats.begin(); it != _floats.end(); ++it)
        {
            it->second.clear();
        }

        for (auto it = _matrices.begin(); it != _matrices.end(); ++it)
        {
            it->second.clear();
        }

        for (auto it = _vec4s.begin(); it != _vec4s.end(); ++it)
        {
            it->second.clear();
        }

        for (auto it = _vec2s.begin(); it != _vec2s.end(); ++it)
        {
            it->second.clear();
        }
    }

    /////////////////////////////////////////////////////////////////////
    /////	Cache the assetId only once
    /////////////////////////////////////////////////////////////////////

    void ComponentManager::cacheAssetIdUnique(unsigned int assetId)
    {
        // Look for the assetId in the cache
        for (auto it = _assetsId.begin(); it != _assetsId.end(); ++it)
        {
            if (assetId == *it)
                return;
        }

        // Not found, add the assetId into the cache
        _assetsId.push_back(assetId);
    }

    /////////////////////////////////////////////////////////////////////
    /////	Wrappers
    /////////////////////////////////////////////////////////////////////

    void ComponentManager::setMatrixBuffer(int bindingIndex)
    {
        // 2D Sprite
        if (_isTexture == true ||
            (_isTexture == false && _isModel == false))
        {
            _quad->setInstanced(true);
            _quad->setMatrixBuffer(bindingIndex);
        }

        // 3D model
        else
        {
            for (auto it = _models.begin(); it != _models.end(); ++it)
            {
                it->second->setInstanced(true);
                it->second->setMatrixBuffer(bindingIndex);
            }
        }
    }

    void ComponentManager::setFloatBuffer(int bindingIndex)
    {
        // 2D Sprite
        if (_isTexture == true ||
            (_isTexture == false && _isModel == false))
        {
            _quad->setInstanced(true);
            _quad->setFloatBuffer(bindingIndex);
        }

        // 3D model
        else
        {
            for (auto it = _models.begin(); it != _models.end(); ++it)
            {
                it->second->setInstanced(true);
                it->second->setFloatBuffer(bindingIndex);
            }
        }
    }

    void ComponentManager::setVec2Buffer(int bindingIndex)
    {
        // 2D Sprite
        if (_isTexture == true ||
            (_isTexture == false && _isModel == false))
        {
            _quad->setInstanced(true);
            _quad->setVec2Buffer(bindingIndex);
        }

        // 3D model
        else
        {
            for (auto it = _models.begin(); it != _models.end(); ++it)
            {
                it->second->setInstanced(true);
                it->second->setVec2Buffer(bindingIndex);
            }
        }
    }

    void ComponentManager::setvec3Buffer(int bindingIndex)
    {
        // 2D Sprite
        if (_isTexture == true ||
            (_isTexture == false && _isModel == false))
        {
            _quad->setInstanced(true);
            _quad->setvec3Buffer(bindingIndex);
        }

        // 3D model
        else
        {
            for (auto it = _models.begin(); it != _models.end(); ++it)
            {
                it->second->setInstanced(true);
                it->second->setvec3Buffer(bindingIndex);
            }
        }
    }

    void ComponentManager::setVec4Buffer(int bindingIndex)
    {
        // 2D Sprite
        if (_isTexture == true ||
            (_isTexture == false && _isModel == false))
        {
            _quad->setInstanced(true);
            _quad->setVec4Buffer(bindingIndex);
        }

        // 3D model
        else
        {
            for (auto it = _models.begin(); it != _models.end(); ++it)
            {
                it->second->setInstanced(true);
                it->second->setVec4Buffer(bindingIndex);
            }
        }
    }

    /////////////////////////////////////////////////////////////////////
    /////	Shader
    /////////////////////////////////////////////////////////////////////

    void ComponentManager::setShader(const std::string &shader)
    {
        _shaderPath = shader;
        _shader = S_Shader->addShader(_shaderPath);
    }

    void ComponentManager::injectCameraPositionUniform(void)
    {
        _shaderNeedCameraPosition = true;
    }

	/////////////////////////////////////////////////////////////////////
	/////	OpenGL states
	/////////////////////////////////////////////////////////////////////

	void ComponentManager::disableDepthTest(void)
	{
		_depthTest = false;
	}

	void ComponentManager::enableCumulativeBlend(void)
	{
		_accumBlend = true;
	}

}

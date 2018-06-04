/********************************************************************
**
**              ComponentManager.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_COMPONENT_MANAGER
#define VOID_CLASH_COMPONENT_MANAGER

#include <map>
#include "Quad.hpp"
#include "Model.hpp"
#include "Instanced.hpp"
#include "ShaderManager.hpp"

namespace VoidClashGraphics
{
    // When we add models or textures, we need to define an assetId
    // related to this ressources. For example, we will say that a
    // player is using the assetId = 7, so we will use the model/texture
    // with 7 as assetId.
    // Since all modules doesn't need differents ressources, we define
    // here the assetId which will be used as the common ressource For
    // all entites of the module
    #define COMMON_ASSET_ID 19052015 // <3

    class ComponentManager
    {
    public:
        ComponentManager(void);
        virtual ~ComponentManager(void);

        virtual void start(void);
        virtual void stop(void);

        // Display type (don't mix both !)
        void addModel(unsigned int assetId, const std::string &modelPath);
        void addTexture(unsigned int assetId, const std::string &texturePath);

        // Shader
        void setShader(const std::string &path);
        void injectCameraPositionUniform();

        // Wrap ogl::Instanced functions
        virtual void setMatrixBuffer(int bindingIndex);
        virtual void setFloatBuffer(int bindingIndex);
        virtual void setVec2Buffer(int bindingIndex);
        virtual void setvec3Buffer(int bindingIndex);
        virtual void setVec4Buffer(int bindingIndex);

		// OpenGL states
		void disableDepthTest(void);
		void enableCumulativeBlend(void);

        // Clear temporary sorting buffers
        virtual void emptyInstances(void);

    protected:
        virtual void cacheAssetIdUnique(unsigned int assetId);

    protected:
        // Models (VAO index - Model)
        bool _isModel;
        std::map<unsigned int, ogl::Model*> _models;

        // Textures (VAO index - Texture)
        ogl::Quad *_quad;
        bool _isTexture;
        std::map<unsigned int, ogl::Texture*> _textures;

        // Shader to use for drawing
        std::string _shaderPath;
        ogl::Shader *_shader;

        // Shader uniforms
        bool _shaderNeedCameraPosition;

        // Buffers used to sort children instances attributes during update
        std::map<unsigned int, std::vector<float>> _floats;
        std::map<unsigned int, std::vector<glm::mat4>> _matrices;
        std::map<unsigned int, std::vector<glm::vec4>> _vec4s;
        std::map<unsigned int, std::vector<glm::vec2>> _vec2s;

        // Cache the assetId to use
        std::vector<unsigned int> _assetsId;

		// OpenGL states
		bool _depthTest;
		bool _accumBlend;
    };
}

#endif /* end of include guard: VOID_CLASH_COMPONENT_MANAGER */

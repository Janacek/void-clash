/********************************************************************
**
**              Instanced.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef OGL_INSTANCED
#define OGL_INSTANCED

#include <vector>
#include "VoidClash_OpenGL.h"
#include "glm/glm.hpp"

namespace ogl
{
    #define INVALID_INSTANCES_NUMBER 9999999

    struct InstancedAttribsBuffer
    {
        InstancedAttribsBuffer(void)
        {
            enabled = false;
            buffer = 0;
            bindingIndex = -1;
        }

        bool enabled;
        GLuint buffer;
        int bindingIndex;
    };

    class Instanced
    {
    public:
        Instanced(void);
        virtual ~Instanced(void);

        virtual void create(void);
        virtual void destroy(void);

        // Set instanced buffer to use
        virtual void setMatrixBuffer(int bindingIndex);
        virtual void setFloatBuffer(int bindingIndex);
        virtual void setVec2Buffer(int bindingIndex);
        virtual void setvec3Buffer(int bindingIndex);
        virtual void setVec4Buffer(int bindingIndex);

        // Update instanced buffer
        virtual void updateFloatBuffer(const std::vector<float> &floats, bool updateDatas = true);
        virtual void updateVec2Buffer(const std::vector<glm::vec2> &vec2s, bool updateDatas = true);
        virtual void updateVec3Buffer(const std::vector<glm::vec3> &vec3s, bool updateDatas = true);
        virtual void updateVec4Buffer(const std::vector<glm::vec4> &vec4s, bool updateDatas = true);
        virtual void updateMatrixBuffer(const std::vector<glm::mat4> &matrices, bool updateDatas = true);

    private:
        // Create instanced buffers
        void createFloatBuffer(void);
        void createVec2Buffer(void);
        void createVec3Buffer(void);
        void createVec4Buffer(void);
        void createMatrixBuffer(void);

    protected:
        // Keep the maximum instances to draw
        unsigned int _maxInstances;

        // Instanced buffers
        InstancedAttribsBuffer _matrixBuffer;
        InstancedAttribsBuffer _floatBuffer;
        InstancedAttribsBuffer _vec2Buffer;
        InstancedAttribsBuffer _vec3Buffer;
        InstancedAttribsBuffer _vec4Buffer;
    };
}

#endif /* end of include guard: OGL_INSTANCED */

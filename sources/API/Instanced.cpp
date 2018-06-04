/********************************************************************
**
**              Instanced.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include <algorithm>
#include "Instanced.hpp"
#include "CheckError.hpp"

using namespace ogl;

/////////////////////////////////////////////////////////////////////
/////	Ctor/Dtor
/////////////////////////////////////////////////////////////////////

Instanced::Instanced(void)
{
    _maxInstances = INVALID_INSTANCES_NUMBER;
}

Instanced::~Instanced(void)
{

}

/////////////////////////////////////////////////////////////////////
/////	Create
/////////////////////////////////////////////////////////////////////

void Instanced::create(void)
{
    if (_matrixBuffer.enabled == true)
        createMatrixBuffer();
    if (_floatBuffer.enabled == true)
        createFloatBuffer();
    if (_vec2Buffer.enabled == true)
        createVec2Buffer();
    if (_vec3Buffer.enabled == true)
        createVec3Buffer();
    if (_vec4Buffer.enabled == true)
        createVec4Buffer();

    checkError();
}

/////////////////////////////////////////////////////////////////////
/////	Destroy
/////////////////////////////////////////////////////////////////////

void Instanced::destroy(void)
{
    if (_matrixBuffer.enabled == true)
        glDeleteBuffers(1, &_matrixBuffer.buffer);
    if (_floatBuffer.enabled == true)
        glDeleteBuffers(1, &_floatBuffer.buffer);
    if (_vec2Buffer.enabled == true)
        glDeleteBuffers(1, &_vec2Buffer.buffer);
    if (_vec3Buffer.enabled == true)
        glDeleteBuffers(1, &_vec3Buffer.buffer);
    if (_vec4Buffer.enabled == true)
        glDeleteBuffers(1, &_vec4Buffer.buffer);

    checkError();
}

/////////////////////////////////////////////////////////////////////
/////	Set buffer which will be used
/////////////////////////////////////////////////////////////////////

void Instanced::setMatrixBuffer(int bindingIndex)
{
    _matrixBuffer.enabled = true;
    _matrixBuffer.bindingIndex = bindingIndex;
}

void Instanced::setFloatBuffer(int bindingIndex)
{
    _floatBuffer.enabled = true;
    _floatBuffer.bindingIndex = bindingIndex;
}

void Instanced::setVec2Buffer(int bindingIndex)
{
    _vec2Buffer.enabled = true;
    _vec2Buffer.bindingIndex = bindingIndex;
}

void Instanced::setvec3Buffer(int bindingIndex)
{
    _vec3Buffer.enabled = true;
    _vec3Buffer.bindingIndex = bindingIndex;
}

void Instanced::setVec4Buffer(int bindingIndex)
{
    _vec4Buffer.enabled = true;
    _vec4Buffer.bindingIndex = bindingIndex;
}

/////////////////////////////////////////////////////////////////////
/////	Create matrix buffer
/////////////////////////////////////////////////////////////////////

void Instanced::createMatrixBuffer(void)
{
    glGenBuffers(1, &_matrixBuffer.buffer);
    glBindBuffer(GL_ARRAY_BUFFER, _matrixBuffer.buffer);

    for (int i = _matrixBuffer.bindingIndex; i < _matrixBuffer.bindingIndex + 4; ++i)
    {
         glEnableVertexAttribArray(i);
         glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
             (void*)((i - _matrixBuffer.bindingIndex) * sizeof(glm::vec4)));
         glVertexAttribDivisor(i, 1);
    }

    checkError();
}

/////////////////////////////////////////////////////////////////////
/////	Create float buffer
/////////////////////////////////////////////////////////////////////

void Instanced::createFloatBuffer(void)
{
    glGenBuffers(1, &_floatBuffer.buffer);
    glBindBuffer(GL_ARRAY_BUFFER, _floatBuffer.buffer);
    glEnableVertexAttribArray(_floatBuffer.bindingIndex);
    glVertexAttribPointer(_floatBuffer.bindingIndex, 1, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT), 0);
    glVertexAttribDivisor(_floatBuffer.bindingIndex, 1);

    checkError();
}

/////////////////////////////////////////////////////////////////////
/////	Create vec2 buffer
/////////////////////////////////////////////////////////////////////

void Instanced::createVec2Buffer(void)
{
    glGenBuffers(1, &_vec2Buffer.buffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vec2Buffer.buffer);
    glEnableVertexAttribArray(_vec2Buffer.bindingIndex);
    glVertexAttribPointer(_vec2Buffer.bindingIndex, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
    glVertexAttribDivisor(_vec2Buffer.bindingIndex, 1);

    checkError();
}

/////////////////////////////////////////////////////////////////////
/////	Create vec3 buffer
/////////////////////////////////////////////////////////////////////

void Instanced::createVec3Buffer(void)
{
    glGenBuffers(1, &_vec3Buffer.buffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vec3Buffer.buffer);
    glEnableVertexAttribArray(_vec3Buffer.bindingIndex);
    glVertexAttribPointer(_vec3Buffer.bindingIndex, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
    glVertexAttribDivisor(_vec3Buffer.bindingIndex, 1);

    checkError();
}

/////////////////////////////////////////////////////////////////////
/////	Create vec4 buffer
/////////////////////////////////////////////////////////////////////

void Instanced::createVec4Buffer(void)
{
    glGenBuffers(1, &_vec4Buffer.buffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vec4Buffer.buffer);
    glEnableVertexAttribArray(_vec4Buffer.bindingIndex);
    glVertexAttribPointer(_vec4Buffer.bindingIndex, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);
    glVertexAttribDivisor(_vec4Buffer.bindingIndex, 1);

    checkError();
}

/////////////////////////////////////////////////////////////////////
/////	Update matrix buffer
/////////////////////////////////////////////////////////////////////

void Instanced::updateMatrixBuffer(const std::vector<glm::mat4> &matrices, bool updateDatas)
{
    if (matrices.size() == 0 || _matrixBuffer.enabled == false)
        return;

    _maxInstances = std::min(_maxInstances, (unsigned int)matrices.size());

    glBindBuffer(GL_ARRAY_BUFFER, _matrixBuffer.buffer);
    if (updateDatas == true)
        glBufferData(GL_ARRAY_BUFFER, matrices.size() * sizeof(glm::mat4), &matrices[0], GL_STREAM_DRAW);

    checkError();
}

/////////////////////////////////////////////////////////////////////
/////	Update float buffer
/////////////////////////////////////////////////////////////////////

void Instanced::updateFloatBuffer(const std::vector<float> &floats, bool updateDatas)
{
    if (floats.size() == 0 || _floatBuffer.enabled == false)
        return;

    _maxInstances = std::min(_maxInstances, (unsigned int)floats.size());

    glBindBuffer(GL_ARRAY_BUFFER, _floatBuffer.buffer);
    if (updateDatas == true)
        glBufferData(GL_ARRAY_BUFFER, floats.size() * sizeof(GL_FLOAT), &floats[0], GL_STREAM_DRAW);

    checkError();
}

/////////////////////////////////////////////////////////////////////
/////	Update vec2s buffer
/////////////////////////////////////////////////////////////////////

void Instanced::updateVec2Buffer(const std::vector<glm::vec2> &vec2s, bool updateDatas)
{
    if (vec2s.size() == 0 || _vec2Buffer.enabled == false)
        return;

    _maxInstances = std::min(_maxInstances, (unsigned int)vec2s.size());

    glBindBuffer(GL_ARRAY_BUFFER, _vec2Buffer.buffer);
    if (updateDatas == true)
        glBufferData(GL_ARRAY_BUFFER, vec2s.size() * sizeof(glm::vec2), &vec2s[0], GL_STREAM_DRAW);

    checkError();
}

/////////////////////////////////////////////////////////////////////
/////	Update vec3s buffer
/////////////////////////////////////////////////////////////////////

void Instanced::updateVec3Buffer(const std::vector<glm::vec3> &vec3s, bool updateDatas)
{
    if (vec3s.size() == 0 || _vec3Buffer.enabled == false)
        return;

    _maxInstances = std::min(_maxInstances, (unsigned int)vec3s.size());

    glBindBuffer(GL_ARRAY_BUFFER, _vec3Buffer.buffer);
    if (updateDatas == true)
        glBufferData(GL_ARRAY_BUFFER, vec3s.size() * sizeof(glm::vec3), &vec3s[0], GL_STREAM_DRAW);

    checkError();
}

/////////////////////////////////////////////////////////////////////
/////	Update vec4s buffer
/////////////////////////////////////////////////////////////////////

void Instanced::updateVec4Buffer(const std::vector<glm::vec4> &vec4s, bool updateDatas)
{
    if (vec4s.size() == 0 || _vec4Buffer.enabled == false)
        return;

    _maxInstances = std::min(_maxInstances, (unsigned int)vec4s.size());

    glBindBuffer(GL_ARRAY_BUFFER, _vec4Buffer.buffer);
    if (updateDatas == true)
        glBufferData(GL_ARRAY_BUFFER, vec4s.size() * sizeof(glm::vec4), &vec4s[0], GL_STREAM_DRAW);

    checkError();
}

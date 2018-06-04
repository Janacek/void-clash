/********************************************************************
**
**              Quad.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "Quad.hpp"
#include "CheckError.hpp"

using namespace ogl;

/////////////////////////////////////////////////////////////////////
/////	Ctor/Dtor
/////////////////////////////////////////////////////////////////////

Quad::Quad(void)
{
    _created = false;
    _binded = false;
    _instanced = false;
    _vao = 0;
    _quad = 0;
}

Quad::~Quad(void)
{

}

/////////////////////////////////////////////////////////////////////
/////	Are we going to do instanced rendering ?
/////////////////////////////////////////////////////////////////////

void Quad::setInstanced(bool instanced)
{
    _instanced = instanced;
}

/////////////////////////////////////////////////////////////////////
/////	Create
/////////////////////////////////////////////////////////////////////

void Quad::create(void)
{
    if (_created == true)
        return;

    // Create the quad
    GLfloat vertices[] =
    {
        -0.5, -0.5, 0,
        0.5, -0.5, 0,
        -0.5, 0.5, 0,
        0.5, 0.5, 0
    };

    // Vertex Array Object
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    // Vertex Buffer Object
    glGenBuffers(1, &_quad);
    glBindBuffer(GL_ARRAY_BUFFER, _quad);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Instanced buffers
    if (_instanced == true)
        ogl::Instanced::create();

    glBindVertexArray(0);

    checkError();

    _created = true;
}

/////////////////////////////////////////////////////////////////////
/////	Destroy
/////////////////////////////////////////////////////////////////////

void Quad::destroy(void)
{
	if (_created == false)
	{
		return;
	}

	// Delete quad
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_quad);

    // Delete instanced buffers
    if (_instanced == true)
        ogl::Instanced::destroy();

	_created = false;

	checkError();
}

/////////////////////////////////////////////////////////////////////
/////	Bind/Unbind
/////////////////////////////////////////////////////////////////////

void Quad::bind(void)
{
    if (_binded == true)
        return;

    glBindVertexArray(_vao);

    _binded = true;

    checkError();
}

void Quad::unbind(void)
{
    if (_binded == false)
        return;

    glBindVertexArray(0);

    _binded = false;

    checkError();
}

/////////////////////////////////////////////////////////////////////
/////	Draw instances
/////////////////////////////////////////////////////////////////////

void Quad::draw(void)
{
    bind();

    // Instanced
    if (_instanced == true && _maxInstances != INVALID_INSTANCES_NUMBER)
    {
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _maxInstances);

        // Reset the max instances
        _maxInstances = INVALID_INSTANCES_NUMBER;

        checkError();
    }

    // Draw only once
    else
    {
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        checkError();
    }

    unbind();
}

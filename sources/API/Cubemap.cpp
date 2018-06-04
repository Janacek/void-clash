/********************************************************************
**
**              Cubemap.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include <map>
#include "SOIL2.h"
#include "Cubemap.hpp"
#include "Log.hpp"
#include "CheckError.hpp"

using namespace ogl;

static GLuint currentCubemap = 0;

/////////////////////////////////////////////////////////////////////
/////	Ctor/Dtor
/////////////////////////////////////////////////////////////////////

Cubemap::Cubemap(void)
{
    _id = 0;
}

Cubemap::~Cubemap(void)
{
    glDeleteTextures(1, &_id);
}

/////////////////////////////////////////////////////////////////////
/////	Create
/////////////////////////////////////////////////////////////////////

void Cubemap::create(const std::string &path)
{
    // Create full paths
    std::map<std::string, std::string> paths;

    paths["POSX"] = path + "_px.png";
    paths["NEGX"] = path + "_nx.png";
    paths["POSY"] = path + "_py.png";
    paths["NEGY"] = path + "_ny.png";
    paths["POSZ"] = path + "_pz.png";
    paths["NEGZ"] = path + "_nz.png";

    VC_INFO("Cubemap::Loading " + path);

    // Generate cubemap
    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _id);
    glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Load faces textures
    int width, height;

    // +X
    VC_INFO("Cubemap::Loading +X: " + paths["POSX"]);
    loadImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X, paths["POSX"], width, height);

    // -X
    VC_INFO("Cubemap::Loading -X: " + paths["NEGX"]);
    loadImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, paths["NEGX"], width, height);

    // +Y
    VC_INFO("Cubemap::Loading +Y: " + paths["POSY"]);
    loadImage(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, paths["POSY"], width, height);

    // -Y
    VC_INFO("Cubemap::Loading -Y: " + paths["NEGY"]);
    loadImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, paths["NEGY"], width, height);

    // +Z
    VC_INFO("Cubemap::Loading +Z: " + paths["POSZ"]);
    loadImage(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, paths["POSZ"], width, height);

    // +Z
    VC_INFO("Cubemap::Loading -Z: " + paths["NEGZ"]);
    loadImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, paths["NEGZ"], width, height);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    ogl::checkError();
}

/////////////////////////////////////////////////////////////////////
/////	Load image
/////////////////////////////////////////////////////////////////////

void Cubemap::loadImage(GLenum target, const std::string &path,
    int &width, int &height)
{
    // Load the image
    unsigned char *data = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_AUTO);

    // Failed ?
    if (data == NULL)
    {
        VC_WARNING_CRITICAL("Cubemap::Error while loading: " + path);
        return;
    }

    // Send data to GPU
    glTexImage2D(target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // Free memory
    SOIL_free_image_data(data);

    ogl::checkError();
}

/////////////////////////////////////////////////////////////////////
/////	Bind/Unbind
/////////////////////////////////////////////////////////////////////

void Cubemap::bind(char unit)
{
    // If the cubemap is already bind, ignore this call
    if (_id == currentCubemap)
    {
        return;
    }

    // Bind the cubemap on the good texture unit
    if (unit == 0)
    {
        glActiveTexture(GL_TEXTURE0);
    }
    else if (unit == 1)
    {
        glActiveTexture(GL_TEXTURE1);
    }
    else if (unit == 2)
    {
        glActiveTexture(GL_TEXTURE2);
    }
    else if (unit == 3)
    {
        glActiveTexture(GL_TEXTURE3);
    }
    else if (unit == 4)
    {
        glActiveTexture(GL_TEXTURE4);
    }
    else if (unit == 5)
    {
        glActiveTexture(GL_TEXTURE5);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, _id);
    currentCubemap = _id;
}

void Cubemap::unbind(void)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    currentCubemap = 0;
}

/////////////////////////////////////////////////////////////////////
/////	Return the cubemap id
/////////////////////////////////////////////////////////////////////

GLuint Cubemap::getId(void)
{
    return _id;
}

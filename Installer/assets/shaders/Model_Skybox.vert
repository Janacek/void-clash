/********************************************************************
**
**              Model_Skybox.vert
**              Created by : Vial Joris
**
********************************************************************/

#version 330

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TextureCoord;
layout(location = 2) in vec3 Normal;

uniform mat4 Camera;
uniform mat4 Model;

out vec3 TextureCoords0;

void main()
{
	gl_Position = Camera * Model * vec4(Position, 1.0);

    // Fragment shader inputs
    TextureCoords0 = Position * 2.0;
}

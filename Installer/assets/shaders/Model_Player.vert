/********************************************************************
**
**              Model_Player.vert
**              Created by : Vial Joris
**
********************************************************************/

#version 330

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TextureCoord;
layout(location = 2) in vec3 Normal;
layout(location = 3) in vec4 Color;
layout(location = 4) in mat4 Model;

uniform mat4 Camera;

out vec2 TextureCoord0;
out vec3 Normal0;
out vec3 Position0;
out vec4 Color0;

void main()
{
	gl_Position = Camera * Model * vec4(Position, 1);

    // Fragment shader inputs
    TextureCoord0 = TextureCoord;
	Normal0 = Normal;
    Position0 = vec3(Model * vec4(Position, 1.0));
    Color0 = Color;
}

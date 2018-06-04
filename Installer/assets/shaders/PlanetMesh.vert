/********************************************************************
**
**              Mesh.vert
**              Created by : Vial Joris
**
********************************************************************/

#version 330

layout (location = 0) in vec3 Position;
layout(location = 10) in vec2 TextureCoord;
layout (location = 11) in vec3 Normal;

uniform mat4 Model;
uniform mat4 Camera;

out vec2 TextureCoord0;

void main()
{
	gl_Position = Camera * Model * vec4(Position, 1);
	
        // Fragment shader inputs
        TextureCoord0 = TextureCoord;
}

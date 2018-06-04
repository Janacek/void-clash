/********************************************************************
**
**              TransparentBack.vert
**              Created by : Vial Joris
**
********************************************************************/

#version 330

layout (location = 0) in vec3 Position;

uniform mat4 Model;

void main()
{
	gl_Position = Model * vec4(Position, 1.0);
}

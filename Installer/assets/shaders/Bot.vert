/********************************************************************
**
**              Bot.vert
**              Created by : Vial Joris
**
********************************************************************/

#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in mat4 Model;

// Scene attributes
layout(std140) uniform view_matrix {
        mat4 Camera;
};

void main()
{
	gl_Position = Camera * Model * vec4(Position, 1);
}

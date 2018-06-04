/********************************************************************
**
**              GravityField.vert
**              Created by : Vial Joris
**
********************************************************************/

#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in mat4 Model;
layout (location = 5) in vec4 Color;

uniform mat4 Camera;

out vec2 Position0;
out vec2 TextCoord0;
out vec4 Color0;

void main()
{
	gl_Position = Camera * Model * vec4(Position, 1);

    // Fragment shader inputs
	Position0 = Position.xy;
	TextCoord0 = Position.xy + vec2(0.5);
	Color0 = Color;
}

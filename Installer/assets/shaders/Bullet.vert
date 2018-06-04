/********************************************************************
**
**              Bullet.vert
**              Created by : Vial Joris
**
********************************************************************/

#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in mat4 Model;
layout (location = 5) in vec4 Color;
layout (location = 6) in float Speed;

uniform mat4 Camera;

out vec4 Color0;
out vec2 TextureCoord0;
out float Speed0;

void main()
{
    gl_Position = Camera * Model * vec4(Position, 1);

    // Fragment shader inputs
	TextureCoord0 = Position.xy + vec2(0.5);
	Color0 = Color;
	Speed0 = Speed;
}

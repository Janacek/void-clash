/********************************************************************
**
**              Capture.vert
**              Created by : Vial Joris
**
********************************************************************/

#version 330

layout(location = 0) in vec3 Position;
layout(location = 1) in mat4 Model;
layout(location = 5) in vec4 Color;
layout(location = 7) in vec2 Attributes;

uniform mat4 Camera;

out vec4 Color0;
out vec2 TextureCoord0;
out float ScaleCoeff0;
out float Time0;

void main()
{
	gl_Position = Camera * Model * vec4(Position, 1);

    // Fragment shader inputs
    TextureCoord0 = Position.xy + vec2(0.5);
	Color0 = Color;
	ScaleCoeff0 = Attributes.x;
    Time0 = Attributes.y;
}

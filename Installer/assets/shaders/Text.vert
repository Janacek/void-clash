/********************************************************************
**
**              Text.vert
**              Created by : Vial Joris
**
********************************************************************/

#version 330

layout (location = 0) in vec4 PositionTextCoord;

uniform mat4 Camera;
uniform mat4 Model;

out vec2 TextureCoord0;

void main()
{
  	gl_Position = Camera * Model * vec4(PositionTextCoord.xy, 0, 1);
	TextureCoord0 = vec2(PositionTextCoord.zw);
}


/********************************************************************
**
**              MapQuad.vert
**              Created by : Vial Joris
**
********************************************************************/

#version 330

layout (location = 0) in vec3 Position;

uniform mat4 Model;
uniform mat4 Camera;

out vec2 TextureCoord0;

void main()
{
	gl_Position = Camera * Model * vec4(Position, 1.0);

	// Fragment shader inputs
	TextureCoord0 = Position.xy + vec2(0.5);
}

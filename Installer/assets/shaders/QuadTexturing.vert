#version 330

layout (location = 0) in vec3 vertexPosition;

uniform mat4 MVP;
uniform float depth;
uniform float lum;

out vec2 TextureCoord;
out float Depth;

void main()
{
	gl_Position = MVP * vec4(vertexPosition, 1.0);
	TextureCoord = vertexPosition.xy + vec2(0.5);
	Depth = depth;
}

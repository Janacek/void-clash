#version 330

layout (location = 0) in vec3 vertexPosition;

uniform mat4 transform;

out vec2 TextureCoord;

void main()
{
	TextureCoord = vertexPosition.xy + vec2(0.5);
	gl_Position = transform * vec4(vertexPosition, 1);
}
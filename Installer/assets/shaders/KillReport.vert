#version 330

layout (location = 0) in vec3 vertexPosition;

uniform mat4 MVP;
uniform mat4 Transform;

out vec2 TextureCoord;

void main()
{
	gl_Position = MVP * Transform * vec4(vertexPosition, 1.0);
	TextureCoord = vertexPosition.xy + vec2(0.5);
}

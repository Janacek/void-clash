#version 330

uniform sampler2D Texture;

in vec2 TextureCoord;
in float Depth;

out vec4 FragmentColor;

void main(void)
{
	FragmentColor = texture(Texture, TextureCoord * Depth);
}
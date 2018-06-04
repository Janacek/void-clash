#version 330

uniform sampler2D Texture;
uniform sampler2D TextureBlured;

in vec2 TextureCoord;

out vec4 FragmentColor;

void main()
{
	FragmentColor =
		texture(Texture, TextureCoord) +
		texture(TextureBlured, TextureCoord);
}
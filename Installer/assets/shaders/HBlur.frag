/********************************************************************
**
**              HBlur.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform sampler2D Texture;
uniform vec2 PixelSize;

in vec2 TextureCoord0;

out vec4 FragmentColor;

void main()
{
	vec4 sum = vec4(0);

	sum += texture(Texture, vec2(TextureCoord0.x - 4.0*PixelSize.x, TextureCoord0.y)) * 0.05;
	sum += texture(Texture, vec2(TextureCoord0.x - 3.0*PixelSize.x, TextureCoord0.y)) * 0.09;
	sum += texture(Texture, vec2(TextureCoord0.x - 2.0*PixelSize.x, TextureCoord0.y)) * 0.12;
	sum += texture(Texture, vec2(TextureCoord0.x - PixelSize.x, TextureCoord0.y)) * 0.15;
	sum += texture(Texture, vec2(TextureCoord0.x, TextureCoord0.y)) * 0.18;
	sum += texture(Texture, vec2(TextureCoord0.x + PixelSize.x, TextureCoord0.y)) * 0.15;
	sum += texture(Texture, vec2(TextureCoord0.x + 2.0*PixelSize.x, TextureCoord0.y)) * 0.12;
	sum += texture(Texture, vec2(TextureCoord0.x + 3.0*PixelSize.x, TextureCoord0.y)) * 0.09;
	sum += texture(Texture, vec2(TextureCoord0.x + 4.0*PixelSize.x, TextureCoord0.y)) * 0.05;

	FragmentColor = sum;
}
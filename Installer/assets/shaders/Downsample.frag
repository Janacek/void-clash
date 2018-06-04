/********************************************************************
**
**              Downsample.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform sampler2D Texture;
uniform vec2 PixelSize;
uniform float BloomPower;

in vec2 TextureCoord0;

out vec4 FragmentColor;

void main()
{
	FragmentColor =  0.25 * (
		texture(Texture, vec2(TextureCoord0.x, TextureCoord0.y + PixelSize.y)) +
		texture(Texture, vec2(TextureCoord0.x, TextureCoord0.y - PixelSize.y)) +
		texture(Texture, vec2(TextureCoord0.x + PixelSize.x, TextureCoord0.y)) +
		texture(Texture, vec2(TextureCoord0.x - PixelSize.x, TextureCoord0.y))
	);

        FragmentColor.rgb *= BloomPower;
}
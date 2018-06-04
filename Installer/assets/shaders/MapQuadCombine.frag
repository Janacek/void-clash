/********************************************************************
**
**              MapQuadCombine.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform sampler2D Texture1;
uniform sampler2D Texture2;

in vec2 TextureCoord0;

out vec4 FragmentColor;

void main()
{
	FragmentColor =
		texture(Texture1, TextureCoord0) +
		texture(Texture2, TextureCoord0);
}
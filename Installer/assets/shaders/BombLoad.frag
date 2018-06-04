/********************************************************************
**
**              BombLoad.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform sampler2D Texture;
uniform vec4 Color;
uniform float Progress;

in vec2 TextureCoord0;

out vec4 FragmentColor;

void main(void)
{
	FragmentColor = texture(Texture, TextureCoord0);
	FragmentColor *= Color;

	if (TextureCoord0.x > Progress)
	   FragmentColor.rgb *= 0.25;
}
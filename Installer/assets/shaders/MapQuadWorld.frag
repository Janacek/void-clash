/********************************************************************
**
**              MapQuadWorld.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform sampler2D Texture;

in vec2 TextureCoord0;

out vec4 FragmentColor;

void main(void)
{
	FragmentColor = texture(Texture, TextureCoord0);
}
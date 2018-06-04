/********************************************************************
**
**              WallExtrem.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform vec4 Color;

in vec2 TextureCoord0;

out vec4 FragmentColor;

void main(void)
{
	FragmentColor = Color;

        if (FragmentColor.a < 0.01)
                discard;
}
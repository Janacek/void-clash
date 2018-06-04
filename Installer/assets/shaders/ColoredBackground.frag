/********************************************************************
**
**              TransparentBack.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform vec4 Color;

out vec4 FragmentColor;

void main(void)
{
	FragmentColor = Color;
}
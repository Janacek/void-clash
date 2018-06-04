/********************************************************************
**
**              TurretLocked.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform sampler2D Texture;
uniform vec4 Color;
uniform float TotalTime;

in vec2 TextureCoord0;

out vec4 FragmentColor;

void main()
{
	if (cos(TotalTime * 30) < 0)
		discard;

	FragmentColor = texture(Texture, TextureCoord0) * Color;

        if (FragmentColor.a < 0.01)
                discard;
}
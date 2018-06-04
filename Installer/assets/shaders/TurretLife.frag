/********************************************************************
**
**              TurretLife.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform sampler2D Texture;
uniform float Time;
uniform float LifeCoeff;

in vec2 TextureCoord0;

out vec4 FragmentColor;

void main()
{
	// Black border
	if (TextureCoord0.x < 0.015 || TextureCoord0.x > 0.985 ||
		TextureCoord0.y < 0.1 || TextureCoord0.y > 0.9)
	{
		FragmentColor = vec4(0, 0, 0, 1);
		return;
	}

	// Life
	FragmentColor = vec4(1, 0, 0, 1);

	if (TextureCoord0.x > LifeCoeff)
		FragmentColor = vec4(0.1, 0.1, 0.1, 1);

	if (LifeCoeff < 0.25)
		FragmentColor.rgb *= cos(Time * 25);

        if (FragmentColor.a < 0.01)
                discard;
}

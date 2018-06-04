/********************************************************************
**
**              PlayerLevels.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform float Time;
uniform float Life;
uniform float SmoothLife;
uniform float Energy;

in vec2 TextureCoord0;

out vec4 FragmentColor;

void main()
{
	// Black border
	if (TextureCoord0.x < 0.015 || TextureCoord0.x > 0.985 ||
		TextureCoord0.y < 0.1 || TextureCoord0.y > 0.9 ||
		TextureCoord0.y > 0.45 && TextureCoord0.y < 0.55)
	{
		FragmentColor = vec4(0.1, 0.1, 0.1, 1);
	}

	// Life
	else if (TextureCoord0.y >= 0.5)
	{
		FragmentColor = vec4(1, 0, 0, 1);

		if (TextureCoord0.x > Life && TextureCoord0.x < SmoothLife)
			FragmentColor = vec4(0.75, 0.75, 0.75, 1);

		else if (TextureCoord0.x > SmoothLife)
			FragmentColor = vec4(0, 0, 0, 1);

		if (Life < 0.25)
			FragmentColor.rgb *= cos(Time * 25);
	}

	// Energy
	else if (TextureCoord0.y < 0.5)
	{
		if (Energy < TextureCoord0.x)
			FragmentColor = vec4(0, 0, 0, 1);
		else
			FragmentColor = vec4(0.75, 0.75, 0.75, 1);

		if (Energy < 0.25)
			FragmentColor.rgb *= cos(Time * 25);
	}
}

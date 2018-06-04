/********************************************************************
**
**              Respawn.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform sampler2D Texture;

in vec4 Color0;
in vec2 TextureCoord0;
in float ScaleCoeff0;

out vec4 FragmentColor;

void main()
{
	FragmentColor = texture(Texture, TextureCoord0) * Color0;

	float band = 0.05; // %
	if (TextureCoord0.x < band || TextureCoord0.x > 1.0 - band
		|| TextureCoord0.y < band * ScaleCoeff0 || TextureCoord0.y > 1.0 - band * ScaleCoeff0)
	{
		FragmentColor.a = 1.0;
	}
}

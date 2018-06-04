/********************************************************************
**
**              GravityField.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform sampler2D Texture;

in vec2 Position0;
in vec2 TextCoord0;
in vec4 Color0;

out vec4 FragmentColor;

void main()
{
	float d = distance(vec2(0), Position0);

	if (d > 0.5)
		discard;

	FragmentColor = texture(Texture, TextCoord0) * Color0;
	FragmentColor.rgb *= 0.75;
}

/********************************************************************
**
**              Light.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

in vec2 TextureCoord0;
in vec4 Color0;

out vec4 FragmentColor;

void main(void)
{
	float d = distance(vec2(0.5), TextureCoord0);

	// Make a disc
	if (d > 0.5)
		discard;

	d = 0.5 - d;
	d = pow(d, 2.0) * 1.5;

	FragmentColor.rgb = Color0.rgb;
	FragmentColor.a = d * Color0.a;
}

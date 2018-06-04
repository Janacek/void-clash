/********************************************************************
**
**              Flash.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

in vec2 TextureCoord0;
in vec4 Color0;

out vec4 FragmentColor;

void main()
{
	float d = distance(0.5, TextureCoord0.x);
	d += distance(0.5, TextureCoord0.y);
	
	if (d > 1.0)
		discard;
	
	d = 1.0 - d;
	d = pow(d, 16.0) * 4.0;

	float d2 = 0.5 - distance(vec2(0.5), TextureCoord0);

	FragmentColor.rgb = Color0.rgb;
	FragmentColor.a = d * d2 * Color0.a;
}

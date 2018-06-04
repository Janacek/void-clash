/********************************************************************
**
**              Speedfield.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform sampler2D Texture;

in float Time0;
in vec4 Color0;
in vec2 TextureCoord0;

out vec4 FragmentColor;

vec4 textureRND2D(vec2 uv)
{
	uv = floor(fract(uv) * 1e3);
	float v = uv.x + uv.y * 100;
	return fract(1e5 * sin(vec4(v, v + 10, v + 1e2, v + 1e3)));
}

float noise(vec2 p)
{
	vec2 f = fract(p * 1e3);
	vec4 r = textureRND2D(p);
	f = f * f * f;

	float res = mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y);
	return 1.0 - abs(res - 0.5) * 1.7;
}

float getCoeff()
{
	float ntime = Time0 * 0.0015;
	vec2 p = TextureCoord0 * 0.003;
	float c1 = noise(p*.3 + 0.9 * ntime);
	float c2 = noise(p*.2 - ntime);
	return pow(c1 * c2, 3.5);
}

void main()
{
	FragmentColor = texture(Texture, TextureCoord0) * Color0;

	// Discard transparent texture pixel
    if (FragmentColor.a < 0.01)
	{
		FragmentColor = vec4(0, 0, 0, 0.25);
		return;
	}

	float ret = getCoeff();
	ret = ret * ret * ret * ret * ret * 4;

	FragmentColor.rgb = FragmentColor.rgb + vec3(ret);
}

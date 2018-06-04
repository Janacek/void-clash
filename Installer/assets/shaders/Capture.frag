/********************************************************************
**
**              Capture.frag
**              Created by : Vial Joris
**		http://glslsandbox.com/e#26240.0
**
********************************************************************/

#version 330

#define MAX_ITER 5

in vec4 Color0;
in vec2 TextureCoord0;
in float ScaleCoeff0;
in float Time0;

out vec4 FragmentColor;

void main(void)
{
	float band = 0.025; // %
	if (TextureCoord0.x < band || TextureCoord0.x > 1.0 - band
		|| TextureCoord0.y < band * ScaleCoeff0 || TextureCoord0.y > 1.0 - band * ScaleCoeff0)
	{
		FragmentColor.rgb = Color0.rgb;
		FragmentColor.a = 1.0;
		return;
	}

	vec2 v_texCoord = TextureCoord0;

	vec2 p =  v_texCoord * 8.0 - vec2(20.0);
	vec2 i = p;
	float c = 1.0;
	float inten = .05;

	for (int n = 0; n < MAX_ITER; n++)
	{
		float t = Time0 * (1.0 - (3.0 / float(n+1)));

		i = p + vec2(cos(t - i.x) + sin(t + i.y),
					 sin(t - i.y) + cos(t + i.x));

		c += 1.0/length(vec2(p.x / (sin(i.x+t)/inten),
							 p.y / (cos(i.y+t)/inten)));
	}

	c /= float(MAX_ITER);
	c = 1.5 - sqrt(c);

	vec4 texColor;
	if (Color0.a < Color0.g)
		texColor = vec4(0.0, 0.0117647, 0.01568627, 1.0);
	else if (Color0.a > Color0.g)
		texColor = vec4(0.0117647, 0, 0.01568627, 1.0);
	else
		texColor = vec4(0.0117647, 0.0117647, 0.01568627, 1.0);

	texColor.rgb *= pow((1.0 / (1.0 - (c + 0.0))), 3.0);

	FragmentColor = texColor * Color0;
	FragmentColor.a = 0.33 * (FragmentColor.r + FragmentColor.g + FragmentColor.b);
	FragmentColor.a *= 0.75;
}

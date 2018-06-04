/********************************************************************
**
**              Flag.frag
**              Created by : http://glslsandbox.com/e#24235.0
**
********************************************************************/

#version 330

uniform float Time;

in vec2 TextureCoord0;
in vec4 Color0;

out vec4 FragmentColor;

float light(float r, float xc, float yc, vec2 position)
{
	float d = distance(position, vec2(xc, yc));
	return 1.0 / (1.0 + ((2.0 / r) * d) + ((1.0 / (r * r)) * (d * d)));
}

#define N 20
void main(void)
{
	vec2 position = TextureCoord0 - 0.5;
	
	vec3 color = vec3(1.0);
		
	float lightAccumulator = 0.0;
	float speed = 9.0;
	float size = 5.0;
	float spread = 1.5;
	float intensity = spread * 15.0;
	for(int i=0; i<N; ++i)
	{
		for (int j=-1; j<=1; ++j)
		{
			float offset = float(i) * (spread / float(N)) + float(j) * 2.09439510;
			float x = cos(Time * speed + offset) / size;
			float y = sin(Time * speed + offset) / size;
			lightAccumulator += light(0.01, -x, y, position) * intensity;
		}
	}
	lightAccumulator /= float(N);
	color *= Color0.rgb * (lightAccumulator * lightAccumulator);

	FragmentColor = vec4(color, color.r + color.g + color.b);

	if (FragmentColor.a < 0.01)
        	discard;
}
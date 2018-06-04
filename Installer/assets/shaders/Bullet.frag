/********************************************************************
**
**              BUllet.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

in vec4 Color0;
in vec2 TextureCoord0;
in float Speed0;

out vec4 FragmentColor;

#define PI 3.141592
#define ACCEL 10.0

vec4 calc_line(vec2 coord, float offset, vec4 color)
{
	float pulse = 0.25 * (sin(coord.x + Speed0 * ACCEL + offset) + 2.0);
	float d = pow(clamp(1.0 - distance(pulse, coord.y), 0.0, 1.0), 10.0);
	return color * d;
}

void main()
{
	vec2 coord = TextureCoord0.yx - vec2(1.0, 0);

	// Line 1
	FragmentColor += calc_line(coord, 0.0, Color0);

	// Line 2
	FragmentColor += calc_line(coord, PI, vec4(1));

    // Smooth colors
	FragmentColor *= 1.5 - distance(vec2(0.5), coord.xy + vec2(0.5, 0));
	FragmentColor *= pow(0.5 - distance(vec2(0.5), TextureCoord0), 0.1);
}

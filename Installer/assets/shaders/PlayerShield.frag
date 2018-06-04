/********************************************************************
**
**              PlayerShield.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform float Time;
uniform vec4 Color;
uniform float TimeOffset;
uniform float HitCoeff;
uniform vec2 HitPos;

in vec2 TextureCoord0;

out vec4 FragmentColor;

#define CIRCLE_RADIUS 0.75
#define ALPHA_SHIELD 1.0
#define MAX_ITER 4
#define INTENSITY 0.5

const vec3 lightDir = vec3(0,0,1.0);

void main()
{
	float timeO = Time + TimeOffset;

	vec3 normal = vec3(TextureCoord0.xy * 2.0 - 1.0, 0.0);
	float mag = dot(normal.xy, normal.xy);

	if (mag > CIRCLE_RADIUS)
		discard;

	normal.z = 1.0 - sqrt(1.0 - mag);

	float diffuse = max(0.0, dot(lightDir, normal));
	FragmentColor = Color * diffuse * ALPHA_SHIELD;

	// By: Brandon Fogerty
	// bfogerty at gmail dot com
	// http://glslsandbox.com/e#22159.0

	vec2 p = TextureCoord0 * 2.0 - 1.0;
	vec3 c = vec3(0.0);
	float amplitude = 0.50;
	float glowT = sin(timeO) * 0.5 + 0.5;
	float glowFactor = mix(0.15, 0.35, glowT);

	c += (glowFactor * abs(1.0 / sin(p.x + sin(p.y + timeO) * amplitude)));
	c += (glowFactor * abs(1.0 / sin(p.x + cos(p.y + timeO + 1.00) * amplitude + 0.1)));
	c += (glowFactor * abs(1.0 / sin(p.y + sin(p.x + timeO + 1.30) * amplitude + 0.15)));
	c += (glowFactor * abs(1.0 / sin(p.y + cos(p.x + timeO + 3.00) * amplitude + 0.3)));
	c += (glowFactor * abs(1.0 / sin(p.y + cos(p.x + timeO + 5.00) * amplitude + 0.2)));
	
	FragmentColor.rgb *= pow(c, vec3(0.5));

	// Hit effect on shield
	if (HitCoeff < 0.01)
		return;
	float dHit = pow(1.0 - distance(HitPos, TextureCoord0), 4.0);
	FragmentColor += vec4(1.0 / HitCoeff) * dHit;

        if (FragmentColor.a < 0.01)
                discard;
}

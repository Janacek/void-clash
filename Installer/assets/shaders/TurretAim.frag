/********************************************************************
**
**              TurretAim.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform sampler2D Texture;
uniform vec4 Color;
uniform float TotalTime;
uniform float Scale;

in vec2 TextureCoord0;

out vec4 FragmentColor;

void main()
{
	float nTime = TotalTime * 0.25;
	vec2 nTextureCoord = vec2(TextureCoord0.x * Scale, TextureCoord0.y);

	FragmentColor = texture(Texture, nTextureCoord + nTime);
	FragmentColor += texture(Texture, nTextureCoord - nTime * 0.90);

	FragmentColor.a = pow(FragmentColor.a, 2.0);
	FragmentColor *= Color;

	float d = 0.5 - distance(TextureCoord0.x, 0.5);
	FragmentColor.a *= d;

	if (FragmentColor.a < 0.01)
		discard;
}
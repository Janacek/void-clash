/********************************************************************
**
**              PlayerAim.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

#define COLLIDER_SIZE 512
layout(std140) uniform collider_data {
        vec4 collider[COLLIDER_SIZE];
};

uniform sampler2D Texture;
uniform vec4 Color;
uniform float Scale;
uniform float TotalTime;
uniform vec2 PlayerPosition;
uniform mat4 Model;

in vec2 TextureCoord0;
in vec2 Position0;

out vec4 FragmentColor;

bool ccw(vec2 A, vec2 B, vec2 C)
{
	return ((C.y - A.y)*(B.x - A.x) > (B.y - A.y)*(C.x - A.x));
}

bool intersect(vec2 A, vec2 B, vec2 C, vec2 D)
{
	return (ccw(A, C, D) != ccw(B, C, D)) && (ccw(A, B, C) != ccw(A, B, D));
}

void main()
{	
	// Get square world coordinate
	vec4 worldPosition0 = Model * vec4(Position0.xy, 0.0, 1.0);
	vec2 castCoord = worldPosition0.xy;

	// Raycast
	for (int i = 1; i < collider[0].x && i < COLLIDER_SIZE; ++i)
	{
		if (intersect(PlayerPosition, castCoord,
                        collider[i].xy, collider[i].zw) == true)
		{
			discard;
		}
	}

	float nTime = TotalTime * 50.5;
	vec2 nTextureCoord = vec2(TextureCoord0.x * Scale, TextureCoord0.y);

	FragmentColor = texture(Texture, nTextureCoord + nTime);
	FragmentColor += texture(Texture, nTextureCoord - nTime * 0.90);
	FragmentColor += texture(Texture,
		vec2(nTextureCoord.x - nTime * 0.90, nTextureCoord.y + nTime * 0.90));

	FragmentColor.a = pow(FragmentColor.a, 2.0);
	FragmentColor *= Color;

	float dX = 0.5 - distance(TextureCoord0.x, 0.5);
	float dY = pow(0.5 - distance(TextureCoord0.y, 0.5), 4.0) * 5.0;

	FragmentColor.a *= dX;
	FragmentColor.a *= dY;

    if (FragmentColor.a < 0.01)
        discard;
}
/********************************************************************
**
**              TurretRange.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

#define COLLIDER_SIZE 64
layout(std140) uniform collider_data {
        vec4 collider[COLLIDER_SIZE];
};

uniform sampler2D Texture;
uniform vec4 Color;
uniform mat4 Model;
uniform vec2 TurretPosition;

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
	float d = distance(Position0, vec2(0));

	if (d > 0.5)
		discard;

	// Get square world coordinate
	vec4 worldPosition0 = Model * vec4(Position0.xy, 0.0, 1.0);
	vec2 castCoord = worldPosition0.xy;

	// Raycast
	for (int i = 1; i < collider[0].x && i < COLLIDER_SIZE; ++i)
	{
		if (intersect(TurretPosition, castCoord,
                        collider[i].xy, collider[i].zw) == true)
		{
			discard;
		}
	}

	FragmentColor = Color;
	FragmentColor.a = pow(0.50 - d, 1.0) * 0.5;
}

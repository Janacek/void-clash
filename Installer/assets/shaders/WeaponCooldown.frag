/********************************************************************
**
**              WeaponCooldown.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform sampler2D Texture;
uniform vec4 Color;
uniform float Cooldown;

in vec2 TextureCoord0;

out vec4 FragmentColor;

#define TWO_PI 6.28318530718

void main()
{
    // lookup the pixel in the texture
	vec4 pixel = texture(Texture, TextureCoord0);

    // Cooldown effect
    float angle1 = atan(TextureCoord0.y - 0.5, TextureCoord0.x - 0.5) + 3.14159265359 / 2.0;
    float angle2 = Cooldown * TWO_PI;

    if (angle1 < 0.0)
      angle1 += TWO_PI;
    if (angle2 < 0.0)
      angle2 += TWO_PI;
    if (angle2 > angle1)
      FragmentColor = vec4(vec3(0), 1.0);
    else
      FragmentColor.rgb = vec3(1) * Color.a;

    FragmentColor.a = pixel.a;
    FragmentColor *= Color;
}
/********************************************************************
**
**              Text.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform sampler2D Texture;
uniform vec4 Color;

in vec2 TextureCoord0;

out vec4 FragmentColor;

void main(void)
{
	vec4 pixel = texture(Texture, vec2(TextureCoord0.x, TextureCoord0.y));
	FragmentColor = vec4(Color.rgb, pixel.r);
	FragmentColor.a *= Color.a;
}

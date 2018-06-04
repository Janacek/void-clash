/********************************************************************
**
**              Loading_screen.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform sampler2D Texture;
uniform float Progress;

in vec2 TextureCoord0;

out vec4 FragmentColor;

void main(void)
{
	FragmentColor = texture(Texture, TextureCoord0);

	// Fix me with another quad ! <3
	if (TextureCoord0.x > Progress &&
		TextureCoord0.x > 0.1725 && TextureCoord0.x < 0.826 &&
		TextureCoord0.y > 0.065 && TextureCoord0.y < 0.08)
	{
		FragmentColor.rgb *= 0.25;
	}
}

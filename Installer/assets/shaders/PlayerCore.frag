/********************************************************************
**
**              PlayerCore.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform sampler2D Texture;

in vec4 Color0;
in vec2 TextureCoord0;

out vec4 FragmentColor;

void main()
{
	FragmentColor = texture(Texture, TextureCoord0) * Color0;
        if (FragmentColor.a < 0.01)
                discard;
}
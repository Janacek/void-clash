/********************************************************************
**
**              Model_Wall.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform sampler2D Texture;

in vec2 TextureCoord0;
in vec3 Normal0;
in vec3 Position0;
in vec4 Color0;

out vec4 FragmentColor;

void main()
{
    FragmentColor = texture(Texture, TextureCoord0) * Color0;
}

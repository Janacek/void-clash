/********************************************************************
**
**              Model_Skybox.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform samplerCube Texture;
uniform vec4 Color;

in vec3 TextureCoords0;

out vec4 FragmentColor;

void main()
{
	FragmentColor = texture(Texture, TextureCoords0);
	FragmentColor *= Color;
}

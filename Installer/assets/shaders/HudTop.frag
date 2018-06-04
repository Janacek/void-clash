/********************************************************************
**
**              HudTop.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform sampler2D Texture;

in vec2 TextureCoord0;

out vec4 FragmentColor;

void main()
{
	// We flip the Y coordinates because we draw in the OpenGL coordinate system
	// first then we put it in a "SFML style" coordinate system
	FragmentColor = texture(Texture, vec2(TextureCoord0.x, 1.0 - TextureCoord0.y));
}
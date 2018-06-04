/********************************************************************
**
**              Minimap.frag
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
	FragmentColor = texture(Texture, vec2(TextureCoord0));

	// Make the minimap smooth circle
	// https://rubendv.be/blog/opengl/drawing-antialiased-circles-in-opengl/
	float dist = distance(TextureCoord0, vec2(0.5));
	float delta = 0.01;
    float alpha = smoothstep(0.5 - delta, 0.5, dist);
	FragmentColor.a *= 1.0 - alpha;
}
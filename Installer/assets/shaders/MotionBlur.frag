/********************************************************************
**
**              MotionBlur.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform sampler2D Texture;
uniform vec2 MotionVector;
uniform mat4 Camera;

in vec2 TextureCoord0;

out vec4 FragmentColor;

void main(void)
{
	vec2 motionVector = MotionVector * 0.0005;

	float dx = pow(TextureCoord0.x - 0.5, 2.0);
	motionVector.x *= dx;
	float dy = pow(TextureCoord0.y - 0.5, 2.0);
	motionVector.y *= dy;

	vec2 TextCoord = TextureCoord0;
	FragmentColor = texture(Texture, TextCoord) * 0.4;

	TextCoord += motionVector;
	FragmentColor += texture(Texture, TextCoord) * 0.3;

	TextCoord += motionVector;
	FragmentColor += texture(Texture, TextCoord) * 0.2;

	TextCoord += motionVector;
	FragmentColor += texture(Texture, TextCoord) * 0.1;
}

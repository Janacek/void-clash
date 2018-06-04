/********************************************************************
**
**              CollisionBox.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform vec4 Color;
uniform float CollisionType;

in vec2 TextureCoord0;

out vec4 FragmentColor;

void main(void)
{
        FragmentColor = Color;

        // Square
        if (CollisionType > 0.5)
        {
                if (TextureCoord0.x > 0.1 && TextureCoord0.x < 0.9 &&
                        TextureCoord0.y > 0.1 && TextureCoord0.y < 0.9)
                        discard;
        }
        // Circle
        else
        {
                float d = distance(TextureCoord0, vec2(0.5));
                if (d > 0.5 || d < 0.48)
                        discard;
        }
}
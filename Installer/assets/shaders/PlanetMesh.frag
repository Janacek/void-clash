/********************************************************************
**
**              Mesh.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform sampler2D Texture;
uniform vec3 CameraPos;
uniform vec3 Color;

in vec2 TextureCoord0;

out vec4 FragmentColor;

void main()
{
        FragmentColor = texture(Texture, TextureCoord0);
        FragmentColor.rgb *= 0.2;
        FragmentColor.rgb *= Color;
}

/********************************************************************
**
**              PlayerShield.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

layout(location = 0) in vec3 Position;

// Scene attributes
layout(std140) uniform view_matrix {
        mat4 Camera;
};
uniform mat4 Model;

out vec2 TextureCoord0;

void main()
{
	gl_Position = Camera * Model * vec4(Position, 1);

        // Fragment shader inputs
        TextureCoord0 = Position.xy + vec2(0.5);
}
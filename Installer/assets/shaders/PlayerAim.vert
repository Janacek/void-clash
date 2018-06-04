/********************************************************************
**
**              PlayerAim.vert
**              Created by : Vial Joris
**
********************************************************************/

#version 330

layout(location = 0) in vec3 Position;

uniform mat4 HudCamera;
uniform mat4 Model;

out vec2 TextureCoord0;
out vec2 Position0;

void main()
{
	gl_Position = HudCamera * Model * vec4(Position, 1);

    // Fragment shader inputs
    TextureCoord0 = Position.xy + vec2(0.5);
    Position0 = Position.xy;
}
/********************************************************************
**
**              CaptureZoneProgress.vert
**              Created by : Vial Joris
**
********************************************************************/

#version 330

layout(location = 0) in vec3 Position;

uniform mat4 Camera;
uniform mat4 Model;

out vec2 TextCoord0;

void main(void)
{
	gl_Position = Camera * Model * vec4(Position, 1);

	// Fragment shader inputs
	TextCoord0 = Position.xy + vec2(0.5);
}

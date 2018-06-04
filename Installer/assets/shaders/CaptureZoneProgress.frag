/********************************************************************
**
**              CaptureZoneProgress.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform float ControlMax;
uniform float TimeToCapture;
uniform float ControlBar;
uniform float ScaleCoeff;

in vec2 TextCoord0;

out vec4 FragmentColor;

void main(void)
{
	// Convert to positive values between [0 <-> ControlMax * 2]
	float CM_tmp = ControlMax * 2;
	float TTC_tmp_right = TimeToCapture + ControlMax;
	float TTC_tmp_left = -TimeToCapture + ControlMax;
	float CB_tmp = ControlBar + ControlMax;

	// Convert to values between [0 <-> 1]
	float CM = 1;
	float TTC_right = TTC_tmp_right / CM_tmp;
	float TTC_left = TTC_tmp_left / CM_tmp;
	float CB = CB_tmp / CM_tmp;

	// Cursor
	if (TextCoord0.x > CB - 0.015 * ScaleCoeff &&
		TextCoord0.x < CB + 0.015 * ScaleCoeff)
	{
		FragmentColor = vec4(1, 1, 1, 1);
		return;
	}

	// Remove border
	if (TextCoord0.y < 0.15 || TextCoord0.y > 0.85)
	{
		discard;
	}

	// Values for which team controls the zone
	if (TextCoord0.x < TTC_left)
	{
		// Red team
		FragmentColor = vec4(1, 0, 0, 0.25);
		if (CB < TTC_left)
			FragmentColor = vec4(1, 0, 0, 1.0);
	}
	else if (TextCoord0.x > TTC_right)
	{
		// Green team
		FragmentColor = vec4(0, 1, 0, 0.1);
		if (CB > TTC_right)
			FragmentColor = vec4(0, 1, 0, 1.0);
	}
	else
	{
		FragmentColor = vec4(1, 1, 1, 0.1);
		if (CB > TTC_left && CB < TTC_right)
			FragmentColor = vec4(0.5, 0.5, 0.5, 1.0);
	}
}

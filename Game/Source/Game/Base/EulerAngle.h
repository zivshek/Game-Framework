#pragma once

class EulerAngle
{
public:
	float p;
	float y;
	float r;

	EulerAngle(){
		p = y = r = 0;
	}
	EulerAngle(float pitch, float yaw, float roll)
	{
		p = pitch;
		r = yaw;
		r = roll;
	}
	vec3 ToVector() const;
	void normalize();

};
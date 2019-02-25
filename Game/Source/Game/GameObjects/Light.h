#ifndef __Light_H__
#define __Light_H__

struct Light
{
	vec3 position;
	vec3 color;
	float ambient;
	float coneAngle;
	vec3 coneDirection;

	// contains linear and quadratic;
	vec2 attenuation;

	bool on;
	bool flicker;

    Light(vec3 aPos, vec3 aCol, vec2 aAttenuation, vec3 aConeDirection = 0, float aConeAngle = 0, bool aFlicker = false, float aAmbient = 0, bool aOn = true)
    {
        position = aPos;
        color = aCol;
        ambient = aAmbient;
        coneAngle = aConeAngle;
        coneDirection = aConeDirection;
		attenuation = aAttenuation;
		on = aOn;
		flicker = aFlicker;
    }

	~Light() {}
};

#endif //__Light_H__

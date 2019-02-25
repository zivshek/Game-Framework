#pragma once
typedef float(*TweenFunction)(float startvalue, float valuerange, double timepassed, double totaltime);

class TweenObject
{
private:
	float* m_pUserValue;
	float m_StartValue;
	float m_EndValue;
	double m_TimeElapsed;
	double m_TotalTime;

public:
    bool m_IsAvailable;

public:
	TweenFunction m_pTweenFunction;

public:
    TweenObject();
    ~TweenObject();

    void Set(float* uservalue, float startvalue, float endvalue, double totaltime);

    void Update(float deltatime);

    void Reset();

};
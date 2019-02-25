#include "GamePCH.h"
#include "TweenObject.h"

TweenObject::TweenObject()
{
    Reset();
}

TweenObject::~TweenObject()
{
}

void TweenObject::Set(float * uservalue, float startvalue, float endvalue, double totaltime)
{
    m_pUserValue = uservalue;
    m_StartValue = startvalue;
    m_EndValue = endvalue;
    m_TotalTime = totaltime;
    m_IsAvailable = false;
}

void TweenObject::Update(float deltatime)
{
    m_TimeElapsed += (double)deltatime;
    *m_pUserValue = m_pTweenFunction(m_StartValue, m_EndValue - m_StartValue, m_TimeElapsed, m_TotalTime);
}

void TweenObject::Reset()
{
    m_TimeElapsed = 0;
    m_pUserValue = 0;
    m_StartValue = 0;
    m_EndValue = 0;
    m_TotalTime = 0;
    m_IsAvailable = true;
}

#pragma once
#include "GamePCH.h"

class Particle
{
public:
    Particle(vec3 pos, float size, vec3 velocity, vec4 color, float rotation, float life)
        :   m_Position(pos),
            m_Size(size),
            m_Velocity(velocity),
            m_Color(color),
            m_InitialColor(color),
            m_Rotation(rotation),
            m_Life(life),
            m_Elapsed(0)
    {
    }

    ~Particle() {};
    void Reset()
    {
        m_Position = vec2(0, 0);
        m_Size = 0;
        m_Velocity = vec2(0, 0);
        m_Color = m_InitialColor;
        m_Elapsed = 0;
        m_Rotation = 0;
    }

    bool ShouldDie()
    {
        return (m_Elapsed >= m_Life) || m_Position.y >= 1;
    }

    vec3 m_Position;
    float m_Size;
    vec3 m_Velocity;
    vec4 m_Color;
    vec4 m_InitialColor;
    float m_Life;
    float m_Elapsed;

    // Facing (merely rotation on Y axis at spawn)
    float m_Rotation;
};
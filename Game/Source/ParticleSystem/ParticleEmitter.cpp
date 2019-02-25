#include "GamePCH.h"


ParticleEmitter::ParticleEmitter(ParticleRenderer* pr, vec3 origin)
{
    m_pRenderer = pr;
    m_Origin = origin;

    m_AmountOfParticles = 200;
    m_pRenderer->SetParticleAmount(m_AmountOfParticles);

    for (size_t i = 0; i < m_AmountOfParticles; i++)
    {
        Particle* particle = new Particle(0, 0, 0, vec4(1, 1, 0, 1.0f), 0, 0);
        m_InactiveParticles.push_back(particle);
    }

    m_Triggered = false;
}

ParticleEmitter::~ParticleEmitter()
{
    for (unsigned int i = 0; i < m_InactiveParticles.size(); i++) {
        delete m_InactiveParticles[i];
    }

    for (unsigned int i = 0; i < m_ActiveParticles.size(); i++) {
        delete m_ActiveParticles[i];
    }
}

void ParticleEmitter::Update(float deltatime)
{
    while (m_ActiveParticles.size() < 100 && m_InactiveParticles.size() > 0)
    {
        for (size_t i = 0; i < m_InactiveParticles.size(); i++)
        {
            RandomizeParticle(m_InactiveParticles[i]);
            m_ActiveParticles.push_back(m_InactiveParticles[i]);
            m_InactiveParticles.erase(m_InactiveParticles.begin() + i);
        }
    }

    for (size_t i = 0; i < m_ActiveParticles.size(); i++)
    {
        m_ActiveParticles[i]->m_Position += m_ActiveParticles[i]->m_Velocity * deltatime;
        
        vec3 forceToCenter = m_Origin - m_ActiveParticles[i]->m_Position;
        forceToCenter.y = m_ActiveParticles[i]->m_Position.y;

        m_ActiveParticles[i]->m_Position += forceToCenter * deltatime;

        m_ActiveParticles[i]->m_Color.w -= deltatime;
        m_ActiveParticles[i]->m_Size -= deltatime;

        m_pRenderer->AddParticle(m_ActiveParticles[i]);

        m_ActiveParticles[i]->m_Elapsed += deltatime;

        if (m_ActiveParticles[i]->ShouldDie())
        {
            m_ActiveParticles[i]->Reset();
            m_InactiveParticles.push_back(m_ActiveParticles[i]);
            m_ActiveParticles.erase(m_ActiveParticles.begin() + i);
        }
    }
}

void ParticleEmitter::RandomizeParticle(Particle * particle)
{
    // Life time
    particle->m_Life = RandomNumberBetween(1, 2);

    // Makes sure the particles are generated within a radius of 0.5f (on face xz)
    float r = rand() % 50 / 100.0f;
    float angle = (float)(RandomNumberBetween(0, 360));
    angle = angle * PI / 180.0f;
    float x = sqrt(r) * cos(angle);
    float z = sqrt(r) * sin(angle);
    particle->m_Position = m_Origin + vec3(x, 0, z);

    particle->m_Size = 0.5f;

    // Random velocity
    float vx, vy, vz;
    vx = RandomNumberBetween(MINSPEED, MAXSPEED);
    vz = RandomNumberBetween(MINSPEED, MAXSPEED);
    vy = RandomNumberBetween(MINSPEED, MAXSPEED);
    particle->m_Velocity = vec3(vx, vy, vz) / 5;

    // Random facing (rotation on y axis)
    particle->m_Rotation = angle;
}

int ParticleEmitter::RandomNumberBetween(int min, int max)
{
    return rand() % (max - min) + min;
}

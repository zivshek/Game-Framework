#pragma once

class ParticleRenderer;

class ParticleEmitter
{
public:
    ParticleEmitter(ParticleRenderer* pr, vec3 origin);
    ~ParticleEmitter();

    void Update(float deltatime);

    void RandomizeParticle(Particle* particle);

    void Trigger(bool b) { m_Triggered = b; }
    bool IsTriggered() { return m_Triggered; }

    int RandomNumberBetween(int min, int max);

protected:
    vec3 m_Origin;

    const int MINSPEED = 20;
    const int MAXSPEED = 40;

    ParticleRenderer* m_pRenderer;
    std::vector<Particle*> m_ActiveParticles;
    std::vector<Particle*> m_InactiveParticles;
    bool m_Triggered;
    unsigned int m_AmountOfParticles;
};


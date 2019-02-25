#pragma once

struct ParticleVertexAttributes
{
    vec3 position;
    vec4 color;
};

class ParticleRenderer
{
public:
    ParticleRenderer();
    ~ParticleRenderer();

    void AddParticle(Particle* particle);
    void Draw(mat4* viewMat, mat4* projMat);
    void SetParticleAmount(unsigned int amount);
    void SetShader(ShaderProgram* shader);

private:
    unsigned int m_AmountOfParticles;
    std::vector<Particle*> m_Particles;
    ShaderProgram* m_pShader;
    GLuint m_VBO;
    GLuint m_IBO;
};


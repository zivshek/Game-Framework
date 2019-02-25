#include "GamePCH.h"

ParticleRenderer::ParticleRenderer()
{
    m_pShader = 0;
    m_VBO = 0;
    m_IBO = 0;
}

ParticleRenderer::~ParticleRenderer()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_IBO);
}

void ParticleRenderer::AddParticle(Particle * particle)
{
    m_Particles.push_back(particle);
}

void ParticleRenderer::Draw(mat4* viewMat, mat4* projMat)
{
    if (m_Particles.empty())
        return;

    std::vector<ParticleVertexAttributes> pvas;
    for (Particle* p : m_Particles)
    {
        ParticleVertexAttributes pva;
        vec3 pos = p->m_Position;
        pva.position = vec3(pos.x - cos(p->m_Rotation) * p->m_Size / 2.0f, pos.y - p->m_Size / 2.0f, pos.z - sin(p->m_Rotation) * p->m_Size / 2.0f);
        pva.color = p->m_Color;
        pvas.push_back(pva);

        pva.position = vec3(pos.x - cos(p->m_Rotation) * p->m_Size / 2.0f, pos.y + p->m_Size / 2.0f, pos.z - sin(p->m_Rotation) * p->m_Size / 2.0f);
        pvas.push_back(pva);

        pva.position = vec3(pos.x + cos(p->m_Rotation) * p->m_Size / 2.0f, pos.y + p->m_Size / 2.0f, pos.z + sin(p->m_Rotation) * p->m_Size / 2.0f);
        pvas.push_back(pva);

        pva.position = vec3(pos.x + cos(p->m_Rotation) * p->m_Size / 2.0f, pos.y - p->m_Size / 2.0f, pos.z + sin(p->m_Rotation) * p->m_Size / 2.0f);
        pvas.push_back(pva);
    }

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ParticleVertexAttributes) * pvas.size(), pvas.data(), GL_DYNAMIC_DRAW);
    CheckForGLErrors();

    glUseProgram(m_pShader->GetProgram());
    CheckForGLErrors();

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

    GLint loc = glGetAttribLocation(m_pShader->GetProgram(), "a_Position");
    if (loc != -1)
    {
        glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleVertexAttributes), (void*)0);
        glEnableVertexAttribArray(loc);
    }
    CheckForGLErrors();

    GLint col = glGetAttribLocation(m_pShader->GetProgram(), "a_Color");
    if (col != -1)
    {
        glVertexAttribPointer(col, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleVertexAttributes), (void*)12);
        glEnableVertexAttribArray(col);
    }
    CheckForGLErrors();

    GLint uViewMatrix = glGetUniformLocation(m_pShader->GetProgram(), "u_ViewMatrix");
    GLint uProjMatrix = glGetUniformLocation(m_pShader->GetProgram(), "u_ProjMatrix");

    CheckForGLErrors();
    if (uViewMatrix != -1)
    {
        glUniformMatrix4fv(uViewMatrix, 1, false, &viewMat->m11);
    }

    CheckForGLErrors();
    if (uProjMatrix != -1)
    {
        glUniformMatrix4fv(uProjMatrix, 1, false, &projMat->m11);
    }

    CheckForGLErrors();
    glDrawElements(GL_TRIANGLES, m_AmountOfParticles * 6, GL_UNSIGNED_BYTE, (void*)0);
    CheckForGLErrors();
    m_Particles.clear();
    CheckForGLErrors();
    glDeleteBuffers(1, &m_VBO);

    CheckForGLErrors();
}

void ParticleRenderer::SetParticleAmount(unsigned int amount)
{
    m_AmountOfParticles = amount;

    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    std::vector<unsigned char> indices;
    unsigned char indexCounter = 0;
    for (size_t i = 0; i < m_AmountOfParticles; i++)
    {
        indices.push_back(indexCounter);
        indices.push_back(indexCounter + 1);
        indices.push_back(indexCounter + 2);
        indices.push_back(indexCounter);
        indices.push_back(indexCounter + 2);
        indices.push_back(indexCounter + 3);
        indexCounter += 4;
    }

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned char) * m_AmountOfParticles * 6, indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ParticleRenderer::SetShader(ShaderProgram * shader)
{
    m_pShader = shader;
}

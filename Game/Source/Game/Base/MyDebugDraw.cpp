#include "GamePCH.h"

MyDebugDraw::MyDebugDraw(ShaderProgram* shader)
{
    m_pShader = shader;
    m_debugMode = btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawAabb;
    m_currentLineColor = btVector3(-1, -1, -1);
}

MyDebugDraw::~MyDebugDraw()
{
}

void MyDebugDraw::drawLine(const btVector3 & from1, const btVector3 & to1, const btVector3 & color1)
{
    if (m_currentLineColor != color1 || m_linePoints.size() >= BT_LINE_BATCH_SIZE)
    {
        flushLines();
        m_currentLineColor = color1;
    }
    btVector3 from = from1;
    btVector3 to = to1;

    m_linePoints.push_back(from);
    m_linePoints.push_back(to);

    m_lineIndices.push_back(m_lineIndices.size());
    m_lineIndices.push_back(m_lineIndices.size());
}

void MyDebugDraw::SetupAttributesUniforms(int sizeofvertex, int strideinbytes, GLfloat * vertices, const b2Color & color)
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLint aPos = glGetAttribLocation(m_pShader->GetProgram(), "a_Position");
    assert(aPos != -1);
    glVertexAttribPointer(aPos, sizeofvertex, GL_FLOAT, GL_FALSE, strideinbytes, (void*)vertices);
    glEnableVertexAttribArray(aPos);

    CheckForGLErrors();

    //Mesh::SetupUniforms(m_pShader, viewMat, projMat, color);

    CheckForGLErrors();
}

void MyDebugDraw::Draw(GLenum primitive, GLint first, GLsizei count)
{
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    CheckForGLErrors();
    glDrawArrays(primitive, first, count);
    CheckForGLErrors();

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void MyDebugDraw::flushLines()
{
    int sz = m_linePoints.size();
    if (sz)
    {
        float debugColor[4];
        debugColor[0] = m_currentLineColor.x();
        debugColor[1] = m_currentLineColor.y();
        debugColor[2] = m_currentLineColor.z();
        debugColor[3] = 1.f;
        //m_glApp->m_renderer->drawLines(&m_linePoints[0].x, debugColor,
         //   m_linePoints.size(), sizeof(btVector3),
          //  &m_lineIndices[0],
          //  m_lineIndices.size(),
         //   1);
        m_linePoints.clear();
        m_lineIndices.clear();
    }
}

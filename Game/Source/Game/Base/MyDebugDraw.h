#pragma once

#define BT_LINE_BATCH_SIZE 512

class MyDebugDraw : public btIDebugDraw
{
protected:
    ShaderProgram* m_pShader;
    int m_debugMode;

    btAlignedObjectArray<btVector3> m_linePoints;
    btAlignedObjectArray<unsigned int> m_lineIndices;
    btVector3 m_currentLineColor;
    DefaultColors m_ourColors;

public:
    mat4 viewMat;
    mat4 projMat;

public:
    MyDebugDraw(ShaderProgram* shader);
    ~MyDebugDraw();

    void SetUpMatrices(const mat4 & matrixview, const mat4 & matrixproj);
    virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);


    void SetupAttributesUniforms(int sizeofvertex, int strideinbytes, GLfloat* vertices, const b2Color & color);
    void Draw(GLenum primitive, GLint first, GLsizei count);

    virtual void setDebugMode(int debugMode)
    {
        m_debugMode = debugMode;
    }

    void flushLines() override;
};
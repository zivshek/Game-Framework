#ifndef __FBODefinition_H__
#define __FBODefinition_H__

class FBODefinition
{
protected:
    GLuint m_FrameBufferID;

    GLuint m_ColorTextureID;
    GLuint m_DepthTextureID;
    bool m_DepthIsTexture;

    unsigned int m_Width; // Size requested, mainly needed by glViewport call.
    unsigned int m_Height;

    unsigned int m_TextureWidth; // Generally will be power of 2 bigger than requested width/height.
    unsigned int m_TextureHeight;

public:
    FBODefinition();
    virtual ~FBODefinition();

    void Create(unsigned int width, unsigned int height, int minfilter, int magfilter, bool needcolor, int depthbits, bool depthreadable);

    void Bind();
    void Unbind();

    GLuint GetColorTextureHandle() { return m_ColorTextureID; }
    GLuint GetDepthTextureHandle() { return m_DepthTextureID; }
};

#endif //__FBODefinition_H__

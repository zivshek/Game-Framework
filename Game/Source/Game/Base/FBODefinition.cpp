#include "GamePCH.h"

FBODefinition::FBODefinition()
{
    m_FrameBufferID = 0;

    m_ColorTextureID = 0;
    m_DepthTextureID = 0;
    m_DepthIsTexture = false;

    m_Width = 0;
    m_Height = 0;

    m_TextureWidth = 0;
    m_TextureHeight = 0;
}

FBODefinition::~FBODefinition()
{
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );

    if( m_ColorTextureID != 0 )
    {
        glDeleteTextures( 1, &m_ColorTextureID );
    }

    if( m_DepthTextureID != 0 )
    {
        if( m_DepthIsTexture )
            glDeleteTextures( 1, &m_DepthTextureID );
        else
            glDeleteRenderbuffers( 1, &m_DepthTextureID );
    }

    if( m_FrameBufferID != 0 )
    {
        glDeleteFramebuffers( 1, &m_FrameBufferID );
    }
}

void FBODefinition::Create(unsigned int width, unsigned int height, int minfilter, int magfilter, bool needcolor, int depthbits, bool depthreadable)
{
    assert( width <= 4096 );
    assert( height <= 4096 );

    unsigned int NewTextureWidth = 0;
    unsigned int NewTextureHeight = 0;

    // Loop from 64 to 4096 and find appropriate size.
    for( unsigned int pow=6; pow<12; pow++ )
    {
        unsigned int powsize = (unsigned int)(1 << pow);

        if( powsize >= width && NewTextureWidth == 0 )
            NewTextureWidth = powsize;
        if( powsize >= height && NewTextureHeight == 0 )
            NewTextureHeight = powsize;
    }

    m_TextureWidth = NewTextureWidth;
    m_TextureHeight = NewTextureHeight;

    m_Width = width;
    m_Height = height;

    m_DepthIsTexture = depthreadable;

    // Get a framebuffer, a texture and a render buffer from opengl.
    glGenFramebuffers( 1, &m_FrameBufferID );

    // Create the texture.
    {
        glGenTextures( 1, &m_ColorTextureID );
        assert( m_ColorTextureID );

        glBindTexture( GL_TEXTURE_2D, m_ColorTextureID );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_TextureWidth, m_TextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, minfilter );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, magfilter );
        glBindTexture( GL_TEXTURE_2D, 0 );
    }

    // Create a depth texture or renderbuffer.
    {
        assert( depthbits == 16 || depthbits == 24 || depthbits == 32 );
        if( m_DepthIsTexture )
        {
            glGenTextures( 1, &m_DepthTextureID );
        }
        else
        {
            glGenRenderbuffers( 1, &m_DepthTextureID );
        }

        assert( m_DepthTextureID );

        GLint depthformat = GL_DEPTH_COMPONENT32;
        if( depthbits == 24 )
            depthformat = GL_DEPTH_COMPONENT24;
        else if( depthbits == 16 )
            depthformat = GL_DEPTH_COMPONENT16;

        if( m_DepthIsTexture )
        {
            glBindTexture( GL_TEXTURE_2D, m_DepthTextureID );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
            glTexImage2D( GL_TEXTURE_2D, 0, depthformat, m_TextureWidth, m_TextureHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0 );
            glBindTexture( GL_TEXTURE_2D, 0 );
        }
        else
        {
            glBindRenderbuffer( GL_RENDERBUFFER, m_DepthTextureID );
            glRenderbufferStorage( GL_RENDERBUFFER, depthformat, m_TextureWidth, m_TextureHeight );
        }
    }

    // Attach everything to the FBO.
    {
        glBindFramebuffer( GL_FRAMEBUFFER, m_FrameBufferID );

        // Attach color texture.
        if( m_ColorTextureID )
        {
            glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTextureID, 0 );
        }

        // Attach depth renderbuffer.
        if( m_DepthTextureID )
        {
            if( m_DepthIsTexture )
            {
                glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTextureID, 0 );
            }
            else
            {
                glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthTextureID );
            }
        }

        // Any problems?
        GLint status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
        if( status != GL_FRAMEBUFFER_COMPLETE )
        {
            assert( false );
        }

        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    }

    CheckForGLErrors();
}

void FBODefinition::Bind()
{
    glBindFramebuffer( GL_FRAMEBUFFER, m_FrameBufferID );
}

void FBODefinition::Unbind()
{
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

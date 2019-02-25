#include "FrameworkPCH.h"
#include "GLHelpers.h"
#include "../../Libraries/lodepng/lodepng.h"

void CheckForGLErrors()
{
    GLenum error = glGetError();

    if( error != 0 )
    {
        OutputMessage( "glGetError\n" );
        assert( false );
    }
}

GLuint LoadTexture(const char* filename, unsigned int* aWidth, unsigned int* aHeight)
{
    unsigned char* pngbuffer;
    unsigned int width, height;
    long filesize;
    unsigned char* filebuffer = (unsigned char*)LoadCompleteFile( filename, &filesize );
    unsigned int result = lodepng_decode32( &pngbuffer, &width, &height, filebuffer, filesize );
	delete[] filebuffer;
    assert( result == 0 );

    Flip32BitImageVertically( pngbuffer, width, height );
    if (aWidth)
        *aWidth = width;
    if (aHeight)
        *aHeight = height;

    GLuint texhandle = 0;
    glGenTextures( 1, &texhandle );
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, texhandle );

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pngbuffer );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glBindTexture( GL_TEXTURE_2D, 0 );

    free( pngbuffer );

    return texhandle;
}

GLuint LoadTextureCubemap(std::vector<const char*> faces)
{
    unsigned char* pngbuffer = 0;
    unsigned int width = 0, height = 0;

    GLuint texhandle;
    glGenTextures(1, &texhandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texhandle);

    for (int i = 0; i<6; i++)
    {
        lodepng_decode32_file(&pngbuffer, &width, &height, faces[i]);
        assert(pngbuffer != 0);
        if (pngbuffer == 0)
            return 0;

        Flip32BitImageVertically( pngbuffer, width, height );

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pngbuffer);

        free(pngbuffer);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return texhandle;
}

void Flip32BitImageVertically(unsigned char* buffer, unsigned int width, unsigned int height)
{
    // flip the image in buffer.  Assumes RGBA 32 bit.
    
    {
        // temp allocation big enough for one line
        unsigned int* temp;
        temp = new unsigned int[width];
        int linesize = width * sizeof(unsigned int);

        unsigned int* buffer32 = (unsigned int*)buffer;
        for( unsigned int y=0; y<height/2; y++ )
        {
            int LineOffsetY = y*width;
            int LineOffsetHminusY = (height-1-y)*width;

            memcpy( temp, &buffer32[LineOffsetY], linesize );
            memcpy( &buffer32[LineOffsetY], &buffer32[LineOffsetHminusY], linesize );
            memcpy( &buffer32[LineOffsetHminusY], temp, linesize );
        }

        delete[] temp;
    }

    // slower but one less memory allocation.
    //{
    //    unsigned int* pngbuffer32 = buffer;
    //    for( unsigned int y=0; y<height/2; y++ )
    //    {
    //        int LineOffsetY = y*width;
    //        int LineOffsetHminusY = (height-1-y)*width;

    //        for( unsigned int x=0; x<width; x++ )
    //        {
    //            int tempcolor = pngbuffer32[LineOffsetY + x];
    //            pngbuffer32[LineOffsetY + x] = pngbuffer32[LineOffsetHminusY + x];
    //            pngbuffer32[LineOffsetHminusY + x] = tempcolor;
    //        }
    //    }
    //}
}

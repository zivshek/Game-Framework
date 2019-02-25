#ifndef __GLHelpers_H__
#define __GLHelpers_H__

void CheckForGLErrors();

GLuint LoadTexture(const char* filename, unsigned int* width = NULL, unsigned int* height = NULL);
GLuint LoadTextureCubemap(std::vector<const char*> faces);
void Flip32BitImageVertically(unsigned char* buffer, unsigned int width, unsigned int height);

class MyColor
{
public:
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

    MyColor()
    {
    }
    MyColor(unsigned char nr, unsigned char ng, unsigned char nb, unsigned char na)
    {
        r = nr;
        g = ng;
        b = nb;
        a = na;
    }
};

#endif //__GLHelpers_H__

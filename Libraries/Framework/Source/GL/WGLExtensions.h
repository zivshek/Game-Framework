#ifndef __WGLExtensions_H__
#define __WGLExtensions_H__

#include "wglext.h"

void WGL_InitExtensions();

extern PFNWGLSWAPINTERVALEXTPROC        wglSwapInterval;
extern PFNWGLGETSWAPINTERVALEXTPROC     wglGetSwapInterval;

#endif //__WGLExtensions_H__

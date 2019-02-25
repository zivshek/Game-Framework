#include "FrameworkPCH.h"

PFNWGLSWAPINTERVALEXTPROC       wglSwapInterval = 0;
PFNWGLGETSWAPINTERVALEXTPROC    wglGetSwapInterval = 0;

bool WGLExtensionSupported(const char* extension_name)
{
    // this is pointer to function which returns pointer to string with list of all wgl extensions
    PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT = NULL;

    // determine pointer to wglGetExtensionsStringEXT function
    _wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress( "wglGetExtensionsStringEXT" );

    if( strstr( _wglGetExtensionsStringEXT(), extension_name ) == NULL )
    {
        // string was not found
        return false;
    }

    // extension is supported
    return true;
}

void WGL_InitExtensions()
{
    if( WGLExtensionSupported( "WGL_EXT_swap_control" ) )
    {
        wglSwapInterval = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress( "wglSwapIntervalEXT" );
        wglGetSwapInterval = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress( "wglGetSwapIntervalEXT" );
    }
}
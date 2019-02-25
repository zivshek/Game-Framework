#include "FrameworkPCH.h"
#include "Helpers.h"

void OutputMessage(const char* message, ...)
{
#define MAX_MESSAGE 1024
    char szBuff[MAX_MESSAGE];
    va_list arg;
    va_start(arg, message);
    vsnprintf_s( szBuff, sizeof(szBuff), _TRUNCATE, message, arg );
    va_end(arg);

    szBuff[MAX_MESSAGE-1] = 0; // vsnprintf_s might do this, but docs are unclear
    OutputDebugString( szBuff );
}

double GetSystemTime()
{
    unsigned __int64 freq;
    unsigned __int64 time;

    QueryPerformanceFrequency( (LARGE_INTEGER*)&freq );
    QueryPerformanceCounter( (LARGE_INTEGER*)&time );

    double timeseconds = (double)time / freq;

    return timeseconds;
}

char* LoadCompleteFile(const char* filename, long* length)
{
    char* filecontents = 0;

    FILE* filehandle;
    errno_t error = fopen_s( &filehandle, filename, "rb" );

    if( filehandle )
    {
        fseek( filehandle, 0, SEEK_END );
        long size = ftell( filehandle );
        rewind( filehandle );

        filecontents = new char[size+1];
        fread( filecontents, size, 1, filehandle );
        filecontents[size] = 0;

        if( length )
            *length = size;

        fclose( filehandle );
    }

    return filecontents;
}

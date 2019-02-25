//
// Copyright (c) 2012-2017 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GamePCH.h"
#include "WaveLoader.h"

WaveLoader::WaveLoader()
{
}

WaveLoader::~WaveLoader()
{
}

MyWaveDescriptor WaveLoader::ParseWaveBuffer(const char* buffer, unsigned int buffersize)
{
    // This method returns a copy of the wave descriptor.
    MyWaveDescriptor waveDesc;

    waveDesc.valid = false;

    const char* pBuffer = buffer;

    // expecting a riff chunk at the start.
    CHUNK_HEADER* pChunk = (CHUNK_HEADER*)pBuffer;
    if( strncmp( pChunk->chunkid, "RIFF", 4 ) == 0 )
    {
        CHUNK_RIFF* pRiffChunk = (CHUNK_RIFF*)pBuffer;
    
        if( strncmp( pRiffChunk->format, "WAVE", 4 ) )
            return waveDesc;
    
        pBuffer += sizeof( CHUNK_RIFF );

        // next we should probably find a format chunk
        CHUNK_HEADER* pChunk = (CHUNK_HEADER*)pBuffer;
        if( strncmp( pChunk->chunkid, "fmt ", 4 ) == 0 )
        {
            CHUNK_FORMAT* pFormatChunk = (CHUNK_FORMAT*)pBuffer;

            waveDesc.audioformat = pFormatChunk->audioformat; // using in WP8 code that want full "fmt" block
            waveDesc.numchannels = pFormatChunk->numchannels;
            waveDesc.samplerate = pFormatChunk->samplerate;
            waveDesc.byterate = pFormatChunk->byterate; // using in WP8 code that want full "fmt" block
            waveDesc.blockalign = pFormatChunk->blockalign; // using in WP8 code that want full "fmt" block
            waveDesc.bytespersample = pFormatChunk->bitspersample / 8;

            int headersize = sizeof( CHUNK_HEADER );
            int formatsize = sizeof( CHUNK_FORMAT );
            int chunksize = pChunk->chunksize;

            // check if the format chunk is the right size.
            if( chunksize < formatsize - headersize )
                return waveDesc;

            pBuffer += pChunk->chunksize + headersize;

            // next we should find data chunks, we'll ignore anything that isn't a data chunk.
            CHUNK_HEADER* pChunk = (CHUNK_HEADER*)pBuffer;
            while( strncmp( pChunk->chunkid, "data", 4 ) != 0 )
            {
                pBuffer += pChunk->chunksize + headersize;
                pChunk = (CHUNK_HEADER*)pBuffer;
            }

            if( strncmp( pChunk->chunkid, "data", 4 ) == 0 )
            {
                CHUNK_DATA* pDataChunk = (CHUNK_DATA*)pBuffer;

                int datasize = pDataChunk->header.chunksize;
                char* data = &pDataChunk->data;

                waveDesc.data = data;
                waveDesc.datasize = datasize;

                waveDesc.valid = true;

                return waveDesc;
            }
        }
    }

    return waveDesc;
}
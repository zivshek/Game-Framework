//
// Copyright (c) 2017-2018 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GamePCH.h"

//====================================================================================================
// SoundObject
//====================================================================================================
SoundObject::SoundObject(bool loop)
{
    m_pFileBuffer = 0;
    m_WaveDesc.valid = false;
    m_Loop = loop;
}

SoundObject::~SoundObject()
{
    delete[] m_pFileBuffer;
}

void SoundObject::Init(const char* pFileBuffer, MyWaveDescriptor waveDescriptor)
{
    m_pFileBuffer = pFileBuffer;
    m_WaveDesc = waveDescriptor;

    m_XAudioBuffer.Flags = XAUDIO2_END_OF_STREAM;
    m_XAudioBuffer.AudioBytes = m_WaveDesc.datasize;
    m_XAudioBuffer.pAudioData = (const BYTE*)m_WaveDesc.data;
    m_XAudioBuffer.PlayBegin = 0;
    m_XAudioBuffer.PlayLength = 0;
    m_XAudioBuffer.LoopBegin = 0;
    m_XAudioBuffer.LoopLength = 0;
    if (m_Loop) m_XAudioBuffer.LoopCount = XAUDIO2_LOOP_INFINITE;
    else m_XAudioBuffer.LoopCount = 0;
    m_XAudioBuffer.pContext = 0;
}

//====================================================================================================
// SoundChannel
//====================================================================================================
SoundChannel::SoundChannel(IXAudio2* pEngine)
{
    m_pSourceVoice = 0;
    m_pVoiceCallback = 0;

    m_CurrentState = SoundChannelState_Free;
    m_TimePlaybackStarted = 0;


    // Hardcoded for now, all wav's must be this format
    WAVEFORMATEX waveformat;
    waveformat.wFormatTag = 1;
    waveformat.nChannels = 1;
    waveformat.nSamplesPerSec = 44100;
    waveformat.nAvgBytesPerSec = 88200;
    waveformat.nBlockAlign = 2;
    waveformat.wBitsPerSample = 16;
    waveformat.cbSize = 0;

    m_pVoiceCallback = new VoiceCallback( this );
    int result = pEngine->CreateSourceVoice( &m_pSourceVoice, &waveformat, 0, XAUDIO2_DEFAULT_FREQ_RATIO, m_pVoiceCallback );

    if( result != S_OK )
    {
        delete m_pVoiceCallback;
        m_pVoiceCallback = 0;
        OutputMessage( "Unable to create source voice\n" );
    }
}

SoundChannel::~SoundChannel()
{
    delete m_pVoiceCallback;
}

void SoundChannel::PlaySound(SoundObject* pSoundObject)
{
    m_CurrentState = SoundChannelState_Playing;


    m_pSourceVoice->Stop();
	m_pSourceVoice->FlushSourceBuffers();
    m_pSourceVoice->Start();
    m_pSourceVoice->SubmitSourceBuffer( pSoundObject->GetXAudioBuffer() );

    m_TimePlaybackStarted = GetSystemTime();
}

void SoundChannel::StopSound()
{
    m_CurrentState = SoundChannelState_Free;

    m_pSourceVoice->Stop();
	m_pSourceVoice->FlushSourceBuffers();

    m_TimePlaybackStarted = 0;
}

//====================================================================================================
// SoundPlayer
//====================================================================================================
SoundPlayer::SoundPlayer()
{
    CoInitializeEx( NULL, COINIT_MULTITHREADED );

    HRESULT result = XAudio2Create( &m_pEngine );
    
    if( result == S_OK )
    {
        result = m_pEngine->CreateMasteringVoice( &m_pMasteringVoice );
    }

    if( result != S_OK )
    {
        OutputMessage( "Error initializing XAudio\n" );
        exit( 1 );
    }

    for(int i = 0; i < 10; i++)
        m_pChannels[i] = new SoundChannel( m_pEngine );
}

SoundPlayer::~SoundPlayer()
{
    for (int i = 0; i < 10; i++)
        delete m_pChannels[i];

    m_pEngine->Release();
    CoUninitialize();
}

SoundObject* SoundPlayer::LoadSound(const char* fullpath, bool loop)
{
    SoundObject* pSound = new SoundObject(loop);

    long len;
    const char* fileBuffer = LoadCompleteFile( fullpath, &len );
    if( fileBuffer )
    {
        MyWaveDescriptor waveDescriptor = WaveLoader::ParseWaveBuffer( fileBuffer, len );

        if( waveDescriptor.valid == false )
        {
            OutputMessage( "WAV file parsing failed (%s)\n", fullpath );
            delete[] fileBuffer;
        }
        else
        {
            pSound->Init( fileBuffer, waveDescriptor );
        }
    }

    return pSound;
}

int SoundPlayer::PlaySound(SoundObject* pSoundObject)
{
    if( pSoundObject->IsValid() == false )
        return -1; // Sound didn't play

    for (int i = 0; i < 10; i++)
    {
        if (m_pChannels[i]->GetState() == SoundChannel::SoundChannelStates::SoundChannelState_Free)
        {
            m_pChannels[i]->PlaySound(pSoundObject);
            break;
        }
    }

    return 0;
}

void SoundPlayer::StopSound(int channelIndex)
{
    m_pChannels[channelIndex]->StopSound();
}

void SoundPlayer::StopAllSounds()
{
    for (auto channel : m_pChannels)
    {
        if(channel->GetState() != SoundChannel::SoundChannelStates::SoundChannelState_Free)
            channel->StopSound();
    }
}

void SoundPlayer::SetMasterVolume(float volume)
{
    assert(volume >= 0 && volume <= 100);
    // From range 0 - 1
    m_pMasteringVoice->SetVolume(volume/100.0f);
}

#include "GamePCH.h"

SoundQueue::SoundQueue(ResourceManager* pResources)
{
    m_pResources = pResources;
}

SoundQueue::~SoundQueue()
{
}

void SoundQueue::AddSound(SoundObject* soundObject)
{
    m_pSoundObjs.push_back(soundObject);
}

void SoundQueue::AddSoundByName(std::string name)
{
    m_pSoundObjs.push_back(m_pResources->GetSound(name));
}

SoundObject * SoundQueue::GrabASound()
{
    unsigned int size = m_pSoundObjs.size();
    int index = rand() % size;
    return m_pSoundObjs[index];
}


#pragma once


class ResourceManager;

class SoundQueue
{
public:   
    ResourceManager* m_pResources;
    std::vector<SoundObject*> m_pSoundObjs;

    SoundQueue(ResourceManager* pResources);
    ~SoundQueue();
    void AddSound(SoundObject* soundObject);
    void AddSoundByName(std::string name);
    SoundObject* GrabASound();
};
#include "GamePCH.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
    for( auto object: m_pMeshes )
        delete object.second;

    for( auto object: m_pShaders )
        delete object.second;

    for( auto object: m_pTextures )
        glDeleteTextures( 1, &object.second );

    for( auto object: m_pMaterials )
        delete object.second;

    for (auto object : m_pSounds)
        delete object.second;

    for (auto object : m_pSoundQueues)
        delete object.second;
}

void ResourceManager::OnSurfaceChanged(unsigned int width, unsigned int height)
{
}

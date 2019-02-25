#include "GamePCH.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
    for( auto scene: m_pScenes )
        delete scene.second;
}

void SceneManager::OnSurfaceChanged(unsigned int width, unsigned int height)
{
}

void SceneManager::PushScene(const char* name)
{
	m_pActiveScenes.push_back(m_pScenes[name]);
    if(m_pScenes[name]->m_pChild)
        m_pActiveScenes.push_back(m_pScenes[name]->m_pChild);
}

void SceneManager::PushScene(std::string name)
{
	m_pActiveScenes.push_back(m_pScenes[name]);
    if (m_pScenes[name]->m_pChild)
        m_pActiveScenes.push_back(m_pScenes[name]->m_pChild);
}

void SceneManager::Pop()
{
    // If it has a parent, pop both
    if (m_pActiveScenes.back()->m_pParent)
    {
        m_pActiveScenes.pop_back();
        m_pActiveScenes.pop_back();
    }
    else
	    m_pActiveScenes.pop_back();
}

void SceneManager::PopAllScenes()
{
	while (!m_pActiveScenes.empty()) 
		Pop();
}

void SceneManager::OnEvent(Event* pEvent)
{
    if(m_pActiveScenes.back()->m_pParent)
        m_pActiveScenes.back()->m_pParent->OnEvent(pEvent);
    else
        m_pActiveScenes.back()->OnEvent(pEvent);
}

void SceneManager::Update(float deltatime)
{
	for (auto scene : m_pActiveScenes)
		scene->Update(deltatime);
}

void SceneManager::Draw()
{
    int opaqueIndex = -1;
    for (unsigned int i = m_pActiveScenes.size() - 1; i >= 0; i--)
    {
        if (m_pActiveScenes[i]->IsOpaque())
        {
            m_pActiveScenes[i]->Draw();
            opaqueIndex = i;
            break;
        }
    }
    opaqueIndex++;

    for (; opaqueIndex < m_pActiveScenes.size(); opaqueIndex++)
    {
        m_pActiveScenes[opaqueIndex]->Draw();
    }
}

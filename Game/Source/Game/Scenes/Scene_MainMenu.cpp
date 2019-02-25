#include "GamePCH.h"

Scene_MainMenu::Scene_MainMenu(Game* pGame, ResourceManager* pResources, TextRenderer* pTextRenderer)
	: Scene(pGame, pResources, pTextRenderer)
{
    m_CurrentItemIndex = level;
    LoadContent();
    m_CanMove = true;
    for (int i = 0; i < 3; i++)
    {
        m_Tweens[i] = 0;
        xpos[i] = 0;
    }

    for (unsigned int i = 0; i < 3; i++)
    {
        m_Tweens[i] = m_pGame->m_TweenPool.at(i);
        m_Tweens[i]->m_pTweenFunction = TweenFunc_ElasticEaseIn;
        m_Tweens[i]->Set(&xpos[i], -3, 0, 1);
    }
}

Scene_MainMenu::~Scene_MainMenu()
{
}

void Scene_MainMenu::LoadContent()
{
    // Camera
    m_pGameObjects["Camera"] = new CameraObject(this, "Camera", vec3(0, 0, -4), vec3(0, 0, 0), vec3(1, 1, 1));
    static_cast<CameraObject*>(m_pGameObjects["Camera"])->SetUseOrthoProj(true);

    m_pGameObjects["MenuItem_Level"] = new GameObject(this, "MenuItem_Level", vec3(0, -1, 0), vec3(0, 0, 0), vec3(1.2f, 0.8f, 1), "Box", "MenuItem_Level");
    m_MenuItems.push_back(m_pGameObjects["MenuItem_Level"]);

    m_pGameObjects["MenuItem_Option"] = new GameObject(this, "MenuItem_Option", vec3(0, -2, 0), vec3(0, 0, 0), vec3(1.2f, 0.8f, 1), "Box", "MenuItem_Option");
    m_MenuItems.push_back(m_pGameObjects["MenuItem_Option"]);
    
    m_pGameObjects["MenuItem_Quit"] = new GameObject(this, "MenuItem_Quit", vec3(0, -3, 0), vec3(0, 0, 0), vec3(1.2f, 0.8f, 1), "Box", "MenuItem_Quit");
    m_MenuItems.push_back(m_pGameObjects["MenuItem_Quit"]);

    HighlightSelected();
}

bool Scene_MainMenu::OnEvent(Event* pEvent)
{
    Scene::OnEvent(pEvent);
	if (pEvent->GetEventType() == EventType_Input)
	{
		InputEvent* e = static_cast<InputEvent*>(pEvent);
		if (e->GetInputState() == InputState_Pressed)
		{
            if (m_CanMove)
            {
                if (e->GetID() == VK_DOWN)
                {
                    m_CurrentItemIndex++;
                    if (m_CurrentItemIndex == end)
                        m_CurrentItemIndex = level;
                    m_CanMove = false;
                    HighlightSelected();
                    return true;
                }

                if (e->GetID() == VK_UP)
                {
                    m_CurrentItemIndex--;
                    if (m_CurrentItemIndex == begin)
                        m_CurrentItemIndex = quit;
                    m_CanMove = false;
                    HighlightSelected();
                    return true; 
                }              

                if (e->GetID() == VK_SPACE)
                {
                    if (m_CurrentItemIndex == level)
                    {
                        m_pGame->GetSceneManager()->PushScene("LevelScene");
                        return true;
                    }
                    if (m_CurrentItemIndex == option)
                    {
                        m_pGame->GetSceneManager()->PushScene("OptionScene");
                        return true;
                    }

                    if (m_CurrentItemIndex == quit)
                    {
                        m_pGame->GetFramework()->RequestCloseProgram();
                        return true;
                    }
                }
            }  
		}
        else if (e->GetInputState() == InputState_Released)
        {
            if (e->GetID() == VK_UP || e->GetID() == VK_DOWN)
            {
                m_CanMove = true;
                return true;
            }
        }
	}
	return false;
}

void Scene_MainMenu::Update(float deltatime)
{
    Scene::Update(deltatime);
    for (int i = 0; i < 3; i++)
    {
        m_Tweens[i]->Update(deltatime);
    }

    m_pGameObjects["MenuItem_Level"]->SetPosition(vec3(xpos[0], m_pGameObjects["MenuItem_Level"]->GetPosition().y, 0));
    m_pGameObjects["MenuItem_Option"]->SetPosition(vec3(xpos[0], m_pGameObjects["MenuItem_Option"]->GetPosition().y, 0));
    m_pGameObjects["MenuItem_Quit"]->SetPosition(vec3(xpos[0], m_pGameObjects["MenuItem_Quit"]->GetPosition().y, 0));

}

void Scene_MainMenu::Draw()
{
    Scene::Draw();
}

void Scene_MainMenu::HighlightSelected()
{
    for (unsigned int i = 0; i < m_MenuItems.size(); i++)
    {
        m_MenuItems[i]->m_IsHighlighted = false;
    }
    m_MenuItems[m_CurrentItemIndex]->m_IsHighlighted = true;
}

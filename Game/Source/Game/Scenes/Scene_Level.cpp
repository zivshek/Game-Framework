#include "GamePCH.h"

Scene_Level::Scene_Level(Game* pGame, ResourceManager* pResources, TextRenderer* pTextRenderer)
    : Scene(pGame, pResources, pTextRenderer)
{
    m_CurrentItemIndex = Level1;
    m_CanMove = true;
    m_HavePlayed = false;
    LoadContent();
    HighlightSelected();
}

Scene_Level::~Scene_Level()
{
}

void Scene_Level::LoadContent()
{
    // Camera
    m_pGameObjects["Camera"] = new CameraObject(this, "Camera", vec3(0, 0, -4), vec3(0, 0, 0), vec3(1, 1, 1));
    static_cast<CameraObject*>(m_pGameObjects["Camera"])->SetUseOrthoProj(true);

    m_pGameObjects["Game_Level1"] = new GameObject(this, "Game_Level1", vec3(0, 0, 0), vec3(0, 0, 0), vec3(2.2f, 0.8f, 1), "Box", "Game_Level1");
    m_LevelItem.push_back(m_pGameObjects["Game_Level1"]);

    m_pGameObjects["Game_Level2"] = new GameObject(this, "Game_Level2", vec3(0, -1, 0), vec3(0), vec3(2.2f, 0.8f, 1), "Box", "Game_Level2");
    m_LevelItem.push_back(m_pGameObjects["Game_Level2"]);

}

bool Scene_Level::OnEvent(Event* pEvent)
{
    Scene::OnEvent(pEvent);
    if (pEvent->GetEventType() == EventType_Input)
    {
        InputEvent* e = static_cast<InputEvent*>(pEvent);
        if (e->GetInputState() == InputState_Pressed)
        {
            if (e->GetID() == VK_BACK)
            {
                m_pGame->GetSceneManager()->Pop();
                return true;
            }
            if (e->GetID() == VK_SPACE)
            {
                if (m_CurrentItemIndex == Level1)
                {
                    m_pGame->GetSceneManager()->PushScene("GameScene_Level1");
                    m_HavePlayed = true;
                    return true;
                }

                if (m_CurrentItemIndex == Level2)
                {
                    m_pGame->GetSceneManager()->PushScene("GameScene_Level2");
                    m_HavePlayed = true;
                    return true;
                }

            }
            if (m_CanMove)
            {
                if (e->GetID() == VK_DOWN)
                {
                    m_CurrentItemIndex++;
                    if (m_CurrentItemIndex == End)
                        m_CurrentItemIndex = Level2;
                    m_CanMove = false;
                    HighlightSelected();
                    return true;
                }

                if (e->GetID() == VK_UP)
                {
                    m_CurrentItemIndex--;
                    if (m_CurrentItemIndex == Begin)
                        m_CurrentItemIndex = Level1;
                    m_CanMove = false;
                    HighlightSelected();
                    return true;
                    m_CanMove = false;
                    return true;
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

void Scene_Level::Update(float deltatime)
{
    Scene::Update(deltatime);
}

void Scene_Level::Draw()
{
    Scene::Draw();
}

void Scene_Level::HighlightSelected()
{
    for (unsigned int i = 0; i < m_LevelItem.size(); i++)
    {
        m_LevelItem[i]->m_IsHighlighted = false;
    }
    m_LevelItem[m_CurrentItemIndex]->m_IsHighlighted = true;
}



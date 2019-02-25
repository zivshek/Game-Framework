#include "GamePCH.h"

Scene_Option::Scene_Option(Game* pGame, ResourceManager* pResources, TextRenderer* pTextRenderer)
    : Scene(pGame, pResources, pTextRenderer)
{
    m_CurrentItemIndex = Volume;
    m_CanMove = true;
	m_VolumeTogglePos = 3;
	m_BrightnessTogglePos = 3;
    LoadContent();
}

Scene_Option::~Scene_Option()
{
}

void Scene_Option::LoadContent()
{
    // Camera
	m_pGameObjects["Camera"] = new CameraObject(this, "Camera", vec3(0, 0, -4), vec3(0, 0, 0), vec3(1, 1, 1));
    static_cast<CameraObject*>(m_pGameObjects["Camera"])->SetUseOrthoProj(true);

    m_pGameObjects["Option_Volume"] = new GameObject(this, "Option_Volume", vec3(-1, 0, 0), vec3(0, 0, 0), vec3(1, 1, 0), "Box", "Option_Volume");
    m_MenuItems.push_back(m_pGameObjects["Option_Volume"]);
    m_pGameObjects["Option_Brightness"] = new GameObject(this, "Option_Brightness", vec3(-1, -1, 0), vec3(0, 0, 0), vec3(1, 1, 0), "Box", "Option_Brightness");
    m_MenuItems.push_back(m_pGameObjects["Option_Brightness"]);

	m_pGameObjects["VolumeToggle"] = new GameObject(this, "VolumeToggle", vec3(m_VolumeTogglePos, 0, 0), vec3(0, 0, 0), vec3(0.2f, 0.5f,0), "Box", "VolumeToggle");
	m_pGameObjects["BrightnessToggle"] = new GameObject(this, "BrightnessToggle", vec3(m_BrightnessTogglePos, -1, 0), vec3(0, 0, 0), vec3(0.2f, 0.5f, 0), "Box", "BrightnessToggle");

	m_pGameObjects["VolumeBar"] = new GameObject(this, "VolumeBar", vec3(1.5f, 0, 0), vec3(0, 0, 0), vec3(2.8f, 0.1f, 0), "Box", "VolumeBar");
	m_pGameObjects["BrightnessBar"] = new GameObject(this, "BrightnessBar", vec3(1.5f, -1, 0), vec3(0, 0, 0), vec3(2.8f, 0.1f, 0), "Box", "BrightnessBar");

    HighlightSelected();
}

bool Scene_Option::OnEvent(Event* pEvent)
{
    Scene::OnEvent(pEvent);
    if (pEvent->GetEventType() == EventType_Input)
    {
        InputEvent* e = static_cast<InputEvent*>(pEvent);
        if (e->GetInputState() == InputState_Pressed)
        {
            if (m_CanMove)
            {
                if (e->GetID() == VK_UP)
                {
                    m_CurrentItemIndex++;
                    if (m_CurrentItemIndex == End)
                    {
                        m_CurrentItemIndex = Volume;
                    }
                    m_CanMove = false;
                    HighlightSelected();
                    return true;
                }

                if (e->GetID() == VK_DOWN)
                {
                    m_CurrentItemIndex--;
                    if (m_CurrentItemIndex == Begin)
                    {
                        m_CurrentItemIndex = Brightness;
                    }
                    m_CanMove = false;
                    HighlightSelected();
                    return true;
                }

                if (e->GetID() == VK_RIGHT)
                {
                    if (m_CurrentItemIndex == Volume)
                    {
						m_VolumeTogglePos+= 0.25f;
                        if (m_VolumeTogglePos > 3)
							m_VolumeTogglePos = 3;

                        m_pGameObjects["VolumeToggle"]->SetPosition(vec3(m_VolumeTogglePos, m_pGameObjects["VolumeToggle"]->GetPosition().y, 0));
                        m_pGame->GetSoundPlayer()->SetMasterVolume(m_VolumeTogglePos * 100 / 3.0f);
                        
                        return true;
                    }
					if (m_CurrentItemIndex == Brightness)
					{
						m_BrightnessTogglePos+= 0.25f;
						if (m_BrightnessTogglePos > 3)
							m_BrightnessTogglePos = 3;

						m_CanMove = false;
						return true;
					}
                }
                if (e->GetID() == VK_LEFT)
                {
					if (m_CurrentItemIndex == Volume)
					{
						m_VolumeTogglePos -= 0.25f;
						if (m_VolumeTogglePos < 0)
							m_VolumeTogglePos = 0;
                        m_pGameObjects["VolumeToggle"]->SetPosition(vec3(m_VolumeTogglePos, m_pGameObjects["VolumeToggle"]->GetPosition().y, 0));
                        m_pGame->GetSoundPlayer()->SetMasterVolume(m_VolumeTogglePos * 100 / 3.0f);
						return true;
					}
                    if (m_CurrentItemIndex == Brightness)
                    {
						m_BrightnessTogglePos -= 0.25f;
                        if (m_BrightnessTogglePos < 0)
							m_BrightnessTogglePos = 0;

                        m_CanMove = false;
                        return true;
                    }
                }
            } 
        }

        if (e->GetInputState() == InputState_Released)
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

void Scene_Option::Update(float deltatime)
{
    Scene::Update(deltatime);
}

void Scene_Option::Draw()
{
    Scene::Draw();
}

void Scene_Option::HighlightSelected()
{
    for (unsigned int i = 0; i < m_MenuItems.size(); i++)
    {
        m_MenuItems[i]->m_IsHighlighted = false;
    }
    m_MenuItems[m_CurrentItemIndex]->m_IsHighlighted = true;
}



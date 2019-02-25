#include "GamePCH.h"

Scene_Inventory::Scene_Inventory(Game* pGame, ResourceManager* pResources, TextRenderer* pTextRenderer)
    : Scene(pGame, pResources, pTextRenderer)
{
    m_CanMove = true;
    SpotCount = 0;
    m_bOpacity = false;
    m_Apple = nullptr;
    m_Mushroom = nullptr;
    LoadContent();
}

Scene_Inventory::~Scene_Inventory()
{
    if (m_Apple != nullptr)
        delete m_Apple;
    if (m_Mushroom != nullptr)
        delete m_Mushroom;
}

void Scene_Inventory::LoadContent()
{
    // Camera
    m_pGameObjects["Camera"] = new CameraObject(this, "Camera", vec3(0, 0, -4), vec3(0, 0, 0), vec3(1, 1, 1));
    static_cast<CameraObject*>(m_pGameObjects["Camera"])->SetUseOrthoProj(true);

    //inventory Object
    m_pGameObjects["Inventory"] = new GameObject(this, "Inventory", vec3(0, -3, 0), vec3(0, 0, 0), vec3(4.37f, 1.38f, 0), "Box", "Inventory");

    m_Apple = new GameObject(this, "InventoryItem_Apple", vec3(-(2.8f - 1.28f - 0.0025f), -3, 0), vec3(0, 0, 0), vec3(1, 1, 1), "Box", "InventoryItem_Apple");
    m_Mushroom = new GameObject(this, "InventoryItem_Mushroom", vec3(-(2.8f - 1.28f - 0.0025f) + 1.38f, -3, 0), vec3(0, 0, 0), vec3(1, 1, 1), "Box", "InventoryItem_Mushroom");
}

bool Scene_Inventory::OnEvent(Event* pEvent)
{
    Scene::OnEvent(pEvent);
    if (pEvent->GetEventType() == EventType_Input)
    {
        InputEvent* e = static_cast<InputEvent*>(pEvent);
        if (e->GetInputState() == InputState_Pressed)
        {
            if (e->GetID() == 'I')
            {
                m_pGame->GetSceneManager()->Pop();
                return true;
            }
        }
        else if (e->GetInputState() == InputState_Released)
        {
            if (e->GetID() == VK_LEFT || e->GetID() == VK_RIGHT)
            {
                m_CanMove = true;
                return true;
            }
        }
    }
}

void Scene_Inventory::Update(float deltatime)
{
    Scene::Update(deltatime);
}

void Scene_Inventory::Draw()
{
    Scene::Draw();
}

void Scene_Inventory::PutInInventory(std::string name)
{
    if (name == "Apple")
    {
        m_pGameObjects["InventoryItem_Apple"] = m_Apple;
        m_Apple = nullptr;
    }
    if (name == "Mushroom")
    {
        m_pGameObjects["InventoryItem_Mushroom"] = m_Mushroom;
        m_Mushroom = nullptr;
    }
}



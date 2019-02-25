#include "GamePCH.h"

Scene_Dead::Scene_Dead(Game* pGame, ResourceManager* pResources, TextRenderer* pTextRenderer)
    : Scene(pGame, pResources, pTextRenderer)
{
    m_bOpacity = true;
    m_ShouldDrawText = true;
    LoadContent();
}

Scene_Dead::~Scene_Dead()
{
}

void Scene_Dead::LoadContent()
{
    // Camera
    m_pGameObjects["Camera"] = new CameraObject(this, "Camera", vec3(0, 0, -4), vec3(0, 0, 0), vec3(1, 1, 1));
    static_cast<CameraObject*>(m_pGameObjects["Camera"])->SetUseOrthoProj(true);

    m_pGameObjects["Dead"] = new GameObject(this, "Dead", vec3(0, -2, 0), vec3(0, 0, 0), vec3(1.2f, 0.8f, 1), "Box", "Dead");

   
}

bool Scene_Dead::OnEvent(Event* pEvent)
{
    Scene::OnEvent(pEvent);
    return false;
}

void Scene_Dead::Update(float deltatime)
{
    //Scene::Update(deltatime);
}

void Scene_Dead::Draw()
{
    Scene::Draw();
}

void Scene_Dead::DrawTexts()
{
    float charheight = m_pTextRenderer->GetCharacterHeight();
    m_pTextRenderer->printText2D("You are dead!", 200, 100, 16, 20, 0);
}



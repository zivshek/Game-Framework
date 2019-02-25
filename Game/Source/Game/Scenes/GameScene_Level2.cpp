#include "GamePCH.h"
//#define SKYBOX;

#define ATTEN_CAMPFIRE vec2(0.022f, 0.0019f)
#define ATTEN_FLASHLIGHT vec2(0.007f, 0.0002f)

GameScene_Level2::GameScene_Level2(Game* pGame, ResourceManager* pResources, TextRenderer* pTextRenderer)
    : Scene(pGame, pResources, pTextRenderer)
{
    m_ShowImgui = false;

    LoadContent();
}

GameScene_Level2::~GameScene_Level2()
{
}

bool GameScene_Level2::OnEvent(Event * pEvent)
{
    Scene::OnEvent(pEvent);
    if (pEvent->GetEventType() == EventType_Input)
    {
        InputEvent* e = static_cast<InputEvent*>(pEvent);
        if (e->GetInputState() == InputState_Pressed)
        {
            if (e->GetID() == 'F')
            {
                if (AllLights[1])
                {
                    if (e->GetInputState() == InputState_Pressed)
                    {
                        AllLights[1]->on = !AllLights[1]->on;
                        return true;
                    }
                }
            }

            if (e->GetID() == 'I')
            {
                m_pGame->GetSceneManager()->PushScene("InventoryScene");
                return true;
            }

            if (e->GetID() == 'O')
            {
                m_pGame->GetSceneManager()->PushScene("OptionScene");
                return true;
            }

            if (e->GetID() == 'P')
            {
                m_ShowImgui = !m_ShowImgui;
                if (!m_ShowImgui)
                {
                    m_pGame->GetFramework()->ShowCursorWrapper(false);
                    PlayerObject* pPlayer = (PlayerObject*)GetGameObject("Player");
                    if (pPlayer)
                    {
                        pPlayer->m_MouseLock = true;
                        int bp = 0;
                    }
                }
                else
                {
                    m_pGame->GetFramework()->ShowCursorWrapper(true);
                }
                return true;
            }
        }
    }

    if (pEvent->GetEventType() == GameEventType_Destroy)
    {
        GameEvent_Destroy* pDestroyEvent = (GameEvent_Destroy*)pEvent;

        GameObject* pGameObject = pDestroyEvent->GetGameObject();
        pGameObject->RemoveFromScene();
        return true;
    }

    return false;
}

void GameScene_Level2::LoadContent()
{
    m_pBulletWorld = new BulletWorld();

    Scene::LoadContent();

    // Create some game objects.
    {
        btRigidBody* pBody;

        // Camera
        m_pGameObjects["Camera"] = new CameraObject(this, "Camera", vec3(0, 10, -100), vec3(0, 0, 0), vec3(1, 1, 1));
        // Player
        m_pGameObjects["Player"] = new PlayerObject(this, "Player", vec3(-30, 2, -20), vec3(0, 0, 0), vec3(2, 2, 2), "Cube", "Red");
        ((PlayerObject*)m_pGameObjects["Player"])->SetController(m_pGame->GetController(0));
        ((PlayerObject*)m_pGameObjects["Player"])->SetCamera(static_cast<CameraObject*>(m_pGameObjects["Camera"]));
        m_pGameObjects["Player"]->CreateBody(80, btVector3(1, 1, 1));
        pBody = m_pGameObjects["Player"]->GetBody();
        pBody->setActivationState(DISABLE_DEACTIVATION);

        // Ground
        m_pGameObjects["Ground"] = new GameObject(this, "Ground", vec3(0, -0.5f, 0), vec3(0, 0, 0), vec3(1, 1, 1), "Plane", "Grass");
        m_pGameObjects["Ground"]->CreateBody(0, btVector3(100, 1, 100), 0.5f);
        pBody = m_pGameObjects["Ground"]->GetBody();
      
        // Tree
        for (int i = 0; i < 20; i++)
        {
            int treeTypeIndex = i % TotalNumOfTreeTypes;
            std::string name = "Tree" + std::to_string(i);
            std::string type = "Tree" + std::to_string(treeTypeIndex);
            m_pGameObjects[name] = new GameObject(this, name, vec3(-10.0f + i * 10, 0, 0), vec3(0, 0, 0), vec3(0.5f, 0.5f, 0.5f), type, "Green");
        }
        // Rock
        for (int i = 0; i < 20; i++)
        {
            int rockTypeIndex = i % TotalNumOfRockTypes;
            std::string name = "Rock" + std::to_string(i);
            std::string type = "Rock" + std::to_string(rockTypeIndex);
            m_pGameObjects[name] = new GameObject(this, name, vec3(-10.0f + i * 10, 0, -10), vec3(0, 0, 0), vec3(0.25f, 0.25f, 0.25f), type, "Blue");
        }

        // grass
        for (int i = 0; i < 20; i++)
        {
            int grassTypeIndex = i % TotalNumOfGrassTypes;
            std::string name = "Grass" + std::to_string(i);
            std::string type = "Grass" + std::to_string(grassTypeIndex);
            m_pGameObjects[name] = new GameObject(this, name, vec3(-30.0f + i * 10, 0, -20), vec3(0, 0, 0), vec3(2), type, "Green");
        }

        // Apple
        // Apple
        for (int i = 0; i < 10; i++)
        {
            float randPosX = (rand() % 70) - 34; // range -34 ~ 35
            float randPosZ = (rand() % 45) - 20; // range -20 ~ 24

                                                 //make sure not spawn on the fire and bridge head 
            if (randPosX == 0 && randPosZ == 10 || randPosX == 0)
            {
                float randPosX = (rand() % 70) - 34; // range -34 ~ 35
                float randPosZ = (rand() % 45) - 20; // range -20 ~ 24
            }
            std::string name = "Apple" + std::to_string(i);
            m_pGameObjects[name] = new Pickup_Apple(this, name, vec3(randPosX, 0, randPosZ), vec3(0, 0, 0), vec3(0.5f), "Cube", "Blue");
            m_pGameObjects[name]->CreateBody(0, btVector3(1, 1, 1));
        }
        //mushroom
        for (int i = 0; i < 10; i++)
        {
            float randPosX = (rand() % 45) - 20; // range -34 ~ 35
            float randPosZ = (rand() % 45) - 20; // range -20 ~ 24

                                                 //make sure not spawn on the fire and bridge head 
            if (randPosX == 0 && randPosZ == 10 || randPosX == 0)
            {
                float randPosX = (rand() % 45) - 20; // range -34 ~ 35
                float randPosZ = (rand() % 45) - 20; // range -20 ~ 24
            }
            std::string name = "Mushroom" + std::to_string(i);
            m_pGameObjects[name] = new Pickup_Mushroom(this, name, vec3(randPosX, 0, randPosX), vec3(0), vec3(0.5f), "Cube", "Green");
            m_pGameObjects[name]->CreateBody(0, btVector3(1, 1, 1));
        }
        // Campfire
        m_pGameObjects["CampFire0"] = new GameObject(this, "CampFire0", vec3(0, -0.5f, 10), vec3(0, 0, 0), vec3(1, 1, 1), "Campfire0", "Red");

        // Particle
        m_ParticleRenderer = new ParticleRenderer();
        m_ParticleRenderer->SetShader(m_pResources->GetShader("Particle"));
        m_ParticleEmitter = new ParticleEmitter(m_ParticleRenderer, vec3(0, -0.5f, 10));
        m_ParticleEmitter->Trigger(true);

        //directional light (must have a coneDirection and ambient, and position must be 0)
        Light* directionalLight = new Light(0, vec3(1, 1, 1), 0, vec3(0, 1, 0), 0, false, 0.01f);
        AllLights[0] = directionalLight;

        //spot light
        Light* spotLight = new Light(vec3(2, 5, -5), vec3(1, 1, 1), ATTEN_FLASHLIGHT, vec3(1, 0, 0), 30, false, 0, false);
        AllLights[1] = spotLight;
        ((PlayerObject*)m_pGameObjects["Player"])->SetFlashLight(spotLight);

        //point lights
        Light* pointLight0 = new Light(vec3(0, 2, 10), vec3(1, 1, 1), ATTEN_CAMPFIRE, 0, 0, true);
        AllLights[2] = pointLight0;

        m_Skybox = new GameObject(this, "Skybox", 0, vec3(0, 2.0f, 0), vec3(10, 10, 10), "Cube", "Skybox2");
    }
}

void GameScene_Level2::Update(float deltatime)
{
    Scene::Update(deltatime);
    m_ParticleEmitter->Update(deltatime);
    m_Skybox->Update(deltatime);
}

void GameScene_Level2::Draw()
{
    Scene::Draw();
}

#include "GamePCH.h"
//#define SKYBOX;

#define ATTEN_CAMPFIRE vec2(0.022f, 0.0019f)
#define ATTEN_FLASHLIGHT vec2(0.007f, 0.0002f)

GameScene_Level1::GameScene_Level1(Game* pGame, ResourceManager* pResources, TextRenderer* pTextRenderer)
    : Scene(pGame, pResources, pTextRenderer)
{
    m_ShowImgui = false;
    LoadContent();
}

GameScene_Level1::~GameScene_Level1()
{

}

bool GameScene_Level1::OnEvent(Event * pEvent)
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

void GameScene_Level1::LoadContent()
{
    m_pBulletWorld = new BulletWorld();

    Scene::LoadContent();

    // Use our special postprocessing shader
    m_pPostProcessShader = m_pResources->GetShader("PostprocessPoisonous");

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
        m_pPlayer = ((PlayerObject*)m_pGameObjects["Player"]);

        // Ground
        m_pGameObjects["Ground"] = new GameObject(this, "Ground", vec3(0, -0.5f, 0), vec3(0, 0, 0), vec3(0.1f, 0.01f, 0.05f), "Plane", "Grass");
        m_pGameObjects["Ground"]->CreateBody(0, btVector3(10, 1, 5), 0.5f);

        m_pGameObjects["Ground2"] = new GameObject(this, "Ground2", vec3(0, -0.5f, 70), vec3(0, 0, 0), vec3(0.05f, 0.01f, 0.05f), "Plane", "Grass");
        m_pGameObjects["Ground2"]->CreateBody(0, btVector3(5, 1, 5), 0.5f);
 
        // Bridge
        const int TOTAL_PLANKS = 10;
        //bridge plank
        for (int i = 0; i < TOTAL_PLANKS; i++)
        {
            std::string name = "Bridge" + std::to_string(i);
            m_pGameObjects[name] = new GameObject(this, "Bridge", vec3(0, -0.5f, (i * 2.2f) +25), vec3(0, 0, 0), vec3(3, 0.3f, 1), "Cube", "Red");
            if (i == 0 || i == TOTAL_PLANKS - 1)
            {
                m_pGameObjects[name]->CreateBody(0, btVector3(3, 0.3f, 1));
            }
            else
            {
                m_pGameObjects[name]->CreateBody(1000, btVector3(3, 0.3f, 1));
            }
        }
        //bridge constraint
        for (unsigned int j = 0; j < TOTAL_PLANKS - 1; j++)
        {
        btRigidBody* b1 = m_pGameObjects["Bridge" + std::to_string(j)]->GetBody();
        btRigidBody* b2 = m_pGameObjects["Bridge" + std::to_string(j + 1)]->GetBody();
        
        btPoint2PointConstraint* leftSpring = new btPoint2PointConstraint(*b1, *b2, btVector3(-1, 0, 1.2 ), btVector3(-1, 0, -1.2));
        m_pBulletWorld->GetDynamicsWorld()->addConstraint(leftSpring);

        btPoint2PointConstraint* rightSpring = new btPoint2PointConstraint(*b1, *b2, btVector3(1, 0, 1.2), btVector3(1, 0, -1.2));
        m_pBulletWorld->GetDynamicsWorld()->addConstraint(rightSpring);
        }

        // Tree
        for (int i = 0; i < 15; i++)
        {
            float ranTall = ((rand() % 10) + 1) * 0.08 + 0.2; //range from 0.2~1.0
            
            float randPosX = (rand() % 70) - 34; // range -34 ~ 35
            float randPosZ = (rand() % 45) - 20; // range -20 ~ 24
            
            //make sure not spawn on the fire and bridge head 
            if (randPosX == 0 && randPosZ == 10 || randPosX == 0)
            {
                float randPosX = (rand() % 70) - 34; // range -34 ~ 35
                float randPosZ = (rand() % 45) - 20; // range -20 ~ 24
            } 

            int treeTypeIndex = i % TotalNumOfTreeTypes;
            std::string name = "Tree" + std::to_string(i);
            std::string type = "Tree" + std::to_string(treeTypeIndex);
            m_pGameObjects[name] = new Breakable_Tree(this, name, vec3(randPosX , -0.5f, randPosZ), vec3(0, 0, 0), vec3(0.5f, ranTall, 0.5f), type, "Green");
            m_pGameObjects[name]->CreateBody(0, btVector3(1, 3, 1));
        }
        // Rock
        for (int i = 0; i < 20; i++)
        {
            float randPosX = (rand() % 70) - 34; // range -34 ~ 35
            float randPosZ = (rand() % 45) - 20; // range -20 ~ 24

            //make sure not spawn on the fire and bridge head 
            if (randPosX == 0 && randPosZ == 10 || randPosX == 0)
            {
                float randPosX = (rand() % 70) - 34; // range -34 ~ 35
                float randPosZ = (rand() % 45) - 20; // range -20 ~ 24
            }

            int rockTypeIndex = i % TotalNumOfRockTypes;
            std::string name = "Rock" + std::to_string(i);
            std::string type = "Rock" + std::to_string(rockTypeIndex);
            m_pGameObjects[name] = new GameObject(this, name, vec3(randPosX, -0.5f, randPosZ), vec3(0, 0, 0), vec3(0.25f, 0.25f, 0.25f), type, "Blue");
            m_pGameObjects[name]->CreateBody(0, btVector3(1, 1.5f, 1));
        }
        //otherland rock
        for (int i = 0; i < 20; i++)
        {
            float randPosX = (rand() % 45) - 20; // range -34 ~ 35
            float randPosZ = (rand() % 45) - 20; // range -20 ~ 24

                                                 //make sure not spawn on the fire and bridge head 
            if (randPosX == 0 && randPosZ == 10 || randPosX == 0)
            {
                float randPosX = (rand() % 45) - 20; // range -34 ~ 35
                float randPosZ = (rand() % 45) - 20; // range -20 ~ 24
            }

            int rockTypeIndexTwo = i % TotalNumOfRockTypes;
            std::string nameTwo = "RockTwo" + std::to_string(i);
            std::string typeTwo = "RockTwo" + std::to_string(rockTypeIndexTwo);
            m_pGameObjects[nameTwo] = new GameObject(this, nameTwo, vec3(randPosX, -0.5f, randPosZ + 70), vec3(0, 0, 0), vec3(0.25f, 0.25f, 0.25f), typeTwo, "Blue");
            m_pGameObjects[nameTwo]->CreateBody(0, btVector3(1, 1.5f, 1));
        }

        // grass
        for (int i = 0; i < 30; i++)
        {
            float randPosX = (rand() % 70) - 34; // range -34 ~ 35
            float randPosZ = (rand() % 45) - 20; // range -20 ~ 24

                                                 //make sure not spawn on the fire and bridge head 
            if (randPosX == 0 && randPosZ == 10 || randPosX == 0 )
            {
                float randPosX = (rand() % 70) - 34; // range -34 ~ 35
                float randPosZ = (rand() % 45) - 20; // range -20 ~ 24
            }
            int grassTypeIndex = i % TotalNumOfGrassTypes;
            std::string name = "Grass" + std::to_string(i);
            std::string type = "Grass" + std::to_string(grassTypeIndex);
            m_pGameObjects[name] = new GameObject(this, name, vec3(randPosX, -0.5f, randPosZ), vec3(0), vec3(2), type, "Green");          
        }
        
        //other land grass
        for (int i = 0; i < 30; i++)
        {
            float randPosX = (rand() % 45) - 20; // range -34 ~ 35
            float randPosZ = (rand() % 45) - 20; // range -20 ~ 24

                                                 //make sure not spawn on the fire and bridge head 
            if (randPosX == 0 && randPosZ == 10 || randPosX == 0)
            {
                float randPosX = (rand() % 45) - 20; // range -34 ~ 35
                float randPosZ = (rand() % 45) - 20; // range -20 ~ 24
            }
            int grassTypeIndexTwo = i % TotalNumOfGrassTypes;
            std::string nameTwo = "GrassTwo" + std::to_string(i);
            std::string typeTwo = "GrassTwo" + std::to_string(grassTypeIndexTwo);
            m_pGameObjects[nameTwo] = new GameObject(this, nameTwo, vec3(randPosX, -0.5f, randPosZ + 70), vec3(0), vec3(2), typeTwo, "Green");
        }

        // Apple
        for (int i = 0; i < 4; i++)
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
            m_pGameObjects[name] = new Pickup_Apple(this, name, vec3(randPosX, 0, randPosZ+70), vec3(0, 0, 0), vec3(0.5f), "Cube", "Blue");
            m_pGameObjects[name]->CreateBody(0, btVector3(1, 1, 1));
        }
        //mushroom
        for (int i = 0; i < 5; i++)
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

        m_Skybox = new GameObject(this, "Skybox", 0, vec3(0, 2.0f, 0), vec3(10, 10, 10), "Cube", "Skybox1");
    }
}

void GameScene_Level1::Update(float deltatime)
{
    Scene::Update(deltatime);
    m_ParticleEmitter->Update(deltatime);
    m_Skybox->Update(deltatime);

    if (m_ShowImgui)
    {
        ImGui::Begin("Imgui Debugging");
        PlayerObject* pPlayer = (PlayerObject*)GetGameObject("Player");
        if (pPlayer)
        {
            pPlayer->m_MouseLock = false;
            ImGui::Checkbox("Poisoned", &pPlayer->m_Poisoned);
        }

        ImGui::End();
    }
}

void GameScene_Level1::Draw()
{
    Scene::Draw();
}

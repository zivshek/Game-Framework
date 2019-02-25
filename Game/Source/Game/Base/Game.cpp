#include "GamePCH.h"

Game::Game(Framework* pFramework)
: GameCore( pFramework )
{
    m_pImGuiManager = 0;

    m_WindowWidth = -1;
    m_WindowHeight = -1;

    m_pResources = 0;
	m_pSceneManager = 0;
    m_pTextRenderer = 0;
    m_CurrentSelectedVolume = m_VolumeToggle = 100.0f;
}

Game::~Game()
{
    // Save our settings and survival time to file when closing the game
    SaveFile();

    while( m_EventQueue.size() != 0 )
    {
        Event* pEvent = m_EventQueue.front();
        m_EventQueue.pop();
        delete pEvent;
    }

	delete m_pTextRenderer;
    delete m_pSceneManager;
    delete m_pImGuiManager;
    delete m_pResources;
    delete m_pSoundPlayer;

    for (auto t : m_TweenPool)
        delete t;
}

void Game::OnSurfaceChanged(unsigned int width, unsigned int height)
{
    glViewport( 0, 0, width, height );

    m_WindowWidth = width;
    m_WindowHeight = height;

    if (m_pTextRenderer)
        m_pTextRenderer->OnSurfaceChanged(width, height);

}

void Game::LoadContent()
{
    // Seed random.
    srand( (unsigned int)GetSystemTime() );
    
    CheckForGLErrors();

    // Turn on depth buffer testing.
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

    // Turn on alpha blending.
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    m_pResources = new ResourceManager();

    // Tween objects
    for (int i = 0; i < 10; i++)
        m_TweenPool.push_back(new TweenObject());

    // Load our shaders.
    {
        m_pResources->AddShader("Lighting", new ShaderProgram("Data/Shaders/lighting.vert", "Data/Shaders/lighting.frag", "Data/Shaders/helpers.txt"));
        m_pResources->AddShader("Skybox", new ShaderProgram("Data/Shaders/skybox.vert", "Data/Shaders/skybox.frag", "Data/Shaders/helpers.txt"));
        m_pResources->AddShader("Particle", new ShaderProgram("Data/Shaders/particle.vert", "Data/Shaders/particle.frag", "Data/Shaders/helpers.txt"));
        m_pResources->AddShader("PostprocessNothing", new ShaderProgram("Data/Shaders/postprocessnothing.vert", "Data/Shaders/postprocessnothing.frag", "Data/Shaders/helpers.txt"));
        m_pResources->AddShader("PostprocessPoisonous", new ShaderProgram("Data/Shaders/postprocesspoisonous.vert", "Data/Shaders/postprocesspoisonous.frag", "Data/Shaders/helpers.txt"));
        m_pResources->AddShader("2DTexture", new ShaderProgram("Data/Shaders/2DTexture.vert", "Data/Shaders/2DTexture.frag", "Data/Shaders/helpers.txt"));
        m_pResources->AddShader("Text", new ShaderProgram("Data/Shaders/Text.vert", "Data/Shaders/Text.frag", "Data/Shaders/helpers.txt"));
    }

    // Create meshes.
    {
        m_pResources->AddMesh("Box", Mesh::CreateBox(vec2(1, 1)));
        m_pResources->AddMesh("Cube", Mesh::CreateCube(vec3(1, 1, 1)));
        vec2 worldsize = vec2(1000, 1000);
        m_pResources->AddMesh("Plane", Mesh::CreatePlane(-worldsize / 2.0f, ivec2(20, 20), worldsize, vec2(5, 5)));
        std::vector< vec3 > vertices, normals;
        std::vector< vec2 > uvs;
        // Load the tree meshes
        BatchLoadMeshes("Data/Models/tree", "Tree", TotalNumOfTreeTypes, vertices, uvs, normals);
        // Load the rock meshes
        BatchLoadMeshes("Data/Models/rock", "Rock", TotalNumOfRockTypes, vertices, uvs, normals);
        BatchLoadMeshes("Data/Models/rock", "RockTwo", TotalNumOfRockTypes, vertices, uvs, normals);
        // Load the grass meshes
        BatchLoadMeshes("Data/Models/grass", "Grass", TotalNumOfGrassTypes, vertices, uvs, normals);
        BatchLoadMeshes("Data/Models/grass", "GrassTwo", TotalNumOfGrassTypes, vertices, uvs, normals);
        // Boxes
        BatchLoadMeshes("Data/Models/box", "Box", 1, vertices, uvs, normals);
        // Campfire
        BatchLoadMeshes("Data/Models/fire", "Campfire", 1, vertices, uvs, normals);
    }

    // Load all sounds
    {
        m_pSoundPlayer = new SoundPlayer();

        // Add sound files
        // Looping sounds
        m_pResources->AddSound("CampFire0", m_pSoundPlayer->LoadSound("Data/Audio/Loop/CampFire0.wav", true));

        // One-time sounds
        m_pResources->AddSound("Barnowl0", m_pSoundPlayer->LoadSound("Data/Audio/Loop/Barnowl0.wav", false));
        m_pResources->AddSound("Environment0", m_pSoundPlayer->LoadSound("Data/Audio/Loop/Environment0.wav", false));
        m_pResources->AddSound("Environment1", m_pSoundPlayer->LoadSound("Data/Audio/Loop/Environment1.wav", false));
        m_pResources->AddSound("Environment2", m_pSoundPlayer->LoadSound("Data/Audio/Loop/Environment2.wav", false));
        m_pResources->AddSound("Owl0", m_pSoundPlayer->LoadSound("Data/Audio/Loop/Owl0.wav", false));
        
        m_pResources->AddSound("Bridge0", m_pSoundPlayer->LoadSound("Data/Audio/Once/Bridge0.wav",false));
        m_pResources->AddSound("Bridge1", m_pSoundPlayer->LoadSound("Data/Audio/Once/Bridge1.wav", false));
        m_pResources->AddSound("Chop0", m_pSoundPlayer->LoadSound("Data/Audio/Once/Chop0.wav", false));
        m_pResources->AddSound("FootStep0", m_pSoundPlayer->LoadSound("Data/Audio/Once/FootStep0.wav", false));
        m_pResources->AddSound("FootStep1", m_pSoundPlayer->LoadSound("Data/Audio/Once/FootStep1.wav", false));
        m_pResources->AddSound("FootStepGrass0", m_pSoundPlayer->LoadSound("Data/Audio/Once/FootStepGrass0.wav", false));
        
        // Load sound queues
        m_pResources->AddSoundQueue("EnvironmentSounds", new SoundQueue(m_pResources));
        m_pResources->AddSoundQueue("FireSound", new SoundQueue(m_pResources));

        // Add sounds to sound queues
        m_pResources->GetSoundQueue("EnvironmentSounds")->AddSoundByName("Barnowl0");
        m_pResources->GetSoundQueue("EnvironmentSounds")->AddSoundByName("Environment0");
        m_pResources->GetSoundQueue("EnvironmentSounds")->AddSoundByName("Environment1");
        m_pResources->GetSoundQueue("EnvironmentSounds")->AddSoundByName("Environment2");
        m_pResources->GetSoundQueue("EnvironmentSounds")->AddSoundByName("Owl0");
        
        m_pResources->GetSoundQueue("FireSound")->AddSoundByName("CampFire0"); 
    }

    // Load our textures.
    {
        m_pResources->AddTexture("Blue", LoadTexture("Data/Textures/Blue.png"));
        m_pResources->AddTexture("Green", LoadTexture("Data/Textures/Green.png"));
        m_pResources->AddTexture("Grass", LoadTexture("Data/Textures/Meadowlands.png"));
        m_pResources->AddTexture("Red", LoadTexture("Data/Textures/Red.png"));
       
        m_pResources->AddTexture("Game_Level1", LoadTexture("Data/Textures/MenuItem_Play1.png"));
        m_pResources->AddTexture("Game_Level2", LoadTexture("Data/Textures/MenuItem_Play2.png"));

        m_pResources->AddTexture("MenuItem_Level", LoadTexture("Data/Textures/MenuItem_Play.png"));
        m_pResources->AddTexture("MenuItem_Option", LoadTexture("Data/Textures/MenuItem_Option.png"));
        m_pResources->AddTexture("MenuItem_Quit", LoadTexture("Data/Textures/MenuItem_Quit.png"));

        m_pResources->AddTexture("Option_Volume", LoadTexture("Data/Textures/Option_Volume.png"));
        m_pResources->AddTexture("Option_Brightness", LoadTexture("Data/Textures/Option_Brightness.png"));

		m_pResources->AddTexture("VolumeToggle", LoadTexture("Data/Textures/Green.png"));
		m_pResources->AddTexture("BrightnessToggle", LoadTexture("Data/Textures/Green.png"));
		m_pResources->AddTexture("VolumeBar", LoadTexture("Data/Textures/Green.png"));
		m_pResources->AddTexture("BrightnessBar", LoadTexture("Data/Textures/Green.png"));
       
        m_pResources->AddTexture("Hud_Time", LoadTexture("Data/Textures/Hud_Time.png"));
        m_pResources->AddTexture("Hud_Hunger", LoadTexture("Data/Textures/Hud_Hunger.png"));

        m_pResources->AddTexture("Inventory", LoadTexture("Data/Textures/Inventory.png"));
        m_pResources->AddTexture("InventoryItem_Empty", LoadTexture("Data/Textures/InventoryItem_Empty.png"));
        m_pResources->AddTexture("InventoryItem_Apple", LoadTexture("Data/Textures/InventoryItem_Apple.png"));
        m_pResources->AddTexture("InventoryItem_Log", LoadTexture("Data/Textures/InventoryItem_Log.png"));
        m_pResources->AddTexture("InventoryItem_Mushroom", LoadTexture("Data/Textures/InventoryItem_Mushroom.png"));

        std::vector<const char*> faces1
        {
            "Data/Textures/Skybox/right.png",
            "Data/Textures/Skybox/left.png",
            "Data/Textures/Skybox/top.png",
            "Data/Textures/Skybox/bottom.png",
            "Data/Textures/Skybox/front.png",
            "Data/Textures/Skybox/back.png",
        };
        std::vector<const char*> faces2
        {                      
            "Data/Textures/Skybox2/right.png",
            "Data/Textures/Skybox2/left.png",
            "Data/Textures/Skybox2/top.png",
            "Data/Textures/Skybox2/bottom.png",
            "Data/Textures/Skybox2/front.png",
            "Data/Textures/Skybox2/back.png",
        };

        m_pResources->AddTexture("Skybox1", LoadTextureCubemap(faces1));
        m_pResources->AddTexture("Skybox2", LoadTextureCubemap(faces2));
    }

    // Create our materials.
    {
        m_pResources->AddMaterial("Blue", new Material(m_pResources->GetShader("Lighting"), m_pResources->GetTexture("Blue"), 50.0f, vec3(1, 1, 1)));
        m_pResources->AddMaterial("Green", new Material(m_pResources->GetShader("Lighting"), m_pResources->GetTexture("Green"), 70.0f, vec3(1, 1, 1)));
        m_pResources->AddMaterial("Grass", new Material(m_pResources->GetShader("Lighting"), m_pResources->GetTexture("Grass"), 30.0f, vec3(1, 1, 1)));       
        m_pResources->AddMaterial("Red", new Material(m_pResources->GetShader("Lighting"), m_pResources->GetTexture("Red"), 120.0f, vec3(1, 1, 1)));
       
        m_pResources->AddMaterial("Skybox1", new Material(m_pResources->GetShader("Skybox"), m_pResources->GetTexture("Skybox1"), 120.0f, vec3(1, 1, 1)));       
        m_pResources->AddMaterial("Skybox2", new Material(m_pResources->GetShader("Skybox"), m_pResources->GetTexture("Skybox2"), 120.0f, vec3(1, 1, 1)));
        
        m_pResources->AddMaterial("Game_Level1", new Material(m_pResources->GetShader("2DTexture"), m_pResources->GetTexture("Game_Level1"), 120.0f, vec3(1, 1, 1)));
        m_pResources->AddMaterial("Game_Level2", new Material(m_pResources->GetShader("2DTexture"), m_pResources->GetTexture("Game_Level2"), 120.0f, vec3(1, 1, 1)));
        
        m_pResources->AddMaterial("MenuItem_Level", new Material(m_pResources->GetShader("2DTexture"), m_pResources->GetTexture("MenuItem_Level"), 120.0f, vec3(1, 1, 1)));
        m_pResources->AddMaterial("MenuItem_Option", new Material(m_pResources->GetShader("2DTexture"), m_pResources->GetTexture("MenuItem_Option"), 120.0f, vec3(1, 1, 1)));
        m_pResources->AddMaterial("MenuItem_Quit", new Material(m_pResources->GetShader("2DTexture"), m_pResources->GetTexture("MenuItem_Quit"), 120.0f, vec3(1, 1, 1)));
       
        m_pResources->AddMaterial("Option_Volume", new Material(m_pResources->GetShader("2DTexture"), m_pResources->GetTexture("Option_Volume"), 120.0f, vec3(1, 1, 1)));
        m_pResources->AddMaterial("Option_Brightness", new Material(m_pResources->GetShader("2DTexture"), m_pResources->GetTexture("Option_Brightness"), 120.0f, vec3(1, 1, 1)));
        
		m_pResources->AddMaterial("VolumeToggle", new Material(m_pResources->GetShader("2DTexture"), m_pResources->GetTexture("VolumeToggle"), 120.0f, vec3(1, 1, 1)));
		m_pResources->AddMaterial("BrightnessToggle", new Material(m_pResources->GetShader("2DTexture"), m_pResources->GetTexture("BrightnessToggle"), 120.0f, vec3(1, 1, 1)));
		m_pResources->AddMaterial("VolumeBar", new Material(m_pResources->GetShader("2DTexture"), m_pResources->GetTexture("VolumeBar"), 120.0f, vec3(1, 1, 1)));
		m_pResources->AddMaterial("BrightnessBar", new Material(m_pResources->GetShader("2DTexture"), m_pResources->GetTexture("BrightnessBar"), 120.0f, vec3(1, 1, 1)));

        m_pResources->AddMaterial("Hud_Time", new Material(m_pResources->GetShader("2DTexture"), m_pResources->GetTexture("Hud_Time"), 120.0f, vec3(1, 1, 1)));
        m_pResources->AddMaterial("Hud_Hunger", new Material(m_pResources->GetShader("2DTexture"), m_pResources->GetTexture("Hud_Hunger"), 120.0f, vec3(1, 1, 1)));

        m_pResources->AddMaterial("Inventory", new Material(m_pResources->GetShader("2DTexture"), m_pResources->GetTexture("Inventory"), 120.0f, vec3(1, 1, 1)));
        m_pResources->AddMaterial("InventoryItem_Empty", new Material(m_pResources->GetShader("2DTexture"), m_pResources->GetTexture("InventoryItem_Empty"), 120.0f, vec3(1, 1, 1)));
        m_pResources->AddMaterial("InventoryItem_Apple", new Material(m_pResources->GetShader("2DTexture"), m_pResources->GetTexture("InventoryItem_Apple"), 120.0f, vec3(1, 1, 1)));
        m_pResources->AddMaterial("InventoryItem_Mushroom", new Material(m_pResources->GetShader("2DTexture"), m_pResources->GetTexture("InventoryItem_Mushroom"), 120.0f, vec3(1, 1, 1)));
        m_pResources->AddMaterial("InventoryItem_Log", new Material(m_pResources->GetShader("2DTexture"), m_pResources->GetTexture("InventoryItem_Log"), 120.0f, vec3(1, 1, 1)));
    }

    // Load the shader and texture for our TextRenderer
    unsigned int w, h;
    m_pResources->AddTexture("Text", LoadTexture("Data/Textures/PersonalFontMap.png", &w, &h));
    // Create the text renderer
    m_pTextRenderer = new TextRenderer(m_pResources, w, h, m_WindowWidth, m_WindowHeight);
    // Create the scene manager
	m_pSceneManager = new SceneManager();

    // Add the scenes
    m_pSceneManager->AddScene("MainMenuScene", new Scene_MainMenu(this, m_pResources, m_pTextRenderer));
    m_pSceneManager->AddScene("OptionScene", new Scene_Option(this, m_pResources, m_pTextRenderer));
    m_pSceneManager->AddScene("LevelScene", new Scene_Level(this, m_pResources, m_pTextRenderer));
    m_pSceneManager->AddScene("DeadScene", new Scene_Dead(this, m_pResources, m_pTextRenderer));
	m_pSceneManager->AddScene("GameScene_Level1", new GameScene_Level1(this, m_pResources, m_pTextRenderer));
    m_pSceneManager->AddScene("InventoryScene", new Scene_Inventory(this, m_pResources, m_pTextRenderer));
    m_pSceneManager->GetScene("InventoryScene")->SetPlayer(m_pSceneManager->GetScene("GameScene_Level1")->GetPlayer());
   
    m_pSceneManager->AddScene("HudScene", new Scene_Hud(this, m_pResources, m_pTextRenderer));
    m_pSceneManager->GetScene("HudScene")->SetParent(m_pSceneManager->GetScene("GameScene_Level1"));
	
    m_pSceneManager->AddScene("GameScene_Level2", new GameScene_Level2(this, m_pResources, m_pTextRenderer));
    //m_pSceneManager->GetScene("InventoryScene")->SetPlayer(m_pSceneManager->GetScene("GameScene_Level2")->GetPlayer());
    //m_pSceneManager->GetScene("HudScene")->SetParent(m_pSceneManager->GetScene("GameScene_Level2"));
	
    m_pSceneManager->PushScene("MainMenuScene");
    
    // Create the ImGui manager
	m_pImGuiManager = new ImGuiManager();
    m_pImGuiManager->Init();

    CheckForGLErrors();

    // Load settings and longest survival time
    LoadFromFile("Data/Saves/Save0.SAV");

    m_pSoundPlayer->PlaySound(m_pResources->GetSoundQueue("FireSound")->GrabASound());
    m_pSoundPlayer->PlaySound(m_pResources->GetSoundQueue("EnvironmentSounds")->GrabASound());
}

void Game::LoadFromFile(const char* filename)
{
    long len;
    const char* filecontents = LoadCompleteFile(filename, &len);
    cJSON* jRoot = cJSON_Parse(filecontents);

    cJSON* jVolume = cJSON_GetObjectItemCaseSensitive(jRoot, "Volume");
    if (jVolume)
    {
        float volume = (float)(jVolume->valuedouble);
        if (m_pSoundPlayer)
            m_pSoundPlayer->SetMasterVolume(volume);
    }

    cJSON* jMap1Survived = cJSON_GetObjectItemCaseSensitive(jRoot, "Map1 Survived");
    if (jMap1Survived)
    {
        float map1Survived = (float)(jMap1Survived->valuedouble);
        m_pSceneManager->GetScene("GameScene_Level1")->GetPlayer()->m_Survived = map1Survived;
    }

    cJSON_Delete(jRoot);
    delete[] filecontents;
}

void Game::SaveFile()
{
    char *string = NULL;
    cJSON *volume = NULL;
    cJSON *jroot = cJSON_CreateObject();

    if(cJSON_AddNumberToObject(jroot, "Volume", m_CurrentSelectedVolume) == NULL)
    {
        cJSON_Delete(jroot);
    }

    if (cJSON_AddNumberToObject(jroot, "Map1 Survived", m_pSceneManager->GetScene("GameScene_Level1")->GetPlayer()->m_Survived) == NULL)
    {
        cJSON_Delete(jroot);
    }

    string = cJSON_Print(jroot);
    FILE* filehandle;
    errno_t error = fopen_s(&filehandle, "Data/Saves/Save0.SAV", "wb");
    if (filehandle)
    {
        fwrite(string, 1, strlen(string), filehandle);
        fclose(filehandle);
    }

    cJSON_Delete(jroot);
}

void Game::QueueEvent(Event* pEvent)
{
    m_EventQueue.push( pEvent );
}

void Game::ProcessEvents()
{
    while( m_EventQueue.size() != 0 )
    {
        Event* pEvent = m_EventQueue.front();
        m_EventQueue.pop();

        OnEvent( pEvent );
        delete pEvent;
    }
}

void Game::OnEvent(Event* pEvent)
{
    if (pEvent->GetEventType() == EventType_Input)
    {
        InputEvent* e = static_cast<InputEvent*>(pEvent);
        if (e->GetInputState() == InputState_Pressed)
        {
            if (e->GetID() == VK_ESCAPE)
            {
                // Pop a scene if there are more than 1 scene when user hits "Escape"
                if (m_pSceneManager->GetActiveScenes().size() > 1)
                {
                    m_pSceneManager->Pop();     
                }
                // Else, just request close the program in framework
                else
                    m_pFramework->RequestCloseProgram();     
            }
        }
    }

    m_pImGuiManager->OnEvent( pEvent );

	m_pSceneManager->OnEvent(pEvent);

    m_Controllers[0].OnEvent( pEvent );
}

void Game::Update(float deltatime)
{
    // Process events at the start of the frame before updating anything else in the game.
    ProcessEvents();

    // Once our events are processes, tell imgui we're starting a new frame.
    m_pImGuiManager->StartFrame( (float)m_WindowWidth, (float)m_WindowHeight, deltatime );

    // Update the volume
    if (m_VolumeToggle != m_CurrentSelectedVolume)
    {
        m_pSoundPlayer->SetMasterVolume(m_VolumeToggle);
        m_CurrentSelectedVolume = m_VolumeToggle;
    }

	m_pSceneManager->Update( deltatime );
}

void Game::Draw()
{
    glClearColor(0, 0, 0.4f, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_pSceneManager->Draw();

	/*NewLineInfo newLineInfo;
	float charheight = m_pTextRenderer->GetCharacterHeight();
	m_pTextRenderer->printText2D("Need some food! Need some food! Need some food! Need some food!", 16, m_WindowHeight - 2 * charheight, 16, 20, &newLineInfo);*/

    CheckForGLErrors();

    // Tell imgui we're at the end of our frame, so it will draw all queued objects.
    m_pImGuiManager->EndFrame();

    CheckForGLErrors();
}

bool Game::LoadObj(const char* filename, std::vector<vec3>& vertices_out, std::vector<vec2>& uvs_out, std::vector<vec3>& normals_out)
{
    long length = 0;
    char* buffer = LoadCompleteFile(filename, &length);
    if (buffer == 0 || length == 0)
    {
        delete buffer;
        return false;
    }
    char* next_token = 0;
    char* line = strtok_s(buffer, "\n", &next_token);

    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< vec3 > temp_vertices;
    std::vector< vec2 > temp_uvs;
    std::vector< vec3 > temp_normals;

    while (line)
    {
        char line_head[10];
        // read the first word of the line
        sscanf_s(line, "%s", line_head, 10);

        if (strcmp(line_head, "v") == 0)
        {
            char str[10];
            vec3 vertex;
            sscanf_s(line, "%s %f %f %f\n", str, 10, &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        }
        else if (strcmp(line_head, "vn") == 0)
        {
            char str[10];
            vec3 normal;
            sscanf_s(line, "%s %f %f %f\n", str, 10, &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        }
        else if (strcmp(line_head, "vt") == 0)
        {
            char str[10];
            vec2 uv;
            sscanf_s(line, "%s %f %f ", str, 10, &uv.x, &uv.y);
            temp_uvs.push_back(uv);
        }
        else if (strcmp(line_head, "f") == 0)
        {
            char str[10];
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            sscanf_s(line, "%s %d/%d/%d %d/%d/%d %d/%d/%d\n", str, 10,
                &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }

        line = strtok_s(0, "\n", &next_token);
    }

    for (unsigned int i = 0; i < vertexIndices.size(); i++)
    {
        unsigned int vertexIndex = vertexIndices[i];
        vec3 vertex = temp_vertices[vertexIndex - 1];
        vertices_out.push_back(vertex);
        unsigned int uvIndex = uvIndices[i];
        vec2 uv = temp_uvs[uvIndex - 1];
        uvs_out.push_back(uv);
        unsigned int normalIndex = normalIndices[i];
        vec3 normal = temp_normals[normalIndex - 1];
        normals_out.push_back(normal);
    }

    delete[] buffer;
    return true;
}

void Game::BatchLoadMeshes(std::string pathhead, std::string namehead, int numoftypes, std::vector<vec3>& vertices_out, std::vector<vec2>& uvs_out, std::vector<vec3>& normals_out)
{
    for (int i = 0; i < numoftypes; i++)
    {
        std::string path = pathhead + std::to_string(i) + ".obj";
        if (LoadObj(path.c_str(), vertices_out, uvs_out, normals_out))
        {
            std::string name = namehead + std::to_string(i);
            m_pResources->AddMesh(name.c_str(), Mesh::CreateMeshForModel(vertices_out, uvs_out, normals_out));
            vertices_out.clear();
            uvs_out.clear();
            normals_out.clear();
        }
    }
}

#include "GamePCH.h"

Scene::Scene(Game* pGame, ResourceManager* pResources, TextRenderer* pTextRenderer)
{
    m_pGame = pGame;
    m_pResources = pResources;
	m_pTextRenderer = pTextRenderer;
	m_bOpacity = true;
    m_pBulletWorld = 0;
    m_FBO = new FBODefinition();
    m_FBO->Create(1024, 1024, GL_NEAREST, GL_NEAREST, true, 16, false);
    m_Skybox = 0;
    m_ParticleEmitter = 0;
    m_ParticleRenderer = 0;
    m_pChild = nullptr;
    m_pParent = nullptr;
    m_pPlayer = nullptr;
    m_ShouldDrawText = false;
    // By default, just render a quad with out any modifications
    m_pPostProcessShader = m_pResources->GetShader("PostprocessNothing");
}

Scene::~Scene()
{
    for( auto object: m_pGameObjects )
    {
        if (object.second != nullptr)
        {
            delete object.second;
        }
    }

	for (auto light : AllLights)
	{
		delete light.second;
	}

    delete m_pBulletWorld;
    if (m_Skybox)
        delete m_Skybox;
    delete m_FBO;
    delete m_ParticleEmitter;
    delete m_ParticleRenderer;
}

void Scene::OnSurfaceChanged(unsigned int width, unsigned int height)
{
}

void Scene::LoadContent()
{
}

bool Scene::OnEvent(Event* pEvent)
{
	return false;
}

void Scene::Update(float deltatime)
{
    CheckForGLErrors();

    if (m_pBulletWorld)
        m_pBulletWorld->Update(deltatime);

    // Update all of the Scene objects in the list.
    for( auto object: m_pGameObjects )
    {
        object.second->Update( deltatime );
    }

    CheckForGLErrors();
}

void Scene::Draw()
{
    BindFBO();
    CameraObject* pCamera = (CameraObject*)GetGameObject("Camera");
    if (pCamera)
    {
        DrawSkybox(pCamera);
        DrawSceneObjects(pCamera);
        if (m_ShouldDrawText)
            DrawTexts();
        if(m_ParticleRenderer)
            m_ParticleRenderer->Draw(pCamera->GetViewMatrix(), pCamera->GetProjMatrix());
    }
    UnbindFBO();

    glViewport(0, 0, m_pGame->GetWindowWidth(), m_pGame->GetWindowHeight());

    Mesh* mesh = m_pResources->GetMesh("Box");
    mesh->SetupAttributes(m_pPostProcessShader);
    mesh->Setup2DUniforms(m_pPostProcessShader, m_FBO->GetColorTextureHandle(), 0, 0);
    PlayerObject* pPlayer = (PlayerObject*)GetGameObject("Player");
    if(pPlayer)
        mesh->SetupIngameUniforms(m_pPostProcessShader, pPlayer->m_Poisoned);
    mesh->Draw(m_pPostProcessShader);
}

void Scene::BindFBO()
{
    m_FBO->Bind();

    // Clear the screen to dark blue.
    glClearColor(0, 0, 0.4f, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, 1024, 1024);
}

void Scene::UnbindFBO()
{
    m_FBO->Unbind();
    CheckForGLErrors();
}

void Scene::DrawSkybox(CameraObject* pCamera)
{
    if (m_Skybox)
    {
        glFrontFace(GL_CCW);
        glDepthMask(GL_FALSE);
        m_Skybox->Draw(5, pCamera->GetViewMatrix(), pCamera->GetProjMatrix(), pCamera->GetPosition());
        glDepthMask(GL_TRUE);
        glFrontFace(GL_CW);
    }
}

void Scene::DrawSceneObjects(CameraObject* pCamera)
{
    vec3 campos = pCamera->GetPosition();
    mat4* pMatView = pCamera->GetViewMatrix();
    mat4* pMatProj = pCamera->GetProjMatrix();

    // Render all of the Scene objects in the list.
    for (int i = 0; i<5; i++)
    {
        //for( std::map<std::string, GameObject*>::iterator iterator = m_pGameObjects.begin(); iterator != m_pGameObjects.end(); iterator++ )
        for (auto object : m_pGameObjects)
        {
            if (object.second->GetRenderOrder() == i)
            {
                object.second->Draw(i, pMatView, pMatProj, campos);
            }
        }
    }
}

void Scene::DrawTexts()
{
}

void Scene::Reset()
{
	for (auto object : m_pGameObjects)
	{
		object.second->Reset();
	}
}

void Scene::AddToScene(std::string name, GameObject* object)
{
	object->SetScene(this);
	m_pGameObjects[name] = object;
}

void Scene::Destroy(GameObject* pObject)
{
	auto itr = m_pGameObjects.find(pObject->GetName());
	if (itr != m_pGameObjects.end())
	{
		delete itr->second;
		m_pGameObjects.erase(itr);
	}
}

GameObject * Scene::GetGameObject(const char * name)
{
    auto itr = m_pGameObjects.find(name);
    if (itr != m_pGameObjects.end())
    {
        return itr->second;
    }
    else
        return nullptr;
}

void Scene::SetParent(Scene * parent)
{
    m_pParent = parent;
    m_pParent->m_pChild = this;
}



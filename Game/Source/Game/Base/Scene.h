#ifndef __Scene_H__
#define __Scene_H__

class Game;
class TextRenderer;
class GameObject;

class Scene
{
protected:
    Game* m_pGame;
    ResourceManager* m_pResources;
	TextRenderer* m_pTextRenderer;

    std::map<std::string, GameObject*> m_pGameObjects;

    BulletWorld* m_pBulletWorld;

	bool m_bOpacity;

    FBODefinition* m_FBO;
    GameObject* m_Skybox;
    ParticleEmitter* m_ParticleEmitter;
    ParticleRenderer* m_ParticleRenderer;
    ShaderProgram* m_pPostProcessShader;

    PlayerObject* m_pPlayer;
    bool m_ShouldDrawText;

public:
    Scene* m_pChild;
    Scene* m_pParent;

public:
    std::map<int, Light*> AllLights;

public:
    Scene(Game* pGame, ResourceManager* pResources, TextRenderer* pTextRenderer);
    virtual ~Scene();

    virtual void OnSurfaceChanged(unsigned int width, unsigned int height);
    virtual void LoadContent();

    virtual bool OnEvent(Event* pEvent);
    virtual void Update(float deltatime);
    virtual void Draw();
    virtual void DrawSceneObjects(CameraObject* pCamera);
    virtual void DrawTexts();
    virtual void BindFBO();
    virtual void UnbindFBO();
    virtual void DrawSkybox(CameraObject* pCamera);

	virtual void Reset();

	void AddToScene(std::string name, GameObject* object);
	void Destroy(GameObject* pObject);

    Game* GetGame() { return m_pGame; }
    ResourceManager* GetResourceManager() { return m_pResources; }

    GameObject* GetGameObject(const char* name);

    BulletWorld* GetBulletWorld() { return m_pBulletWorld; }
    bool IsOpaque() { return m_bOpacity; }

    void SetParent(Scene* parent);

    PlayerObject* GetPlayer() { return m_pPlayer; }
    void SetPlayer(PlayerObject* pPlayer) { m_pPlayer = pPlayer; }
};

#endif //__Scene_H__

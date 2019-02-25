#ifndef __Game_H__
#define __Game_H__

class ShaderProgram;

// Need to update these values if new models are added
const int TotalNumOfTreeTypes = 3;
const int TotalNumOfRockTypes = 3;
const int TotalNumOfGrassTypes = 3;

class Game : public GameCore
{
protected:
    ImGuiManager* m_pImGuiManager;

    unsigned int m_WindowWidth;
    unsigned int m_WindowHeight;

    std::queue<Event*> m_EventQueue;

    ResourceManager* m_pResources;
    SceneManager* m_pSceneManager;
	TextRenderer* m_pTextRenderer;

    PlayerController m_Controllers[4];

    SoundPlayer* m_pSoundPlayer;
    float m_CurrentSelectedVolume;
    float m_VolumeToggle;

public:
    std::vector<TweenObject*> m_TweenPool;

public:
    Game(Framework* pFramework);
    virtual ~Game();

    virtual void OnSurfaceChanged(unsigned int width, unsigned int height);
    virtual void LoadContent();
    virtual void LoadFromFile(const char* filename);
    virtual void SaveFile();

    virtual void QueueEvent(Event* pEvent);
    virtual void ProcessEvents();
    virtual void OnEvent(Event* pEvent);

    virtual void Update(float deltatime);
    virtual void Draw();

    unsigned int GetWindowWidth() { return m_WindowWidth; }
    unsigned int GetWindowHeight() { return m_WindowHeight; }

    PlayerController* GetController(int index) { return &m_Controllers[index]; }
    SceneManager* GetSceneManager() { return m_pSceneManager; }

    bool LoadObj(const char* filename, std::vector<vec3>& vertices_out, std::vector<vec2>& uvs_out, std::vector<vec3>& normals_out);
    void BatchLoadMeshes(std::string pathhead, std::string namehead, int numoftypes, std::vector<vec3>& vertices_out, std::vector<vec2>& uvs_out, std::vector<vec3>& normals_out);

    SoundPlayer* GetSoundPlayer() {return m_pSoundPlayer;}
};



#endif //__Game_H__

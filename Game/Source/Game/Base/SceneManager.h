#pragma once

class SceneManager;
class Scene;

class SceneManager
{
protected:
   
    std::map<std::string, Scene*> m_pScenes;
	std::vector<Scene*> m_pActiveScenes;
 
public:
    SceneManager();
    virtual ~SceneManager();

    virtual void OnSurfaceChanged(unsigned int width, unsigned int height);

    void AddScene(const char* name, Scene* pScene) { m_pScenes[name] = pScene; }

    Scene* GetScene(const char* name) { return m_pScenes[name]; }
    Scene* GetScene(std::string name) { return m_pScenes[name]; }

	std::vector<Scene*> GetActiveScenes() { return m_pActiveScenes; }

	void PushScene(const char* name);
	void PushScene(std::string name);
	void Pop();
	void PopAllScenes();

	void OnEvent(Event* pEvent);
	void Update(float deltatime);
	void Draw();

};


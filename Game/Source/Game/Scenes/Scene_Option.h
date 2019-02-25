#pragma once

class Scene_Option : public Scene
{
protected:
    enum OptionItem { Begin = -1, Volume, Brightness, End };
    int m_CurrentItemIndex;
    bool m_CanMove;
    std::vector<GameObject*> m_MenuItems;
	
public:
    Scene_Option(Game* pGame, ResourceManager* pResources, TextRenderer* pTextRenderer);
	virtual ~Scene_Option();
	virtual void LoadContent() override;
	virtual bool OnEvent(Event* pEvent) override;
	virtual void Update(float deltatime) override;
	virtual void Draw() override;
    
    //float GetVolumeTogPos() { return m_VolumeTogglePos; }
    //float GetBrightnessTogPos() { return m_BrightnessTogglePos; }
    float m_VolumeTogglePos;
    float m_BrightnessTogglePos;

    void HighlightSelected();
};
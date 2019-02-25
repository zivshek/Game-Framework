#pragma once

class Scene_Level : public Scene
{
protected:
    enum OptionItem { Begin = -1, Level1, Level2, End };
    std::vector<GameObject*> m_LevelItem;
    int m_CurrentItemIndex;
    bool m_CanMove;
    bool m_HavePlayed;
public:
    Scene_Level(Game* pGame, ResourceManager* pResources, TextRenderer* pTextRenderer);
	virtual ~Scene_Level();
	virtual void LoadContent() override;
	virtual bool OnEvent(Event* pEvent) override;
	virtual void Update(float deltatime) override;
	virtual void Draw() override;
    void HighlightSelected();
};
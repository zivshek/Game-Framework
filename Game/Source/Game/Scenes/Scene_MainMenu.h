#pragma once

class Scene_MainMenu : public Scene
{
protected:

    enum MenuItem { begin = -1, level , option, quit, end };
    std::vector<GameObject*> m_MenuItems;
    int m_CurrentItemIndex;
    bool m_CanMove;
    TweenObject* m_Tweens[3];
    float xpos[3];

public:
	Scene_MainMenu(Game* pGame, ResourceManager* pResources, TextRenderer* pTextRenderer);
	virtual ~Scene_MainMenu();
	virtual void LoadContent() override;
	virtual bool OnEvent(Event* pEvent) override;
	virtual void Update(float deltatime) override;
	virtual void Draw() override;
    void HighlightSelected();
};
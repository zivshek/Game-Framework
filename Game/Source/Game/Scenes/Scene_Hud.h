#pragma once

class Scene_Hud : public Scene
{
protected:
    int m_Hour;
    int m_Hunger;

public:
    Scene_Hud(Game* pGame, ResourceManager* pResources, TextRenderer* pTextRenderer);
	virtual ~Scene_Hud();
	virtual void LoadContent() override;
	virtual bool OnEvent(Event* pEvent) override;
	virtual void Update(float deltatime) override;
	virtual void Draw() override;
    virtual void DrawTexts() override;
};
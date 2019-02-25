#pragma once

class Scene_Dead : public Scene
{
protected:


public:
    Scene_Dead(Game* pGame, ResourceManager* pResources, TextRenderer* pTextRenderer);
	virtual ~Scene_Dead();
	virtual void LoadContent() override;
	virtual bool OnEvent(Event* pEvent) override;
	virtual void Update(float deltatime) override;
	virtual void Draw() override;
    virtual void DrawTexts() override;
};
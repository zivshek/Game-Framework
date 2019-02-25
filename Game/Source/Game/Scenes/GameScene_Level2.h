#pragma once

class Game;

class GameScene_Level2 : public Scene
{
protected:
    bool m_ShowImgui;
public:
    GameScene_Level2(Game* pGame, ResourceManager* pResources, TextRenderer* pTextRenderer);
    virtual ~GameScene_Level2();

	virtual bool OnEvent(Event* pEvent);
    virtual void LoadContent();

    virtual void Update(float deltatime);
    virtual void Draw();
};


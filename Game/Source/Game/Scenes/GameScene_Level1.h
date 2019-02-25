#ifndef __GameScene_Level1_H__
#define __GameScene_Level1_H__

class Game;

class GameScene_Level1 : public Scene
{
protected:
    bool m_ShowImgui;

public:
	GameScene_Level1(Game* pGame, ResourceManager* pResources, TextRenderer* pTextRenderer);
    virtual ~GameScene_Level1();

	virtual bool OnEvent(Event* pEvent);
    virtual void LoadContent();

    virtual void Update(float deltatime);
    virtual void Draw();
};

#endif //__GameScene_Level1_H__

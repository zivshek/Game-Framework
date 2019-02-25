#pragma once

class Scene_Inventory: public Scene
{
protected:

    enum MenuItem { begin = -1, Spot1, Spot2, Spot3, Spot4, Spot5, Spot6, end };
    bool m_CanMove;
    int SpotCount;

    GameObject* m_Apple;
    GameObject* m_Mushroom;

public:
    Scene_Inventory(Game* pGame, ResourceManager* pResources, TextRenderer* pTextRenderer);
	virtual ~Scene_Inventory();
	virtual void LoadContent() override;
	virtual bool OnEvent(Event* pEvent) override;
	virtual void Update(float deltatime) override;
	virtual void Draw() override;

    void PutInInventory(std::string name);
};
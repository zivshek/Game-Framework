#ifndef __PlayerObject_H__
#define __PlayerObject_H__

std::string InventoryItems[] =
{
	"Apple", "Poisonous Mushroom"
};

struct InventoryItem
{
	std::string name = "Not Set";
	int amount = 1;
    int hpRefill = 0;
    int hungerRefrill = 0;
    bool isFood = false;
    bool isPoisonous = false;
};

class Interactable;
class Pickup;

class PlayerObject : public GameObject
{
protected:
    
    PlayerController* m_pController;
    CameraObject* m_pFirstPersonCamera;
    Light* m_pFlashLight;
    vec2 m_MouseLastPosition;
    const float m_Speed = 8;
    int m_Hunger;
    const int m_MaxHunger = 100;
    bool m_Dead;
	float m_hungerTimer = 2.0f;
	float m_survingTimer = 3.0f;

public:
    float m_Survived;
	bool m_Poisoned;
    bool m_MouseLock;
    const float m_Reach = 5;

public:
    Interactable* m_CurrentIteractable;
	std::map<std::string, InventoryItem> m_Inventory;

public:
    PlayerObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial);
    PlayerObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, std::string meshname, std::string materialname);
    virtual ~PlayerObject();

    void SetController(PlayerController* pController);
    void SetCamera(CameraObject* pCamera);
    void SetFlashLight(Light* pLight) { m_pFlashLight = pLight; }
    Light* GetFlashLight()const { return m_pFlashLight; }

    virtual void Update(float deltatime);

	void AddToInventory(InventoryItem item);
	void RemoveFromInventory(Pickup* pPickup);

    void Interact();

    int GetHunger() { return m_Hunger; }
    int GetMaxHunger() { return m_MaxHunger; }
};

#endif //__PlayerObject_H__

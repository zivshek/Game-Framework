#pragma once

class Pickup_Mushroom : public Pickup
{
public:
    Pickup_Mushroom(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial);
    Pickup_Mushroom(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, std::string meshname, std::string materialname);
    virtual ~Pickup_Mushroom();

    virtual void AddToInventory(PlayerObject* pPlayer) override;
private:
    bool m_Poisonous;
    int m_HungerRefill;
};

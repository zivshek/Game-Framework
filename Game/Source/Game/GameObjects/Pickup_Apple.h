#pragma once

class Pickup_Apple : public Pickup
{
public:
    Pickup_Apple(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial);
    Pickup_Apple(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, std::string meshname, std::string materialname);
    virtual ~Pickup_Apple();

    virtual void AddToInventory(PlayerObject* pPlayer) override;

private:
    bool m_Poisonous;
    int m_HungerRefill;
};
#include "GamePCH.h"

Pickup_Apple::Pickup_Apple(Scene * pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh * pMesh, Material * pMaterial)
    : Pickup(pScene, name, pos, rot, scale, pMesh, pMaterial)
{
    m_PickupType = Pickup_Food;
    m_Poisonous = false;
    m_HungerRefill = 5;
}

Pickup_Apple::Pickup_Apple(Scene * pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, std::string meshname, std::string materialname)
    : Pickup(pScene, name, pos, rot, scale, meshname, materialname)
{
    m_PickupType = Pickup_Food;
    m_Poisonous = false;
    m_HungerRefill = 5;
}

Pickup_Apple::~Pickup_Apple()
{
}

void Pickup_Apple::AddToInventory(PlayerObject * pPlayer)
{
    InventoryItem item;
    item.name = "Apple";
    item.hpRefill = 0;
    item.hungerRefrill = 20;
    item.isFood = true;
    pPlayer->AddToInventory(item);
    pPlayer->m_CurrentIteractable = nullptr;
    Destroy();
}

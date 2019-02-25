#include "GamePCH.h"

Pickup::Pickup(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial)
 : Interactable(pScene, name, pos, rot, scale, pMesh, pMaterial)
{
    m_InterType = InteractableType::InterType_Pickup;
    m_PickupType = PickupType::Pickup_NotSet;
}

Pickup::Pickup(Scene * pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, std::string meshname, std::string materialname)
    : Interactable(pScene, name, pos, rot, scale, meshname, materialname)
{
    m_InterType = InteractableType::InterType_Pickup;
    m_PickupType = PickupType::Pickup_NotSet;
}

Pickup::~Pickup()
{

}

void Pickup::Interact(PlayerObject* pPlayer)
{
	AddToInventory(pPlayer);
}

void Pickup::AddToInventory(PlayerObject* pPlayer)
{
}


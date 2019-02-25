#pragma once

enum PickupType
{
	Pickup_Food, Pickup_Resource, Pickup_NotSet
};

class PlayerObject;

class Pickup : public Interactable
{
protected:
	PickupType m_PickupType;
public:
	Pickup(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial);
    Pickup(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, std::string meshname, std::string materialname);
	virtual ~Pickup();

	virtual void Interact(PlayerObject* pPlayer);
	virtual void AddToInventory(PlayerObject* pPlayer);
	PickupType GetPickupType() const { return m_PickupType; }
};
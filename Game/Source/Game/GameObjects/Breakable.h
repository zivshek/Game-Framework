#pragma once

static int SpawnedPickupPostfix = 0;

class Breakable : public Interactable
{
public:
	Breakable(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial);
    Breakable(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, std::string meshname, std::string materialname);
	virtual ~Breakable();

    virtual void Interact(PlayerObject* pPlayer);
protected:
	virtual void Spawn();
	void Break();
	void OnBroken();

protected:
	int m_SpawnAmount;
	int m_Resistence;
};

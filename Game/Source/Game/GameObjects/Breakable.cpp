#include "GamePCH.h"

Breakable::Breakable(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial)
	: Interactable(pScene, name, pos, rot, scale, pMesh, pMaterial)
	, m_SpawnAmount(0)
	, m_Resistence(3)
{
    m_InterType = InteractableType::InterType_Breakable;
}

Breakable::Breakable(Scene * pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, std::string meshname, std::string materialname)
    : Interactable(pScene, name, pos, rot, scale, meshname, materialname)
    , m_SpawnAmount(0)
    , m_Resistence(3)
{
    m_InterType = InteractableType::InterType_Breakable;
}

Breakable::~Breakable()
{

}

void Breakable::Interact(PlayerObject * pPlayer)
{
    Break();
}

void Breakable::Spawn()
{
}

void Breakable::Break()
{
	m_Resistence--;

	if (m_Resistence == 0)
	{
		OnBroken();
	}
}

void Breakable::OnBroken()
{
	Spawn();
	Destroy();
}

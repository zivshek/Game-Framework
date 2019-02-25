#include "GamePCH.h"

Interactable::Interactable(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial)
	: GameObject(pScene, name, pos, rot, scale, pMesh, pMaterial)
{
    m_InterType = InterType_Notset;
}

Interactable::Interactable(Scene * pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, std::string meshname, std::string materialname)
    : GameObject(pScene, name, pos, rot, scale, meshname, materialname)
{
    m_InterType = InterType_Notset;
}

Interactable::~Interactable()
{

}


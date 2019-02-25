#pragma once

// Interactable stuff
enum InteractableType { InterType_Breakable, InterType_Pickup, InterType_Notset };

class Interactable : public GameObject
{

public:
	Interactable(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial);
    Interactable(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, std::string meshname, std::string materialname);
	virtual ~Interactable();

    virtual void Interact(PlayerObject* pPlayer) {};

    InteractableType m_InterType;
};
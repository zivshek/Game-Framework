#include "GamePCH.h"

PlayerObject::PlayerObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial)
: GameObject( pScene, name, pos, rot, scale, pMesh, pMaterial )
{
    m_pController = 0;
    m_Poisoned = false; 
    m_MouseLock = true;
    m_MouseLastPosition = 0;
    m_Hunger = 0;
    m_CurrentIteractable = nullptr;
    m_pFirstPersonCamera = nullptr;
    m_Survived = 0;
    m_Dead = false;
}

PlayerObject::PlayerObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, std::string meshname, std::string materialname)
: GameObject( pScene, name, pos, rot, scale, meshname, materialname )
{
    m_pController = 0;
    m_Poisoned = false;
    m_MouseLock = true;
    m_MouseLastPosition = 0;
    m_Hunger = m_MaxHunger;
    m_CurrentIteractable = nullptr;
    m_pFirstPersonCamera = nullptr;
    m_Survived = 0;
    m_Dead = false;
}

PlayerObject::~PlayerObject()
{
}

void PlayerObject::SetController(PlayerController* pController)
{
    m_pController = pController;
}

void PlayerObject::SetCamera(CameraObject* pCamera)
{
    m_pFirstPersonCamera = pCamera;
}

void PlayerObject::Update(float deltatime)
{
    GameObject::Update( deltatime );
    if( m_pController == 0 )
        return;

    if (m_Hunger > 0)
    {
		m_hungerTimer -= deltatime;
		m_survingTimer -= deltatime;
		
		if (m_hungerTimer <= 0) {
			m_Hunger -= 1;
			m_hungerTimer = 2.0f;
		}

		if (m_survingTimer <= 0) {
			m_Survived += 1;
			m_survingTimer = 3.0f;
		}
    }

    if (m_Hunger <= 0 && !m_Dead)
    {
        //m_pScene->GetGame()->GetSceneManager()->PopAllScenes();
        //m_pScene->GetGame()->GetSceneManager()->PushScene("DeadScene");
        m_Dead = true;
    }

    // Calculate the direction we're facing but zero out y, because we don't want to move up
    vec3 forwardDir = m_EAngle.ToVector();
    forwardDir.y = 0;
    forwardDir.Normalize();

    // Just an up vector
    vec3 upDir = vec3(0, 1, 0);

    // Calculate the right/left direction by cross product of the above two vectors
    vec3 rightDir = upDir.Cross(forwardDir);

    vec3 movementGoal = 0;

    if (m_pController->IsButtonPressed( PCB_Forward ))
        movementGoal.x += m_Speed;
    if( m_pController->IsButtonPressed( PCB_Backward ) )
        movementGoal.x -= m_Speed;
    if( m_pController->IsButtonPressed( PCB_Left ) )
        movementGoal.z -= m_Speed;
    if( m_pController->IsButtonPressed( PCB_Right ) )
        movementGoal.z += m_Speed;

    if (m_pController->IsButtonPressed( PCB_Interact ))
    {
        Interact();
        m_pController->SetButtonReleased(PCB_Interact);
    }

	int mid_x = 400;
	int mid_y = 300;

    if (m_MouseLock)
    {
	    vec2 mousepos = m_pController->GetMousePosition();
	    mousepos.y = 600 - mousepos.y;
	    if ((mousepos.x != mid_x) || (mousepos.y != mid_y))
	    {
		    // Update the camera direction (looking at) according to the direction we are facing
		    m_EAngle.p -= (mid_y - mousepos.y) / 500.0f;
		    m_EAngle.y += (mid_x - mousepos.x) / 500.0f;
		    m_EAngle.normalize();

            if (m_pFirstPersonCamera)
            {
                m_pFirstPersonCamera->m_LookingAt = (m_Position + vec3(0, 0.5f, 0) + m_EAngle.ToVector().Normalize() * 50);
            }
	    }
    
        // Set mouse back to screen center
        m_pScene->GetGame()->GetFramework()->SetCursorPosition(mid_x, mid_y);
    }
	// Constant speed
    vec3 targetVelocity = forwardDir * movementGoal.x + rightDir * movementGoal.z;
	btVector3 targetVelocitybtVec3 = btVector3(targetVelocity.x, targetVelocity.y, targetVelocity.z);
	btVector3 currentVelocitybtVec3 = m_pBody->getLinearVelocity();
	btVector3 velocityDifference = targetVelocitybtVec3 - currentVelocitybtVec3;
    velocityDifference.setY(0);

    m_pBody->applyCentralImpulse ((1 / m_pBody->getInvMass()) * velocityDifference);
    
    // Ray test
    if (m_pFirstPersonCamera)
    {
        // Update camera position
        m_pFirstPersonCamera->SetPosition(m_Position + vec3(0, 0.5f, 0));

        vec3 camDir = m_pFirstPersonCamera->m_LookingAt - m_pFirstPersonCamera->GetPosition();
        vec3 camPos = m_pFirstPersonCamera->GetPosition();
        btVector3 btFrom(camPos.x, camPos.y, camPos.z);
        btVector3 btTo = btFrom + btVector3(camDir.x, camDir.y, camDir.z) * m_Reach;
        btCollisionWorld::ClosestRayResultCallback res(btFrom, btTo);
        m_pScene->GetBulletWorld()->GetDynamicsWorld()->rayTest(btFrom, btTo, res);
        if (res.hasHit())
        {
            Interactable* temp = dynamic_cast<Interactable*>((GameObject*)res.m_collisionObject->getUserPointer());
            if (temp)
                m_CurrentIteractable = temp;
            else
                m_CurrentIteractable = nullptr;
        }
        else
            m_CurrentIteractable = nullptr;
    }

    // Update flash light position and direction
    m_pFlashLight->position = m_Position + vec3(0, 0.5f, 0);
    m_pFlashLight->coneDirection = m_EAngle.ToVector().Normalize();
}

void PlayerObject::AddToInventory(InventoryItem item)
{
    auto itr = m_Inventory.find(item.name);
    if (itr != m_Inventory.end())
    {
        m_Inventory[item.name].amount++;
    }
    else
    {
        m_Inventory[item.name] = item;
        static_cast<Scene_Inventory*>(m_pScene->GetGame()->GetSceneManager()->GetScene("InventoryScene"))->PutInInventory(item.name);
    }
}

void PlayerObject::RemoveFromInventory(Pickup* pPickup)
{
//
}

void PlayerObject::Interact()
{
    if (m_CurrentIteractable)
        m_CurrentIteractable->Interact(this);
}

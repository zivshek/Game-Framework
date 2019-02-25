#include "GamePCH.h"

CameraObject::CameraObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale)
: GameObject( pScene, name, pos, rot, scale, 0, 0 )
{
    m_LookingAt = vec3(0,0,0);
    m_UseOrthoProj = false;
}

CameraObject::~CameraObject()
{
}

void CameraObject::Update(float deltatime)
{
    //m_MatView.SetIdentity();  
    m_MatView.CreateLookAtViewLeftHanded(m_Position,vec3(0,1,0), m_LookingAt);

    if(m_UseOrthoProj)
        m_MatProj.CreateOrtho(-5, 5, -5, 5, -5, 5);
    else
        m_MatProj.CreatePerspectiveVFoV( 45, 800.0f/600.0f, 0.01f, 500 );

    GameObject::Update( deltatime );
}

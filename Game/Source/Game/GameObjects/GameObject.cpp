#include "GamePCH.h"

GameObject::GameObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial)
{
    Init( pScene, name, pos, rot, scale, pMesh, pMaterial );
}

GameObject::GameObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, std::string meshname, std::string materialname)
{
    ResourceManager* pResources = pScene->GetResourceManager();
    Init( pScene, name, pos, rot, scale, pResources->GetMesh( meshname ), pResources->GetMaterial( materialname ) );
}

GameObject::~GameObject()
{
    if( m_pBody )
    {
        m_pScene->GetBulletWorld()->GetDynamicsWorld()->removeRigidBody( m_pBody );
        delete m_pBody;
    }
}

void GameObject::Init(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial)
{
    m_pScene = pScene;

    m_RenderOrder = 0;
    m_IsHighlighted = false;
    m_Name = name;

	m_InitialPosition = m_Position = pos;
    m_InitialRotation = m_Rotation = rot;
    m_Scale = scale;

    m_pMesh = pMesh;
    m_pMaterial = pMaterial;

    m_pBody = 0;
}

void GameObject::Update(float deltatime)
{
    //setWorldTransform(m_pBody->getWorldTransform());
}

void GameObject::Draw(int renderorder, mat4* pMatView, mat4* pMatProj, vec3 campos)
{
    if( m_pMesh == 0 )
        return;

    SetRenderOrder(renderorder);

    ShaderProgram* pShaderProgram = m_pMaterial->GetShader();

    mat4 world;
    world.CreateSRT( m_Scale, m_Rotation, m_Position );

    m_pMesh->SetupAttributes( pShaderProgram );
    m_pMesh->SetupUniforms( m_pMaterial, &world, pMatView, pMatProj, campos );
    m_pMesh->SetupLighting(m_pMaterial, m_pScene->AllLights);
    CustomUniformSetup();
    m_pMesh->Draw( pShaderProgram );
}

void GameObject::CreateBody(float mass, const btVector3& scale, float friction, float restitution)
{
    btCollisionShape* colShape = 0;

#undef new

    colShape = new btBoxShape(scale);

    m_pScene->GetBulletWorld()->AddShape( colShape );

    colShape->setLocalScaling(scale);

    btVector3 localInertia;
    colShape->calculateLocalInertia( mass, localInertia );

    btRigidBody::btRigidBodyConstructionInfo rbInfo( mass, this, colShape, localInertia );
    rbInfo.m_friction = friction;
    rbInfo.m_restitution = restitution;

    m_pBody = new btRigidBody( rbInfo );
    m_pBody->setUserPointer(this);

    m_pScene->GetBulletWorld()->GetDynamicsWorld()->addRigidBody( m_pBody );
}

void GameObject::getWorldTransform(btTransform & worldTrans) const
{
    worldTrans.setOrigin(btVector3(m_Position.x, m_Position.y, m_Position.z));
    worldTrans.setRotation(btQuaternion(m_Rotation.y / 180.0f * PI, m_Rotation.x / 180.0f * PI, m_Rotation.z / 180.0f * PI));
}

void GameObject::setWorldTransform(const btTransform & worldTrans)
{
    mat4 worldmat;
    worldTrans.getOpenGLMatrix(&worldmat.m11);
    m_Position = worldmat.GetTranslation();
    m_Rotation = worldmat.GetEulerAngles() / PI * 180.0f;
}

void GameObject::AddToScene(Scene* pScene)
{
	m_pScene = pScene;
	m_pScene->AddToScene(m_Name, this);
}

void GameObject::Destroy()
{
	if (m_pScene)
	{
		GameEvent_Destroy* pDestroyEvent = new GameEvent_Destroy(this);
		m_pScene->GetGame()->QueueEvent(pDestroyEvent);
	}
}

void GameObject::Reset()
{
	m_Position = m_InitialPosition;
	m_Rotation = m_InitialRotation;

	if (m_pBody)
	{
		m_pBody->setLinearVelocity(btVector3(0, 0, 0));
		m_pBody->setAngularVelocity(btVector3(0, 0, 0));
		// Not sure if we need to set the transform manually
		// since the getWorldTransform and setWorldTransform
		// should be called automatically already
	}
}

void GameObject::RemoveFromScene()
{
	if (m_pScene)
	{
		m_pScene->Destroy(this);
		m_pScene = 0;
	}
}

void GameObject::CustomUniformSetup()
{
    ShaderProgram* program = m_pMaterial->GetShader();

    GLint uAddColor = glGetUniformLocation(program->GetProgram(), "u_AddColor");

    if (uAddColor != -1)
    {
        if (m_IsHighlighted)
            glUniform4f(uAddColor, 1, -1, -1, 0);
        else
            glUniform4f(uAddColor, 0, 0, 0, 0);
    }
}

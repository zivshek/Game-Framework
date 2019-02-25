#ifndef __GameObject_H__
#define __GameObject_H__

class ShaderProgram;
class Mesh;
class Scene;


class GameObject : public btMotionState
{
protected:
    Scene* m_pScene;

    unsigned int m_RenderOrder;

    std::string m_Name;

	vec3 m_InitialPosition;
	vec3 m_InitialRotation;
    vec3 m_Position;
    vec3 m_Rotation;
    vec3 m_Scale;
    EulerAngle m_EAngle;

    Mesh* m_pMesh;
    Material* m_pMaterial;

    btRigidBody* m_pBody;
public:
    bool m_IsHighlighted;

public:
    GameObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial);
    GameObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, std::string meshname, std::string materialname);
    virtual ~GameObject();

    unsigned int GetRenderOrder() const { return m_RenderOrder; } 
    void SetRenderOrder(unsigned int order) { m_RenderOrder = order; }

    virtual void Init(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial);

    virtual void Update(float deltatime);
    virtual void Draw(int renderorder, mat4* pMatView, mat4* pMatProj, vec3 campos);

    void SetMesh(Mesh* pMesh) { m_pMesh = pMesh; }
    void SetMaterial(Material* pMaterial) { m_pMaterial = pMaterial; }

    Mesh* GetMesh()			const { return m_pMesh; } 
    Material* GetMaterial() const { return m_pMaterial; } 

    void SetPosition(vec3 pos) { m_Position = pos;  }
    void SetRotation(vec3 rot) { m_Rotation = rot;  }
    void SetScale(vec3 scale)  { m_Scale    = scale;}
	void SetScene(Scene* pScene) { m_pScene = pScene; }

    vec3 GetPosition()		const { return m_Position; }
    vec3 GetRotation()		const { return m_Rotation; }
    vec3 GetScale()			const { return m_Scale;	   }
	std::string GetName()   const { return m_Name; }
    btRigidBody* GetBody()  const { return m_pBody;	   }

    void CreateBody(float mass, const btVector3& scale, float friction = 0.5f, float restitution = 0);

    // Inherited via btMotionState
    virtual void getWorldTransform(btTransform & worldTrans) const override;
    virtual void setWorldTransform(const btTransform & worldTrans) override;

	void AddToScene(Scene* pScene);
	void Destroy();
	virtual void Reset();
	virtual void RemoveFromScene();

    virtual void CustomUniformSetup();
};

#endif //__GameObject_H__

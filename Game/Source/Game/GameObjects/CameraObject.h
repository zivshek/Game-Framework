#ifndef __CameraObject_H__
#define __CameraObject_H__

class CameraObject : public GameObject
{
protected:
    mat4 m_MatView;
    mat4 m_MatProj;
    bool m_UseOrthoProj;
	
public:
    vec3 m_LookingAt;

public:
    CameraObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale);
    virtual ~CameraObject();

    virtual void Update(float deltatime);

    mat4* GetViewMatrix() { return &m_MatView; }
    mat4* GetProjMatrix() { return &m_MatProj; }

    void SetUseOrthoProj(bool use) { m_UseOrthoProj = use; }

};



#endif //__CameraObject_H__

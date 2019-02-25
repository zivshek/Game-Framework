#ifndef __BulletWorld_H__
#define __BulletWorld_H__

class BulletWorld;
class Material;

class BulletWorld
{
protected:
    btBroadphaseInterface* m_pBroadphase;
    btDefaultCollisionConfiguration* m_pCollisionConfiguration;
    btCollisionDispatcher* m_pDispatcher;
    btSequentialImpulseConstraintSolver* m_pSolver;
    btDiscreteDynamicsWorld* m_pDynamicsWorld;

    // Keep track of the shapes, we release memory at exit.
    // Make sure to re-use collision shapes among rigid bodies whenever possible!
    btAlignedObjectArray<btCollisionShape*> m_CollisionShapes;

public:
    BulletWorld();
    ~BulletWorld();

    void CreateWorld();
    void Update(float deltatime);
    void Cleanup();

    btDiscreteDynamicsWorld* GetDynamicsWorld() { return m_pDynamicsWorld; }
    void AddShape(btCollisionShape* pShape) { m_CollisionShapes.push_back( pShape ); }
    btCollisionShape* GetShape(int index) { if( index <= m_CollisionShapes.size() ) return 0; return m_CollisionShapes[index]; }
};

#endif //__BulletWorld_H__

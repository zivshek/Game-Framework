#include "GamePCH.h"
#include "BulletWorld.h"

#undef new

BulletWorld::BulletWorld()
{
    m_pBroadphase = 0;
    m_pCollisionConfiguration = 0;
    m_pDispatcher = 0;
    m_pSolver = 0;
    m_pDynamicsWorld = 0;

    CreateWorld();
}

BulletWorld::~BulletWorld()
{
    Cleanup();
}

void BulletWorld::CreateWorld()
{
    assert( m_pDynamicsWorld == 0 );

    // btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    m_pBroadphase = new btDbvtBroadphase();

    // Collision configuration contains default setup for memory, collision setup.
    //   Advanced users can create their own configuration.
    m_pCollisionConfiguration = new btDefaultCollisionConfiguration();

    // Use the default collision m_pDispatcher.
    //   For parallel processing you can use a diffent m_pDispatcher (see Extras/BulletMultiThreaded).
    m_pDispatcher = new btCollisionDispatcher( m_pCollisionConfiguration );

    // The default constraint m_pSolver.
    //   For parallel processing you can use a different m_pSolver (see Extras/BulletMultiThreaded).
    m_pSolver = new btSequentialImpulseConstraintSolver();

    m_pDynamicsWorld = new btDiscreteDynamicsWorld( m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfiguration );

    m_pDynamicsWorld->setGravity( btVector3(0,-9.8f,0) );
}

void BulletWorld::Update(float deltatime)
{
    // Update physics based on how much time passed since last frame.
    m_pDynamicsWorld->stepSimulation( deltatime, 0 );
}

void BulletWorld::Cleanup()
{
    // Remove the rigidbodies from the dynamics world and delete them.
    for( int i=m_pDynamicsWorld->getNumCollisionObjects()-1; i>=0; i-- )
    {
        btCollisionObject* obj = m_pDynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast( obj );
        if( body && body->getMotionState() )
        {
            delete body->getMotionState();
        }

        m_pDynamicsWorld->removeCollisionObject( obj );
        delete obj;
    }

    // Delete collision shapes.
    for( int i=0; i<m_CollisionShapes.size(); i++ )
    {
        btCollisionShape* shape = m_CollisionShapes[i];
        m_CollisionShapes[i] = 0;
        delete shape;
    }
    m_CollisionShapes.clear();

    delete m_pDynamicsWorld;
    delete m_pSolver;
    delete m_pDispatcher;
    delete m_pCollisionConfiguration;
    delete m_pBroadphase;

    m_pDynamicsWorld = 0;
    m_pSolver = 0;
    m_pDispatcher = 0;
    m_pCollisionConfiguration = 0;
    m_pBroadphase = 0;
}

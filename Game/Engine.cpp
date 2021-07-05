#include "Engine.h"

using namespace glm;
void Engine::init()
{
    camera = new Camera();

    ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    collisionConfiguration = new btDefaultCollisionConfiguration();
    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    overlappingPairCache = new btDbvtBroadphase();
    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    solver = new btSequentialImpulseConstraintSolver;
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
    //keep track of the shapes, we release memory at exit.
    //make sure to re-use collision shapes among rigid bodies whenever possible!
    btAlignedObjectArray<btCollisionShape*> collisionShapes;
}

void Engine::display()
{
    // Call before displaying individual elements
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // update Camera perspective
    gluLookAt(camera->position.x, camera->position.y, camera->position.z,
        camera->viewDirection.x + camera->position.x,
        camera->viewDirection.y + camera->position.y,
        camera->viewDirection.z + camera->position.z,
        camera->UP.x, camera->UP.y, camera->UP.z);
}

void Engine::update()
{
    //update deltatime
    elapsed = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = elapsed - prev_elapsed;
    prev_elapsed = elapsed;
    // Update physics objects
    dynamicsWorld->stepSimulation(1.f / 30.f);



    ////print positions of all objects
    //for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
    //{
    //    btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
    //    btRigidBody* body = btRigidBody::upcast(obj);
    //    btTransform trans;
    //    if (body && body->getMotionState())
    //    {
    //        body->getMotionState()->getWorldTransform(trans);
    //    }
    //    else
    //    {
    //        trans = obj->getWorldTransform();
    //    }
    //    printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
    //}
}

void Engine::render()
{
}


void Engine::reshape(GLubyte* texture)
{
    glTexImage2D(GL_TEXTURE_2D,
        0,                    // level 0
        3,                    // use only R, G, and B components
        2, 2,                 // texture has 2x2 texels
        0,                    // no border
        GL_RGB,               // texels are in RGB format
        GL_UNSIGNED_BYTE,     // color components are unsigned bytes
        texture);
}

btRigidBody* Engine::createSphere(float sx, float px, float py, float pz, float mass) // Create a sphere with dynamic physics
{
    btCollisionShape* colShape = new btSphereShape(btScalar(sx));
    collisionShapes.push_back(colShape);

    btTransform startTransform;
    startTransform.setIdentity();

    btScalar    tMass(mass);

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (tMass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        colShape->calculateLocalInertia(tMass, localInertia);

    startTransform.setOrigin(btVector3(px, py, pz));
    btDefaultMotionState* motionState = new btDefaultMotionState(startTransform);

    btRigidBody::btRigidBodyConstructionInfo rbInfo(tMass, motionState, colShape, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);


    body->setWorldTransform(startTransform);
    dynamicsWorld->addRigidBody(body);
    return body;
}
//https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ
btRigidBody* Engine::createGround(btVector3 pos, btVector3 shape)
{
    btCollisionShape* groundShape = new btBoxShape(shape);
    Engine::Instance()->collisionShapes.push_back(groundShape);
    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(pos);

    btScalar mass(0.);

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        groundShape->calculateLocalInertia(mass, localInertia);

    //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* motionState = new btDefaultMotionState(groundTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, groundShape, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

    //add the body to the dynamics world
    dynamicsWorld->addRigidBody(body);
    return body;
}

void Engine::quit()
{
    //cleanup in the reverse order of creation/initialization
    int i;
    //remove the rigidbodies from the dynamics world and delete them
    for (i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
    {
        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState())
        {
            delete body->getMotionState();
        }
        dynamicsWorld->removeCollisionObject(obj);
        delete obj;
    }

    //delete collision shapes
    for (int j = 0; j < collisionShapes.size(); j++)
    {
        btCollisionShape* shape = collisionShapes[j];
        collisionShapes[j] = 0;
        delete shape;
    }

    //delete dynamics world
    delete dynamicsWorld;

    //delete solver
    delete solver;

    //delete broadphase
    delete overlappingPairCache;

    //delete dispatcher
    delete dispatcher;

    delete collisionConfiguration;

    //next line is optional: it will be cleared by the destructor when the array goes out of scope
    collisionShapes.clear();

}

Engine* Engine::Instance()
{
    static Engine* sing = new Engine();
    return sing;
}

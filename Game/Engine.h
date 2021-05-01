#pragma once
#include <bullet/btBulletDynamicsCommon.h>
#include "Camera.h"

#include <GL/glut.h>
#include <glm/gtx/transform.hpp>
class Engine
{

	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	float elapsed = 0.0f;
	float prev_elapsed = 0.0f;

public:

	float deltaTime = 0.0f;
	btDiscreteDynamicsWorld* dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	Camera* camera;
	Engine() { init(); }
	void init();
	void display();
	void update();
	void render();
	btRigidBody* createSphere(float sx, float px, float py, float pz, float mass); // Create a dynamic sphere
	btRigidBody* createGround(btVector3 pos, btVector3 shape); // Create a static rectangle
	void quit();
	btDiscreteDynamicsWorld* getWorld() { return dynamicsWorld; }
	static Engine* Instance();
};


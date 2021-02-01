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
	btDiscreteDynamicsWorld* dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

public:

	Camera* camera;
	Engine() { init(); }
	void init();
	void display();
	void update();
	void render();

	void quit();
};


#pragma once
// This application shows balls bouncing on a checkerboard, with no respect
// for the laws of Newtonian Mechanics.  There's a little spotlight to make
// the animation interesting, and arrow keys move the camera for even more
// fun.

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>
#include <bullet\LinearMath\btVector3.h>
#include <GL/glut.h>
#include <glm/gtx/transform.hpp>
#include <bullet/btBulletDynamicsCommon.h>
#include "Engine.h"

#include <iostream>

// A checkerboard class.  A checkerboard has alternating red and white
// squares.  The number of squares is set in the constructor.  Each square
// is 1 x 1.  One corner of the board is (0, 0) and the board stretches out
// along positive x and positive z.  It rests on the xz plane.  I put a
// spotlight at (4, 3, 7).
class Checkerboard {
    int displayListId;
    btTransform tform;
    int width;
    int depth;
    btRigidBody* body;
    GLfloat color[3];

    btVector3 origin;

public:
    Checkerboard(int width, int depth) : width(width), depth(depth) {

        //the ground is a cube of side 100 at position y = -56.
        btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(width), btScalar(1.), btScalar(depth)));

        Engine::Instance()->collisionShapes.push_back(groundShape);

        btTransform groundTransform;
        groundTransform.setIdentity();
        origin = btVector3(-10, -140, 1);
        groundTransform.setOrigin(origin);

        btScalar mass(0.);
        color[0] = 0.7;
        color[1] = 0.31;
        color[2] = 0.1;
        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            groundShape->calculateLocalInertia(mass, localInertia);

        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* motionState = new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, groundShape, localInertia);
        body = new btRigidBody(rbInfo);

        //add the body to the dynamics world
        Engine::Instance()->dynamicsWorld->addRigidBody(body);

    }
    double centerx() { return width / 2; }
    double centerz() { return depth / 2; }
    void create() {
        // Creates the rendering block
        // Colors
        GLfloat WHITE[] = { 1, 1, 1 };
        GLfloat RED[] = { 1, 0, 0 };
        GLfloat GREEN[] = { 0, 1, 0 };
        GLfloat MAGENTA[] = { 1, 0, 1 };

        displayListId = glGenLists(1);
        glNewList(displayListId, GL_COMPILE);

        GLfloat lightPosition[] = { 10, 3, 7, 1 };
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

        glBegin(GL_QUADS);
        update();
        //glEnable(GL_NORMALIZE);
        btVector3 loc = tform.getOrigin();

        //width = loc.z();
        //depth = loc.x();
        //glNormal3d(0,1,0);

        // Checkerboard
        //for (int x = 0; x < width - 1; x++) {
        //    for (int z = 0; z < depth - 1; z++) {

        //        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,
        //            (x + z) % 2 == 0 ? GREEN : WHITE);

        //        glVertex3d(x + loc.x() , 0 + loc.y(), z +loc.z());
        //        glVertex3d(x + 1 + loc.x(), 0 + loc.y(), z + loc.z());
        //        glVertex3d(x + 1 + loc.x(), 0 + loc.y(), z + 1 + loc.z());
        //        glVertex3d(x + loc.x(), 0 + loc.y(), z + 1 + loc.z());
        //    }
        //}
        glColor3d(0, 1, 1);
        glVertex3f(loc.getX(), loc.getY() , loc.getZ());
        glColor3d(0, 1, 1);
        glVertex3f(loc.getX() + width, loc.getY(), loc.getZ());
        glColor3d(0, 1, 1);
        glVertex3f(loc.getX() + width, loc.getY(), loc.getZ() + depth);
        glColor3d(0, 1, 1);
        glVertex3f(loc.getX(), loc.getY(), loc.getZ() + depth );

        //glDisable(GL_NORMALIZE);
        glEnd();
        glEndList();
    }
    void update();

    void draw() {
        glCallList(displayListId);
    }
    void display();
};
class Rain
{
    float maxlife = 300.0f;
    float life = 0.0f;
    btRigidBody* body;
    btTransform tform;
    GLfloat* color;
    float r = 1;

public:
    Rain(float x, float y, float z,
        GLfloat* c) : color(c) {
        body = Engine::Instance()->createSphere(r, x, y, z, .000001);

        if (body && body->getMotionState())
        {
            body->getMotionState()->getWorldTransform(tform);
        }
    }
    void update();
    ~Rain() {

        delete body->getMotionState();
        delete body->getCollisionShape();
        Engine::Instance()->getWorld()->removeRigidBody(body);
        delete body;
    }
    void display();
    float getLife() {
        return life;
    }
    const float getMaxLife() {
        return maxlife;
    }
};
class Objects
{

public:

	class Triangles
	{

	public:
        float x, y, z;

        Triangles(float x, float y, float z) : x(x), y(y), z(z) {}

        void create() {

        }

		void draw() {
            // Draw a textured triangle

            glEnable(GL_TEXTURE_2D);
            glBegin(GL_TRIANGLES);
            glTexCoord2f(0.5, 1.0);    glVertex2f(-3 - x, 3 + y);
            glTexCoord2f(0.0, 0.0);    glVertex2f(-3 - x, 0 + y);
            glTexCoord2f(1.0, 0.0);    glVertex2f(0 - x, 0 + y);

            glTexCoord2f(4, 8);        glVertex2f(3 - x, 3 + y);
            glTexCoord2f(0.0, 0.0);    glVertex2f(0 - x, 0 + y);
            glTexCoord2f(8, 0.0);      glVertex2f(3 - x, 0 + y);

            glTexCoord2f(5, 5);        glVertex2f(0 - x, 0 + y);
            glTexCoord2f(0.0, 0.0);    glVertex2f(-1.5 - x, -3 + y);
            glTexCoord2f(4, 0.0);      glVertex2f(1.5 - x, -3 + y);
            glEnd();
            glDisable(GL_TEXTURE_2D);

		}
	};


};


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

        // position vector    
        origin = btVector3(30, -110, 5);
        // shape vector
        btVector3 shape = btVector3(btScalar(width), btScalar(1.), btScalar(depth));

        // Add to Physics World
        body = Engine::Instance()->createGround(origin, shape);

        // Colours
        color[0] = 0.5;
        color[1] = 0.41;
        color[2] = 0.1;

    }
    double centerx() { return width / 2; }
    double centerz() { return depth / 2; }
    void create() {
        // Creates the rendering block
        // Modern GL creates a prerendered 'glList'
        // For static meshes
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
        btVector3 loc = tform.getOrigin();
        glColor3d(0, 1, 1);
        glVertex3f(loc.getX(), loc.getY() , loc.getZ());
        glColor3d(0, 1, 1);
        glVertex3f(loc.getX() + width, loc.getY(), loc.getZ());
        glColor3d(0, 1, 1);
        glVertex3f(loc.getX() + width, loc.getY(), loc.getZ() + depth);
        glColor3d(0, 1, 1);
        glVertex3f(loc.getX(), loc.getY(), loc.getZ() + depth );
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
    float maxlife = 500.0f;
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

class Triangles
{

public:
    float x, y, z;

    Triangles(float x, float y, float z) : x(x), y(y), z(z) {}

    void create() {

    }

	void draw() {
        // Draw a textured triangle
        //
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

// A ball
class Ball {
    double radius;
    GLfloat* color;
    int direction;
    btRigidBody* body;
    btTransform tform;

public:
    Ball(double r, GLfloat* c, double x, double y, double z) : // c Color should be 3 floats sequential.
        radius(r), color(c), direction(-1) {
        printf("ball #created at = %f,%f,%f\n", x, y, z);
        body = Engine::Instance()->createSphere(r, x, y, z, .000001);

        if (body && body->getMotionState())
        {
            body->getMotionState()->getWorldTransform(tform);
        }
    }
    void display() {
        if (body && body->getMotionState())
        {
            //tform = body->getCenterOfMassTransform();
            body->getMotionState()->getWorldTransform(tform);
        }

        btVector3 pos = tform.getOrigin();
        glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
        glTranslated(pos.getX(), pos.getY(), pos.getZ());
        glutSolidSphere(radius, 30, 30);
        glPopMatrix();
    }
    void update() {

    }
};
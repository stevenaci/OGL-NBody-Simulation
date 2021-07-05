#pragma once

#include <GL/glew.h>
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

struct vertex
{
    GLfloat x, y, z;
};

class Floor {
    int displayListId;
    btTransform tform;
    int width;
    int depth;
    btRigidBody* body;
    GLfloat color[3];
    btVector3 origin;

public:
    Floor(int width, int depth);
    double centerx();
    double centerz();
    void create();
    void update();
    void draw();
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
        GLfloat* c);

    void update();
    ~Rain() {
        delete body->getMotionState();
        delete body->getCollisionShape();
        Engine::Instance()->getWorld()->removeRigidBody(body);
        delete body;
    }
    void display();
    float getLife() {                 return life;    
    }
    const float getMaxLife() {        return maxlife;
    }
};

class Triangles
{
private:
    glm::vec3 scale;
    vertex origin;

public:
    Triangles(float x, float y , float z)
    { 
        scale = { 0.0f, 1.0f, 0.0f }; 
        origin = { x,y,z };
    

    }
    void setPosition(glm::vec3 xyz);
    void create() {}

    void draw();
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


// Textured Cube,
// binding & buffers :
// (ogl 3.1)
class Cube {
public:
    Cube() {};
    void display();
};

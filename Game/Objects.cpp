#include "Objects.h"
void Rain::update()
{

    life += 1.0f;
}

void Rain::display()
{
    if (body && body->getMotionState())
    {
        body->getMotionState()->getWorldTransform(tform);
    }
    btVector3 pos = tform.getOrigin();
    // Set up and draw a sphere
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
    glTranslated(pos.getX(), pos.getY(), pos.getZ());
    glutSolidSphere(r, 30, 30);
    glPopMatrix();

}
void Floor::display() {

    btVector3 pos = tform.getOrigin();
    // Set up and draw a rectangle GLmatrix
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
    glTranslated(pos.getX(), pos.getY(), pos.getZ());
    glRotatef(-90.0, 1, 0, 0);
    glRectf(0- width, 0- depth, width, depth); 
    glPopMatrix();
}

void Floor::update() {
    if (body && body->getMotionState())
    {
        body->getMotionState()->getWorldTransform(tform);
    }
}
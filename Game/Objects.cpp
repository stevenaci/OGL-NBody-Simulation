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
    //printf("rain life at = %f\n", life);
    //printf("rain created at = %f,%f,%f\n", float(tform.getOrigin().getX()), float(tform.getOrigin().getY()), float(tform.getOrigin().getZ()));

    btVector3 pos = tform.getOrigin();
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
    glTranslated(pos.getX(), pos.getY(), pos.getZ());
    //glRectf(0, 0, 10, 7);

    glutSolidSphere(r, 30, 30);
    glPopMatrix();

}
void Checkerboard::display() {


    btVector3 pos = tform.getOrigin();
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);

    glTranslated(pos.getX(), pos.getY(), pos.getZ());

    glRotatef(-90.0, 1, 0, 0);
    glRectf(0- width, 0- depth, width, depth);

    glPopMatrix();
}

void Checkerboard::update() {
    if (body && body->getMotionState())
    {
        body->getMotionState()->getWorldTransform(tform);
    }
    std::cout << "\n" << tform.getOrigin().getX() << " " << tform.getOrigin().getY();
}
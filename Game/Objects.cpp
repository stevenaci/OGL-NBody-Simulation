#include "Objects.h"

double Floor::centerx() { return width / 2; }
double Floor::centerz() { return depth / 2; }

Rain::Rain(float x, float y, float z, GLfloat* c) : color(c) {
    body = Engine::Instance()->createSphere(r, x, y, z, 1.23);

    if (body && body->getMotionState())
    {
        body->getMotionState()->getWorldTransform(tform);
    }
}

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


Floor::Floor(int width, int depth)
    : width(width), depth(depth) {

    // position vector    
    origin = btVector3(0, -5, 0);
    // shape vector
    //btVector3 shape = btVector3(btScalar(width), btScalar(1.), btScalar(depth));
    btVector3 shape = btVector3(btScalar(width), btScalar(1.), btScalar(depth));

    // Add to Physics World
    body = Engine::Instance()->createGround(origin, shape);

    // Colours
    color[0] = 0.5;
    color[1] = 0.41;
    color[2] = 0.1;

}

void Floor::draw()
{
    glCallList(displayListId);
}
void Floor::display() {

    btVector3 pos = tform.getOrigin();
    // Set up and draw a rectangle GLmatrix
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
    glTranslated(pos.getX(), pos.getY(), pos.getZ());
    glRotatef(-90.0, 1, 0, 0);
    glRectf(0 - width, 0 - depth, width, depth);
    glPopMatrix();
}

void Floor::create()
{
    // Creates the rendering block
    // Modern GL creates a prerendered 'glList'
    // For static meshes

    displayListId = glGenLists(1);
    glNewList(displayListId, GL_COMPILE);
    GLfloat lightPosition[] = { 10, 3, 7, 1 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glBegin(GL_QUADS);
    glVertex3f(-width, -depth, 12);
    glVertex3f(width, -depth, 12);
    glVertex3f(-width, depth, 12);
    glVertex3f(-width, -depth, 12);

    glEnd();
    glEndList();
}

void Floor::update() {
    if (body && body->getMotionState())
    {
        body->getMotionState()->getWorldTransform(tform);
    }
}

void Cube::display()
{


}

void Triangles::setPosition(glm::vec3 xyz)
{

}

glm::vec3 rotationaxis(0.0f, 1.0f, 0.0f);
void Triangles::draw()
{
    // Draw a textured triangle
    // 
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_TRIANGLES);

    glTexCoord2f(0.5, 1.0);    glVertex2f(-3, 3);
    glTexCoord2f(0.0, 0.0);    glVertex2f(-3, 0);
    glTexCoord2f(1.0, 0.0);    glVertex2f(0 , 0);

    glTexCoord2f(4, 8);        glVertex2f(3, 3);
    glTexCoord2f(0.0, 0.0);    glVertex2f(0, 0 );
    glTexCoord2f(8, 0.0);      glVertex2f(3 , 0 );

    glTexCoord2f(5, 5);        glVertex2f(0 , 0 );
    glTexCoord2f(0.0, 0.0);    glVertex2f(-1.5 , -3 );
    glTexCoord2f(4, 0.0);      glVertex2f(1.5, -3 );
    glEnd();
    glDisable(GL_TEXTURE_2D);

}

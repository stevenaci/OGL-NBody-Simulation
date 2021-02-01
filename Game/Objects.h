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


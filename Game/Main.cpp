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
#include "Objects.h"
#include <glm/gtx/transform.hpp>
#include <iostream>
#include "Engine.h"

#include <cstdlib>

#include <bullet/btBulletDynamicsCommon.h>

// Colors
GLfloat WHITE[] = { 1, 1, 1 };
GLfloat RED[] = { 1, 0, 0 };
GLfloat GREEN[] = { 0, 1, 0 };
GLfloat MAGENTA[] = { 1, 0, 1 };

// Mesh


#define WIDTH 1600
#define HEIGHT 1200

#define WLEFT 0
#define WRIGHT WIDTH
#define WBOTTOM 0
#define WTOP HEIGHT 

#define RGBBLACK 0,0,0
#define RGBGREY .8,.8,.8

static int tracking = 0;
float previousx;
float previousy;

Engine engine = Engine();

// Mouse Tracking
int inwindow(int x, int y)
{
    return(x > WLEFT && x<WRIGHT&& y>WBOTTOM && y < WTOP);
}

void mouse_motion(int x, int y)
{
    engine.camera->mouseUpdate(glm::vec2(x, y));
    //std::cout << "Mouse update";
}



// A ball.  A ball has a radius, a color, and bounces up and down between
// a maximum height and the xz plane.  Therefore its x and z coordinates
// are fixed.  It uses a lame bouncing algorithm, simply moving up or
// down by 0.05 units at each frame.
class Ball {
    double radius;
    GLfloat* color;
    double maximumHeight;
    double x;
    double y;
    double z;
    int direction;
public:
    Ball(double r, GLfloat* c, double h, double x, double z) :
        radius(r), color(c), maximumHeight(h), direction(-1),
        y(h), x(x), z(z) {
    }
    void update() {
        y += direction * 0.05;
        if (y > maximumHeight) {
            y = maximumHeight; direction = -1;
        }
        else if (y < radius) {
            y = radius; direction = 1;
        }
        glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
        glTranslated(x, y, z);
        glutSolidSphere(radius, 30, 30);
        glPopMatrix();
    }
};

// A checkerboard class.  A checkerboard has alternating red and white
// squares.  The number of squares is set in the constructor.  Each square
// is 1 x 1.  One corner of the board is (0, 0) and the board stretches out
// along positive x and positive z.  It rests on the xz plane.  I put a
// spotlight at (4, 3, 7).
class Checkerboard {
    int displayListId;
    int width;
    int depth;
public:
    Checkerboard(int width, int depth) : width(width), depth(depth) {}
    double centerx() { return width / 2; }
    double centerz() { return depth / 2; }
    void create() {

        displayListId = glGenLists(1);
        glNewList(displayListId, GL_COMPILE);

        GLfloat lightPosition[] = { 10, 3, 7, 1 };
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

        glBegin(GL_QUADS);
        glNormal3d(0, 1, 0);
        for (int x = 0; x < width - 1; x++) {
            for (int z = 0; z < depth - 1; z++) {
                glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,
                    (x + z) % 2 == 0 ? GREEN : WHITE);
                glVertex3d(x, 0, z);
                glVertex3d(x + 1, 0, z);
                glVertex3d(x + 1, 0, z + 1);
                glVertex3d(x, 0, z + 1);
            }
        }
        glEnd();
        glEndList();
    }
    void update() {

    }
    void draw() {
        glCallList(displayListId);
        //create();
    }
};

// Global variables: a camera, a checkerboard and some balls.
Checkerboard checkerboard(18, 18);
Objects::Triangles triangles(0, 0, 25);


Ball balls[] = {
  Ball(1, GREEN, 7, 6, 1),
  Ball(1.5, MAGENTA, 6, 3, 4),
  Ball(0.4, WHITE, 5, 1, 7)
};


// Application-specific initialization: Set up global lighting parameters
// and create display lists.
void init() {
    glEnable(GL_DEPTH_TEST);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE);
    glLightfv(GL_LIGHT0, GL_SPECULAR, WHITE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, WHITE);
    glMaterialf(GL_FRONT, GL_SHININESS, 30);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);


    checkerboard.create();
}
// Return a random float in the range 0.0 to 1.0.
GLfloat randomFloat() {
    return (GLfloat)rand() / RAND_MAX;
}

// A fish bitmap, size is 27x11, but all rows must have a multiple of 8 bits,
// so we define it like it is 32x11.
GLubyte fish[] = {
  0x00, 0x60, 0x01, 0x00,
  0x00, 0x90, 0x01, 0x00,
  0x03, 0xf8, 0x02, 0x80,
  0x1c, 0x37, 0xe4, 0x40,
  0x20, 0x40, 0x90, 0x40,
  0xc0, 0x40, 0x78, 0x80,
  0x41, 0x37, 0x84, 0x80,
  0x1c, 0x1a, 0x04, 0x80,
  0x03, 0xe2, 0x02, 0x40,
  0x00, 0x11, 0x01, 0x40,
  0x00, 0x0f, 0x00, 0xe0,
};
void update() {
    engine.update();

}
// Draws one frame, the checkerboard then the balls, from the current camera
// position.
void display() {
    
    // Engine.display()
    engine.display();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    gluLookAt(engine.camera->position.x, engine.camera->position.y, engine.camera->position.z,
        engine.camera->viewDirection.x + engine.camera->position.x,
        engine.camera->viewDirection.y + engine.camera->position.y,
        engine.camera->viewDirection.z + engine.camera->position.z,
        engine.camera->UP.x, engine.camera->UP.y, engine.camera->UP.z);
    // Draws the fish bitmaps
    for (int i = 0; i < 20; i++) {
        glColor3f(randomFloat() * (i + 1), randomFloat() * (i + 1), randomFloat());
        glRasterPos3f(randomFloat() * (i + 1), randomFloat() * (i + 1), 0.0);
        glBitmap(27, 11, 0, 0, 0, 0, fish);
    }

    checkerboard.draw();
    for (int i = 0; i < sizeof balls / sizeof(Ball); i++) {
        balls[i].update();
    }
    triangles.draw();
    glFlush();
    glutSwapBuffers();

    //std::cout << "DISPLAYING";
}
#define red {0xff, 0x00, 0x00}
#define yellow {0xff, 0xff, 0x00}
#define magenta {0xff, 0, 0xff}

GLubyte texture[][3] = {
    red, yellow,
    yellow, red,
};
// On reshape, constructs a camera that perfectly fits the window.
void reshape(GLint w, GLint h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, GLfloat(w) / GLfloat(h), 1.0, 150.0);
    glMatrixMode(GL_MODELVIEW);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // reshape all textures?
    glTexImage2D(GL_TEXTURE_2D,
        0,                    // level 0
        3,                    // use only R, G, and B components
        2, 2,                 // texture has 2x2 texels
        0,                    // no border
        GL_RGB,               // texels are in RGB format
        GL_UNSIGNED_BYTE,     // color components are unsigned bytes
        texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}

// Requests to draw the next frame.
void timer(int v) {
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, v);
}

// Moves the camera according to the key pressed, then ask to refresh the
// display.
void special(int key, int, int) {
    switch (key) {
    case GLUT_KEY_LEFT: engine.camera->strafeLeft(); break;
    case GLUT_KEY_RIGHT: engine.camera->strafeRight(); break;
    case GLUT_KEY_UP: engine.camera->moveForward(); break;
    case GLUT_KEY_DOWN: engine.camera->moveBackward(); break;
    }
    glutPostRedisplay();
}

// Initializes GLUT and enters the main loop.
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(80, 80);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Bouncing Balls");
    //glutIdleFunc(update);
    glutDisplayFunc(display);
    //glutMouseFunc();
    glutMotionFunc(mouse_motion);
    //glutPassiveMotionFunc();
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    glutTimerFunc(100, timer, 0);
    init();
    glutMainLoop();
}
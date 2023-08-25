// This application shows balls bouncing on a checkerboard, with no respect
// for the laws of Newtonian Mechanics.  There's a little spotlight to make
// the animation interesting, and arrow keys move the camera for even more
// fun.
#include <GL/glew.h>
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>
#include "Engine.h"
#include <glm/gtx/transform.hpp>
#include <iostream>
#include "Objects.h"
#include <cstdlib>
#include <deque>
#include <vector>
#include <bullet/btBulletDynamicsCommon.h>

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

// Colors
GLfloat WHITE[] = { 1, 1, 1 };
GLfloat RED[] = { 1, 0, 0 };
GLfloat GREEN[] = { 0, 1, 0 };
GLfloat MAGENTA[] = { 1, 0, 1 };

Engine* engine = Engine::Instance();

// Global variables
Floor groundfloor(18, 18);
Triangles triangles(1, 1, 1);
std::vector<std::unique_ptr<Rain>> rain;

Ball balls[] = {
  Ball(1, GREEN, 0, 10, 10.f),
  Ball(1.5, GREEN, 0, 20, 10.f),
  Ball(0.4, WHITE, 0, 30, 10.f)
};

// Mouse Tracking
int inwindow(int x, int y)
{
    return(x > WLEFT && x<WRIGHT&& y>WBOTTOM && y < WTOP);
}

void mouse_motion(int x, int y)
{
    engine->camera->mouseUpdate(glm::vec2(x, y));
}

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
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    glewInit();
    groundfloor.create();
    //mesh.create();
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

int it = 0; // Simple seed counter for generating rain
void update() {

    engine->update();

    // Update/ Generate Rain
    it++;
    for (auto& r : rain)
    {
        r->update();
        if (r->getLife() > r->getMaxLife())
        {
            r = nullptr;
        }
    }
    if (it % 4 == 0) {
        it = 0;
        rain.push_back(std::make_unique<Rain>(randomFloat(),  randomFloat(), randomFloat(), RED));
    }
    rain.erase(std::remove(begin(rain), end(rain), nullptr),
        end(rain));
    groundfloor.update();
}

// Draws one frame of our scene from the current camera
// position.
void display() {

    engine->pre_display();
    groundfloor.draw();

    for (auto& r : rain)
    {
        if (r != nullptr)
            r->display();
    }

    // Draws the fish bitmaps
    for (int i = 0; i < 20; i++) {
        glColor3f(randomFloat() * (i + 1), randomFloat() * (i + 1), randomFloat());
        glRasterPos3f(randomFloat() * (i + 1), randomFloat() * (i + 1), 0.0);
        glBitmap(27, 11, 0, 0, 0, 0, fish);
    }
    
    for (int i = 0; i < sizeof balls / sizeof(Ball); i++) {
        balls[i].display();
    }

    glFlush();
    glutSwapBuffers();
}

// On reshape, constructs a camera that perfectly fits the window.
void reshape(GLint w, GLint h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, GLfloat(w) / GLfloat(h), 1.0, 150.0);
    glMatrixMode(GL_MODELVIEW);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Engine has this reshape function that can be iterated to reshape a texture
    //    engine->reshape();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

// Requests to draw the next frame.
void timer(int v) {
    glutPostRedisplay();
    glutTimerFunc(1000 / 60 * Engine::Instance()->deltaTime , timer, v);
}

// Moves the camera according to the key pressed, then ask to refresh the
// display.
void special(int key, int, int) {
    switch (key) {
        case GLUT_KEY_LEFT: engine->camera->strafeLeft(); break;
        case GLUT_KEY_RIGHT: engine->camera->strafeRight(); break;
        case GLUT_KEY_UP: engine->camera->moveForward(); break;
        case GLUT_KEY_DOWN: engine->camera->moveBackward(); break;
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'w': engine->camera->moveUp(); break;
        case 's': engine->camera->moveDown(); break;
    }
}

// Initializes GLUT and enters the main loop.
int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(80, 80);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Rain Room");
    glutIdleFunc(update);
    glutDisplayFunc(display);
    //glutMouseFunc();
    glutMotionFunc(mouse_motion);
    //glutPassiveMotionFunc();
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(100, timer, 0);
    init();
    glutMainLoop();
}
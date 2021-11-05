#pragma once
#include <glm\gtx\transform.hpp>

// A camera.  It moves horizontally in a circle centered at the origin of
// radius 10.  It moves vertically straight up and down.
class Camera {


public:
	glm::vec3 position;
	glm::vec3 viewDirection;
	glm::vec2 oldMousePosition;
	const glm::vec3 UP;

	float ms = 1.0f;
	Camera();
	void mouseUpdate(const glm::vec2& newMousePosition);
	
	glm::mat4 getWorldToViewMatrix();


	void moveBackward();
	void strafeLeft();
	void strafeRight();
	void moveUp();
	void moveDown();
	void moveForward();
    void swingRight() {  }
    void swingLeft() {}
	void swingUp() {};
	void swingDown() {};




	//
	//    struct Target {
	//        double y = 0;
	//        double x = 0;
	//        double z = 0;
	//        double yrot = 0;
	//        double xrot = 0;
	//
	//        double PI_OVER_180 = 3.141459 / 180.0;
	//
	//    public:
	//        double getX() { return x; }
	//        double getY() { return y; }
	//        double getZ() { return z; }
	//        void moveForward() {
	//
	//
	//        }
	//        void moveBackward() { z--; }
	//    };
	//    double y;          // the current y position
	//    double x;
	//    double z;
	//    double theta;      // determines the x and z positions
	//    double dTheta;     // increment in theta for swinging the camera around
	//    double dy;         // increment in y for moving the camera up/down
	//    double d = 0.1;
	//
	//public:
	//    Camera() : theta(0), y(3), x(0), z(0), dTheta(0.04), dy(0.2) {
	//
	//    }
	//    Target _tar;
	//    double getX() {
	//        return (10 * cos(theta)) + _tar.x;
	//    }
	//    double getY() { return y + _tar.y; }
	//    double getZ() { return (10 * sin(theta)) + _tar.z; }
	//
	//    void swingRight() { theta += dTheta; _tar.yrot += dTheta * 10; }
	//    void swingLeft() {
	//        theta -= dTheta; _tar.yrot -= dTheta * 10;
	//    }
	//    void swingUp() { y += dy; }
	//    void swingDown() { if (y > dy) y -= dy; }
	//

};
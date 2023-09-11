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
	float ms = 0.20f;
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

};
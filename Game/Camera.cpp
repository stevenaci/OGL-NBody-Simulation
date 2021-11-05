#include "Camera.h"
#include <glm/gtx/transform.hpp>
#include <iostream>

Camera::Camera() :
	viewDirection(0.0, 0.0, -5.0),
	UP(0.0f, 1.0f, 0.0f), position(2.0, 12.0, 0.0)
{
}

void Camera::mouseUpdate(const glm::vec2& newMousePosition) {
	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;

	if (glm::length(mouseDelta) > 10.0f)
	{
		oldMousePosition = newMousePosition;
		return;
	}
	const float ROTATIONAL_SPEED = 0.2;
	glm::vec3 toRotateAround = glm::cross(viewDirection, UP);
	glm::mat4 rotator = glm::rotate(-mouseDelta.x * ROTATIONAL_SPEED, UP) *
		glm::rotate(-mouseDelta.y * ROTATIONAL_SPEED, toRotateAround);

	viewDirection = glm::mat3(rotator) * viewDirection;

	std::cout << "\n camera direction:" << viewDirection.x << ":" << viewDirection.y << ":" << viewDirection.z << "\n";
	oldMousePosition = newMousePosition;
}

glm::mat4 Camera::getWorldToViewMatrix()
{
	return glm::lookAt(position, viewDirection + position, UP);
}

void Camera::moveBackward()
{

	position -= ms * viewDirection;
}

void Camera::strafeLeft()
{
	glm::vec3 strafeDirection = glm::cross(viewDirection, UP);
	position -= ms * strafeDirection;
}

void Camera::strafeRight()
{
	glm::vec3 strafeDirection = glm::cross(viewDirection, UP);
	position += ms * strafeDirection;
}

void Camera::moveUp()
{
	position += ms * UP;
}

void Camera::moveDown()
{

	position -= ms * UP;
}

void Camera::moveForward()
{
	position += ms * viewDirection;
}

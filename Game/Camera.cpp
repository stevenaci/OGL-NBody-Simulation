#include "Camera.h"
#include <glm/gtx/transform.hpp>
#include <iostream>

Camera::Camera() :
	viewDirection(2.0f, 2.0f, 1.0f),
	UP(0.0f, 1.0f, 0.0f),position(77.0,-77.0,-25.0)
{
}

void Camera::mouseUpdate(const glm::vec2& newMousePosition) {
	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;

	if (glm::length(mouseDelta) > 50.0f)
	{
		oldMousePosition = newMousePosition;
		return;
	}
	const float ROTATIONAL_SPEED = 0.02f;
	glm::vec3 toRotateAround = glm::cross(viewDirection, UP);
	glm::mat4 rotator = glm::rotate(-mouseDelta.x * ROTATIONAL_SPEED, UP) *
		glm::rotate(-mouseDelta.y * ROTATIONAL_SPEED, toRotateAround);

	viewDirection = glm::mat3(rotator) * viewDirection;

	std::cout << "\n" << viewDirection.x << ":" << viewDirection.y << ":" << viewDirection.z << "\n";
	oldMousePosition = newMousePosition;
}

glm::mat4 Camera::getWorldToViewMatrix() const
{
	return glm::lookAt(position, position + viewDirection, UP);
}

void Camera::moveBackward()
{

	position -= movespeed * viewDirection;
}

void Camera::strafeLeft()
{
	glm::vec3 strafeDirection = glm::cross(viewDirection, UP);
	position -= movespeed * strafeDirection;
}

void Camera::strafeRight()
{
	glm::vec3 strafeDirection = glm::cross(viewDirection, UP);
	position += movespeed * strafeDirection;
}

void Camera::moveUp()
{
	position += movespeed * UP;
}

void Camera::moveDown()
{

	position -= movespeed * UP;
}

void Camera::moveForward()
{
	position += movespeed * viewDirection;
}
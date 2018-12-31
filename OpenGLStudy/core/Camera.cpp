#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 eulerAngles)
{
	m_position = position;
	m_eulerAngles = eulerAngles;
	m_transform = glm::eulerAngleYXZ(eulerAngles.y, eulerAngles.x, eulerAngles.z) * glm::translate(glm::mat4(1.0f), m_position);
	m_invTransform = glm::inverse(m_transform);
}

void Camera::setPosition(glm::vec3 position)
{
}

void Camera::setRotation(glm::vec3 eulerAngles)
{
}

void Camera::setTransform(glm::mat4 transform)
{
}

glm::vec3 Camera::getPosition()
{
	return glm::vec3();
}

glm::vec3 Camera::getRotation()
{
	return glm::vec3();
}

glm::mat4 Camera::getTransform()
{
	return glm::mat4();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::mat4();
}

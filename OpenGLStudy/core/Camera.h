#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"

class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 eulerAngles);

	void setPosition(glm::vec3 position);
	void setRotation(glm::vec3 eulerAngles);
	void setTransform(glm::mat4 transform);

	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::mat4 getTransform();

	glm::mat4 getViewMatrix();

private:
	glm::vec3 m_eulerAngles;
	glm::vec3 m_position;
	glm::mat4 m_transform;
	glm::mat4 m_invTransform;
};

#endif
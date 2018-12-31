#ifndef SIMPLECAMERA_H
#define SIMPLECAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const float SENSITIVITY = 0.1f;
const float HORIZONTAL_SPEED = 4.0f;
const float VERTICAL_SPEED = 3.0f;

class SimpleCamera
{
public:
	SimpleCamera(glm::vec3 position, float yaw, float pitch);
	void move(glm::vec3 direction, float deltaTime);
	void rotate(float yawOffset, float pitchOffset);
	const glm::mat4 getViewMatrix();
	glm::vec3 getPosition();

private:
	float yaw;
	float pitch;
	glm::mat4 intermediate;
	glm::mat4 view;
};

#endif
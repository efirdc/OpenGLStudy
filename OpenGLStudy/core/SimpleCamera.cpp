#include "SimpleCamera.h"

SimpleCamera::SimpleCamera(glm::vec3 position, float yaw, float pitch) :
	yaw(yaw),
	pitch(pitch)
{
	view[3] = glm::vec4(position, 1.0f);
	rotate(0.0f, 0.0f);
}

void SimpleCamera::move(glm::vec3 direction, float deltaTime)
{
	glm::vec3 speed(HORIZONTAL_SPEED, VERTICAL_SPEED, HORIZONTAL_SPEED);
	
	// position is negative in the view matrix because camera needs to translate objects to the origin
	view[3] += intermediate * glm::vec4(direction * speed * deltaTime, 0.0f);
}

void SimpleCamera::rotate(float yawOffset, float pitchOffset)
{
	// save position, and reset views to identity matrix
	glm::vec4 position = view[3];
	view = intermediate = glm::mat4(1.0f);

	// constrain yaw to [0, 360] and pitch to [-90, 90]
	yaw = glm::mod(yaw - yawOffset * SENSITIVITY, 360.0f);
	pitch = glm::clamp(pitch - pitchOffset * SENSITIVITY, -90.0f, 90.0f);

	// rotate by yaw around the vertical y-axis
	// followed by rotation by pitch on the right facing x-axis
	intermediate = glm::rotate(intermediate, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::rotate(intermediate, glm::radians(pitch), glm::vec3(view[0]));
	view[3] = position;
}

const glm::mat4 SimpleCamera::getViewMatrix()
{
	return glm::inverse(view);
}

glm::vec3 SimpleCamera::getPosition()
{
	return glm::vec3(view[3]);
}

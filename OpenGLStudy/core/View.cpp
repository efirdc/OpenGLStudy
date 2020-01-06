#include "View.h"

void View::updateView2(GLFWwindow* window, const glm::vec2& deltaMousePos, float deltaTime)
{
	// disable control when mouse is active
	glm::vec3 movementAxis(0.0f);
	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
	{
		// get movement axis
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			movementAxis[0] -= 1.0f;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			movementAxis[0] += 1.0f;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			movementAxis[2] -= 1.0f;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			movementAxis[2] += 1.0f;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			movementAxis[1] += 1.0f;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			movementAxis[1] -= 1.0f;
		movementAxis = glm::length(movementAxis) > 0.0f ? glm::normalize(movementAxis) : movementAxis;

		// constrain y axis rotation (yaw) to [0, 360] and x axis rotation (pitch) to [-90, 90]
		eulerAngles.y = glm::mod(eulerAngles.y - deltaMousePos.x * sensitivity, 360.0f);
		eulerAngles.x = glm::clamp(eulerAngles.x - deltaMousePos.y * sensitivity, -90.0f, 90.0f);
	}

	// perform rotation around y axis (yaw) and rotation around x axis (pitch)
	glm::mat4 intermediate = glm::rotate(glm::mat4(1.0f), glm::radians(eulerAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
	matrix = glm::rotate(intermediate, glm::radians(eulerAngles.x), glm::vec3(1.0f, 0.0f, 0.0f));

	// calculate deccelerated velocity

	// decellerate when the movement axis is 0, accelerate otherwise
	float speed = glm::length(velocity);
	if (glm::length(movementAxis) == 0.0f && speed != 0.0f)
		velocity = glm::clamp(speed - decceleration * deltaTime, 0.0f, maxVelocity) * glm::normalize(velocity);
	else
	{
		glm::vec3 accelDir(intermediate * glm::vec4(movementAxis, 0.0f));
		velocity += accelDir * acceleration * deltaTime;
		if (speed > maxVelocity)
			velocity = glm::normalize(velocity) * maxVelocity;
	}

	// perform translation and calculate inverse matrix
	position += velocity * deltaTime;
	matrix[3] = glm::vec4(position, 1.0f);
	invMatrix = glm::inverse(matrix);
}
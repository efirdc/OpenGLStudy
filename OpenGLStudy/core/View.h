#ifndef VIEW_H
#define VIEW_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GLFW/glfw3.h"

class View
{
public:
	glm::vec3 position;
	glm::vec3 eulerAngles;
	glm::mat4 matrix;
	glm::mat4 invMatrix;
	glm::vec3 velocity;
	float sensitivity = 0.1f;
	float maxVelocity = 18.0f;
	float acceleration = maxVelocity / 0.05f;
	float decceleration = maxVelocity / 0.05f;

	void updateView2(GLFWwindow* window, const glm::vec2& deltaMousePos, float deltaTime);
};

#endif
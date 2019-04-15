#include "SceneManager.h"

void SceneManager::newFrame()
{
	// esc to quit
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// update framebuffer size
	sizeFramebufferToWindow();

	// update time
	float newTime = glfwGetTime();
	deltaTime = newTime - time;
	time = newTime;

	// update mouse
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	glm::vec2 newMousePos((float)mouseX, (float)mouseY);
	deltaMousePos = newMousePos - mousePos;
	prevMousePos = mousePos;
	mousePos = newMousePos;

	// handle switching between camera mode and mouse active mode
	if (captureMouse) {
		bool mouseInWindow = glm::all(glm::greaterThan(mousePos, glm::vec2(0.0)) && glm::lessThan(mousePos, screenSize));
		bool leftMouseClicked = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;
		bool leftCtrl = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;
		if (leftMouseClicked)
		{
			if (mouseInWindow && !imguiIO->WantCaptureMouse)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			else
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		if (leftCtrl)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void SceneManager::sizeFramebufferToWindow()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	int frameBufferWidth, frameBufferHeight;
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	glViewport(0, 0, frameBufferWidth, frameBufferHeight);
	screenSize = glm::vec2((float)frameBufferWidth, (float)frameBufferHeight);
}
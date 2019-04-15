#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

class SceneManager
{
public:
	GLFWwindow * window;
	ImGuiIO * imguiIO;
	glm::vec2 screenSize;
	glm::vec2 mousePos;
	glm::vec2 prevMousePos;
	glm::vec2 deltaMousePos;
	float time;
	float deltaTime;
	bool leftMouseDown;
	bool rightMouseDown;

	bool captureMouse;

	void newFrame();
	void sizeFramebufferToWindow();
};
#endif
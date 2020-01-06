#pragma once

#include <iostream>
#include <string>

#include "glDebug.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

class GLProgram
{
public:
	GLProgram(
		int versionMajor,
		int versionMinor,
		bool captureMouse,
		int windowWidth,
		int windowHeight,
		std::string windowName,
		bool glDebug
	);

	~GLProgram();

	void run();

protected:
	GLFWwindow* window;
	ImGuiIO* imguiIO;

	int versionMajor;
	int versionMinor;
	bool captureMouse;
	std::string windowName;
	glm::vec2 screenSize;
	bool glDebug;

	glm::vec2 mousePos;
	glm::vec2 prevMousePos;
	glm::vec2 deltaMousePos;

	float time = 0.0f;
	float deltaTime = 0.0f;
	int frameNumber = 0;
	bool leftMouseDown = false;
	bool rightMouseDown = false;
	
	void newFrame();
	void sizeFramebufferToWindow();
	
	virtual void update() = 0;
};


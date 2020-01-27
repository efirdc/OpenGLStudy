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
#include <map>

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
	glm::ivec2 screenSize;
	bool glDebug;

	glm::vec2 mousePos;
	glm::vec2 prevMousePos;
	glm::vec2 deltaMousePos;

	float time = 0.0f;
	float deltaTime = 0.0f;
	int frameNumber = 0;

	struct InputData {bool pressed = false, held = false;};

	std::map<int, InputData> mouseInputs;
	std::map<int, InputData> keyInputs;

	void updateInputData();
	void newFrame();
	void sizeFramebufferToWindow();
	
	virtual void update() = 0;
};


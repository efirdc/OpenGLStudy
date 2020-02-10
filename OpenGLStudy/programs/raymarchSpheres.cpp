#include <iostream>
#include <math.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "Shader.h"
#include "SceneManager.h"
#include "View.h"

#include "utilities.h"

struct TextureUnits
{
	const int sphereData1 = 0;
	const int sphereData2 = 1;
};

int raymarchSpheres()
{
	// Struct to hold scene data
	SceneManager * sceneManager = new SceneManager;
	sceneManager->captureMouse = true;

	// Initialize GLFW, set version to 4.3, tell OpenGL that we want to use the core profile
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window object and error check
	// Make the window the curent context
	sceneManager->window = glfwCreateWindow(1600, 900, "Raymarch test", NULL, NULL);
	if (sceneManager->window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(sceneManager->window);
	glfwSwapInterval(0);

	// Initialize GLAD to get OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	// Setup ImGui
	ImGui::CreateContext();
	sceneManager->imguiIO = &ImGui::GetIO(); (void)sceneManager->imguiIO;
	ImGui_ImplGlfwGL3_Init(sceneManager->window, true);
	ImGui::StyleColorsDark();

	// View
	View * view = new View;

	Shader::bindGlobalUniform("time", (void *)&sceneManager->time);

	Shader raymarch("shaders/raymarch/raymarch.vert", "shaders/raymarch/spheres.frag");
	Shader compute(GL_COMPUTE_SHADER, "shaders/compute/spheres.comp");

	// Setup quad VBO
	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	unsigned int quadVBO;
	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	// Setup VAO
	unsigned int quadVAO;
	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Compute shader experiment
	unsigned int computeShaderTexture[2];
	glGenTextures(2, computeShaderTexture);
	for (int i = 0; i < 2; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_3D, computeShaderTexture[i]);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, 64, 64, 64, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindImageTexture(i, computeShaderTexture[i], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	}
	
	static glm::vec3 clearColor(0.0f);

	TextureUnits textureUnits;
	float gamma = 2.2f;
	Shader::bindGlobalUniform("gamma", (void *)&gamma);
	Shader::bindGlobalUniform("view", (void *)&(view->matrix));
	Shader::bindGlobalUniform("sphereData1", (void *)&textureUnits.sphereData1);
	Shader::bindGlobalUniform("sphereData2", (void *)&textureUnits.sphereData2);

	// Main loop
	sceneManager->newFrame();
	while (!glfwWindowShouldClose(sceneManager->window))
	{
		// new frame
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();
		sceneManager->newFrame();

		// Settings window
		ImGuiWindowFlags windowFlags = 0;
		if (glfwGetInputMode(sceneManager->window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
			windowFlags |= ImGuiWindowFlags_NoInputs;
		ImGui::Begin("Settings (Press CTRL to activate mouse)", nullptr, windowFlags);
		static glm::vec3 fogColor(0.1f);
		static glm::vec3 backgroundColor(0.05f);
		{
			ImGui::Text("Use WASD to move, SPACE to rise, LEFT SHIFT to fall.");
			ImGui::SliderFloat("gamma", &gamma, 1.0f, 2.2f);
			ImGui::ColorEdit3("fog color", (float*)&fogColor);
			ImGui::ColorEdit3("background color", (float*)&backgroundColor);

			// Error reporting
			static int lastError = 0;
			int currentError = glGetError();
			if (currentError)
				lastError = currentError;
			ImGui::Text("Current Opengl Error Code: %d", currentError);
			ImGui::Text("Last Opengl Error Code: %d", lastError);

			// Display fps
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
		ImGui::End();

		for (int i = 0; i < 2; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_3D, computeShaderTexture[i]);
		}
		compute.update();
		compute.use();
		glDispatchCompute(64, 64, 64);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		// clear stuff
		glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// get view and projection matrices
		view->updateView2(sceneManager->window, sceneManager->deltaMousePos, sceneManager->deltaTime);
		float aspect = sceneManager->screenSize.y != 0.0f ? (float)sceneManager->screenSize.x / (float)sceneManager->screenSize.y : 1.0f;
		glm::mat4 projection = glm::perspective(glm::radians(75.0f), aspect, 0.1f, 100.0f);

		raymarch.update();
		raymarch.use();
		raymarch.setUniform("view", view->matrix);
		raymarch.setUniform("aspect", aspect);
		raymarch.setUniform("fogColor", fogColor);
		raymarch.setUniform("backgroundColor", backgroundColor);

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// end frame stuff
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(sceneManager->window);
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}
#include <algorithm>
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "utilities.h"

#include "glm/glm.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "Shader.h"
#include "SceneManager.h"
#include "FluidBuffer.h"

int fluidSimulation()
{
	// Set up scene manager
	SceneManager * sceneManager = new SceneManager;
	sceneManager->captureMouse = false;

	// Initialize GLFW, set version to 3.3, tell OpenGL that we want to use the core profile
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window object and error check, make the window the curent context, then bind the window resize callback function
	sceneManager->window = glfwCreateWindow(1600, 900, "GPU Fluid Simulation", NULL, NULL);
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

	// Setup ImGui
	ImGui::CreateContext();
	sceneManager->imguiIO = &ImGui::GetIO(); (void)sceneManager->imguiIO;
	ImGui_ImplGlfwGL3_Init(sceneManager->window, true);
	ImGui::StyleColorsDark();

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

	// Fluid values
	int fluidWidth = 640;
	int fluidHeight = 360;
	float standardTimestep = 1.0f / 60.0f;
	float mouseSplatRadius = 7.5f;
	float mouseForce = 0.275f;
	int pressureIterations = 50;

	FluidBuffer fluidBuffer(640, 360);
	
	Shader displayShader("shaders/fluid/screenQuad.vs", "shaders/fluid/display.fs");
	Shader advectVelocityShader("shaders/fluid/screenQuad.vs", "shaders/fluid/advectVelocity.fs");
	Shader velocitySplatShader("shaders/fluid/screenQuad.vs", "shaders/fluid/simpleSplat.fs");
	Shader divergenceShader("shaders/fluid/screenQuad.vs", "shaders/fluid/divergence.fs");
	Shader pressureShader("shaders/fluid/screenQuad.vs", "shaders/fluid/pressure.fs");
	Shader subtractPressureShader("shaders/fluid/screenQuad.vs", "shaders/fluid/subtractPressure.fs");

	// Main loop
	sceneManager->newFrame();
	while (!glfwWindowShouldClose(sceneManager->window))
	{
		// new frame
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();
		sceneManager->newFrame();

		// Settings window
		static float timestep = 1.0f;
		static int displayMode = 1;
		ImGui::Begin("Settings");
		{
			ImGui::SliderInt("pressure iterations", &pressureIterations, 1, 200);
			ImGui::SliderFloat("timestep", &timestep, 0.01f, 5.0f);
			standardTimestep = timestep / 60.0f;
			ImGui::SliderFloat("mouse radius", &mouseSplatRadius, 1.0f, 50.0f);
			ImGui::SliderFloat("mouse force", &mouseForce, 0.01f, 1.0f);
			const char * displayModes[] = {"All", "Velocity", "Pressure", "Divergence"};
			ImGui::Combo("display mode", &displayMode, displayModes, IM_ARRAYSIZE(displayModes));
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

		bool showDemoWindow = true;
		//ImGui::ShowDemoWindow(&showDemoWindow);

		// Velocity splat step
		fluidBuffer.bind();
		velocitySplatShader.use();
		velocitySplatShader.setInt("fluid", 0);
		velocitySplatShader.setFloat("timestep", sceneManager->deltaTime / standardTimestep);
		velocitySplatShader.setVec2("pixelSize", 1.0f / glm::vec2(fluidWidth, fluidHeight));
		glm::vec2 texCoordMousePos = sceneManager->mousePos / sceneManager->screenSize;
		texCoordMousePos.y = 1.0f - texCoordMousePos.y;
		glm::vec2 fluidMouse = texCoordMousePos * glm::vec2(fluidWidth, fluidHeight);
		velocitySplatShader.setVec2("mousePosition", fluidMouse);
		velocitySplatShader.setVec2("mouseDelta", sceneManager->deltaMousePos * glm::vec2(1.0f, -1.0f));
		velocitySplatShader.setFloat("mouseForce", mouseForce);
		velocitySplatShader.setFloat("radius", mouseSplatRadius);
		velocitySplatShader.setFloat("leftMouseDown", sceneManager->leftMouseDown ? 1.0f : 0.0f);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		fluidBuffer.swapFluidBuffers();

		// Advection step
		fluidBuffer.bind();
		advectVelocityShader.use();
		advectVelocityShader.setInt("fluid", 0);
		advectVelocityShader.setFloat("timestep", sceneManager->deltaTime / standardTimestep);
		advectVelocityShader.setVec2("pixelSize", 1.0f / glm::vec2(fluidWidth, fluidHeight));
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		fluidBuffer.swapFluidBuffers();

		// Divergence step
		fluidBuffer.bind();
		glViewport(0, 0, fluidWidth, fluidHeight);
		divergenceShader.use();
		divergenceShader.setInt("fluid", 0);
		divergenceShader.setFloat("timestep", sceneManager->deltaTime / standardTimestep);
		divergenceShader.setVec2("pixelSize", 1.0f / glm::vec2(fluidWidth, fluidHeight));
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		fluidBuffer.swapFluidBuffers();

		// Pressure step
		for (int i = 0; i < pressureIterations; i++) {
			fluidBuffer.bind();
			glViewport(0, 0, fluidWidth, fluidHeight);
			pressureShader.use();
			pressureShader.setInt("fluid", 0);
			pressureShader.setFloat("timestep", sceneManager->deltaTime / standardTimestep);
			pressureShader.setVec2("pixelSize", 1.0f / glm::vec2(fluidWidth, fluidHeight));
			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			fluidBuffer.swapFluidBuffers();
		}

		// Subtract pressure step
		fluidBuffer.bind();
		glViewport(0, 0, fluidWidth, fluidHeight);
		subtractPressureShader.use();
		subtractPressureShader.setInt("fluid", 0);
		subtractPressureShader.setFloat("timestep", sceneManager->deltaTime / standardTimestep);
		subtractPressureShader.setVec2("pixelSize", 1.0f / glm::vec2(fluidWidth, fluidHeight));
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		fluidBuffer.swapFluidBuffers();

		// Use the default framebuffer
		sceneManager->sizeFramebufferToWindow();
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Display final texture on the default framebuffer
		fluidBuffer.bindTextures();
		displayShader.use();
		displayShader.setInt("fluid", 0);
		displayShader.setInt("displayMode", displayMode);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// swap the buffers
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(sceneManager->window);
	}

	// terminate glfw, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}
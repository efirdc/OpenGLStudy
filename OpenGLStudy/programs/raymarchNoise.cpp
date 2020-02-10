#include <iostream>
#include <math.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "glDebug.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "imgui_color_gradient.h"

#include "Shader.h"
#include "SceneManager.h"
#include "StreamTexture.h"
#include "View.h"

#include "utilities.h"

int raymarchNoise()
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
	sceneManager->window = glfwCreateWindow(1600, 900, "Raymarch", NULL, NULL);
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

	glDebug_enable();

	// Setup ImGui
	ImGui::CreateContext();
	sceneManager->imguiIO = &ImGui::GetIO(); (void)sceneManager->imguiIO;
	ImGui_ImplGlfwGL3_Init(sceneManager->window, true);
	ImGui::StyleColorsDark();

	// View
	View * view = new View;
	Shader raymarch("shaders/raymarch/raymarch.vert", "shaders/raymarch/volumetric.frag");
	Shader compute(GL_COMPUTE_SHADER, "shaders/compute/noiseVolume.comp");

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
	//ComputeShader::printSizes();
	const int volumeSize = 64;
	unsigned int computeShaderTexture;
	glGenTextures(1, &computeShaderTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, computeShaderTexture);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, volumeSize, volumeSize, volumeSize, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	const float borderValues[4] = { 0.0, 0.0, 0.0, 0.0 };
	glTexParameterfv(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, borderValues);
	glBindImageTexture(0, computeShaderTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	static glm::vec3 clearColor(0.0f);

	const struct TextureUnits {
		int noise = 0;
		int cloudColorCurve = 1;
	} textureUnits;

	StreamTexture1D * cloudColorCurve = new StreamTexture1D(GL_RGB32F, 256, GL_RGB, GL_FLOAT, 3, 4, false);
	glActiveTexture(GL_TEXTURE0 + textureUnits.cloudColorCurve);
	glBindTexture(GL_TEXTURE_1D, cloudColorCurve->textureID);
	ImGradient colorGradient;
	glm::vec3 * densityColors = (glm::vec3 *)cloudColorCurve->getPixelBuffer();
	int numColors = cloudColorCurve->width;
	for (int i = 0; i < numColors; i++)
	{
		ImVec4 color = colorGradient.getColorAt((float)i / (float)(numColors - 1));
		densityColors[i] = glm::vec3(color.x, color.y, color.z);
	}
	cloudColorCurve->unmapPixelBuffer();
	cloudColorCurve->getPixelBuffer(); // bug workadound. StreamTexture doesnt initialize properly without this
	cloudColorCurve->unmapPixelBuffer();

	float gamma = 2.2f;
	Shader::bindGlobalUniform("time", (void *)&sceneManager->time);
	Shader::bindGlobalUniform("gamma", (void *)&gamma);
	Shader::bindGlobalUniform("view", (void *)&(view->matrix));
	Shader::bindGlobalUniform("noiseData", (void *)&textureUnits.noise);
	Shader::bindGlobalUniform("cloudColorCurve", (void *)&textureUnits.cloudColorCurve);

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

			// Control the density color gradient
			bool changed = false;
			if (ImGui::TreeNode("Cloud Color Gradient"))
			{
				changed = ImGui::GradientEditor(&colorGradient);
				ImGui::TreePop();
			}
			if (changed)
			{
				glm::vec3 * colors = (glm::vec3 *)cloudColorCurve->getPixelBuffer();
				int numColors = cloudColorCurve->width;
				for (int i = 0; i < numColors; i++)
				{
					ImVec4 color = colorGradient.getColorAt((float)i / (float)(numColors - 1));
					colors[i] = glm::vec3(color.x, color.y, color.z);
				}
				cloudColorCurve->unmapPixelBuffer();
				cloudColorCurve->getPixelBuffer();
				cloudColorCurve->unmapPixelBuffer();
			}

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

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_3D, computeShaderTexture);
		compute.update();
		compute.use();
		glDispatchCompute(volumeSize, volumeSize, volumeSize);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);

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
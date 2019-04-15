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
#include "SimpleCamera.h"
#include "SceneManager.h"

#include "loopback.h"
#include "SpectrumAnalyzer.h"
#include "SpectrumFilter.h"
#include "utilities.h"

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

struct View
{
	View() : position(glm::vec3(0.0f)), eulerAngles(glm::vec3(0.0f)) {}
	glm::vec3 position;
	glm::vec3 eulerAngles;
	glm::mat4 matrix;
	glm::mat4 invMatrix;
};

static void updateView(View * view, SceneManager * sceneManager);
static void updateView2(View * view, SceneManager * sceneManager);

int sphereParticles()
{
	// Struct to hold scene data
	SceneManager * sceneManager = new SceneManager;
	sceneManager->captureMouse = true;

	// Initialize GLFW, set version to 3.3, tell OpenGL that we want to use the core profile
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window object and error check
	// Make the window the curent context
	sceneManager->window = glfwCreateWindow(1600, 900, "Spherical particle system", NULL, NULL);
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
	_crtBreakAlloc = 18;

	// Setup ImGui
	ImGui::CreateContext();
	sceneManager->imguiIO = &ImGui::GetIO(); (void)sceneManager->imguiIO;
	ImGui_ImplGlfwGL3_Init(sceneManager->window, true);
	ImGui::StyleColorsDark();

	// Setup VBO for the cube
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
	};
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// Setup VAOs
	unsigned int particleVAO, lightVAO;
	glGenVertexArrays(1, &particleVAO);
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(particleVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(lightVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// shaders
	Shader cubeShader("shaders/cubeParticle.vs", "shaders/cubeParticle.fs");
	Shader lightShader("shaders/light.vs", "shaders/light.fs");

	// Camera
	// SimpleCamera camera(glm::vec3(0.0f, 0.0f, 1.5f), 0.0f, 0.0f);

	const int frameSize = 4096;
	const int numSpectrumsInAverage = 18;
	const int numFreqBins = 1024;
	const float domainShiftFactor = 10.0f;

	SpectrumAnalyzer analyzer(frameSize);
	
	loopback_init();
	int frameGap = 128;
	int numAudioSamples = analyzer.getFrameSize() * 2;
	const int bezierCurveSize = 4096;
	float * audioBuffer = new float[numAudioSamples]();

	// initialize frequency amplitude curve
	float * frequencyAmplitudeCurve = new float[bezierCurveSize]();
	ImVec2 fAmpControlPoints[2] = { { 0.0f, 0.25f },{ 0.0f, 1.0f } };
	glm::vec2 frequencyAmplitudePoints[bezierCurveSize];
	utl::bezierTable((glm::vec2 *)fAmpControlPoints, frequencyAmplitudePoints, bezierCurveSize);
	utl::curve2Dto1D(frequencyAmplitudePoints, bezierCurveSize, frequencyAmplitudeCurve, bezierCurveSize);
	
	// initialize peak smoothing curve
	float peakRadius = 0.05f;
	int peakCurveSize = (int)((float)numFreqBins * peakRadius);
	float * peakCurve = new float[peakCurveSize]();
	ImVec2 peakControlPoints[2] = { { 1.00f, 0.00f },{ 0.0f, 1.00f } };
	glm::vec2 peakCurvePoints[bezierCurveSize];
	utl::bezierTable((glm::vec2 *)peakControlPoints, peakCurvePoints, bezierCurveSize);
	utl::curve2Dto1D(peakCurvePoints, bezierCurveSize, peakCurve, peakCurveSize);

	AmplitudeFilter amplitudeFilter(frequencyAmplitudeCurve, bezierCurveSize);
	DomainShiftFilter domainShiftFilter(domainShiftFactor, numFreqBins);
	PeakFilter peakFilter(peakCurve, peakCurveSize);
	AverageFilter averageFilter(numSpectrumsInAverage);

	// View
	View * view = new View;

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
		static int numObjects = 500;
		static float objectScale = 0.05f;
		static float timeScale = 0.5f;
		static float objectRotationScalar = 2.5f;
		static float objectRotationScalar2 = 45.0f;
		static float objectTranslationScalar = 2.0f;
		static glm::vec3 rotationAxis(0.4f, 1.0, 0.1f);
		static glm::vec3 color1(1.0f);
		static glm::vec3 color2(0.0f);
		static glm::vec3 lightColor(1.0f);
		static glm::vec3 clearColor(0.0f);
		static float ambientStrength = 0.1f;
		static float specularStrength = 1.0f;
		static float shininess = 2.0f;
		static bool blinn = true;
		static float gamma = 2.2f;
		{
			ImGui::Text("Use WASD to move, SPACE to rise, LEFT SHIFT to fall.");
			ImGui::SliderInt("number of objects", &numObjects, 1, 1000);
			ImGui::SliderFloat("object size", &objectScale, 0.00f, 0.5f);
			ImGui::SliderFloat("speed", &timeScale, 0.0f, 20.0f);
			ImGui::SliderFloat("object yaw scalar", &objectRotationScalar, 0.0f, 50.0f);
			ImGui::SliderFloat("object pitch scalar", &objectRotationScalar2, 0.0f, 50.0f);
			ImGui::SliderFloat("object radius scalar", &objectTranslationScalar, 0.0f, 10.0f);
			ImGui::SliderFloat3("main rotation axis", (float*)&rotationAxis, -1.0f, 1.0f);
			ImGui::ColorEdit3("object color 1", (float*)&color1);
			ImGui::ColorEdit3("object color 2", (float*)&color2);
			ImGui::ColorEdit3("light color", (float*)&lightColor);
			ImGui::ColorEdit3("clear color", (float*)&clearColor);
			ImGui::SliderFloat("ambient strength", &ambientStrength, 0.0f, 1.0f);
			ImGui::SliderFloat("specular strength", &specularStrength, 0.0f, 5.0f);
			ImGui::SliderFloat("specular shininess", &shininess, 1.0f, 256.0f);
			ImGui::Checkbox("specular blinn", &blinn);
			ImGui::SliderFloat("gamma", &gamma, 1.0f, 2.2f);
			glm::mat4 viewMatrix = view->matrix;
			ImGui::Text("View matrix debug:");
			ImGui::Text("%.1f, %.1f, %.1f, %.1f", viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0], viewMatrix[3][0]);
			ImGui::Text("%.1f, %.1f, %.1f, %.1f", viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1], viewMatrix[3][1]);
			ImGui::Text("%.1f, %.1f, %.1f, %.1f", viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2], viewMatrix[3][2]);
			ImGui::Text("%.1f, %.1f, %.1f, %.1f", viewMatrix[0][3], viewMatrix[1][3], viewMatrix[2][3], viewMatrix[3][3]);
			ImGui::Text("%.1f, %.1f", sceneManager->screenSize.x, sceneManager->screenSize.y);
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

		// clear stuff
		glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Capture system audio, and keep track of the number of new samples
		static int newSamples = 0;
		newSamples += loopback_getSound(audioBuffer, numAudioSamples); 

		// Main audio processing loop. This runs every time there is enough new audio samples to process the next audio frame.
		static const FrequencySpectrum * frequencySpectrum = analyzer.getFrequencySpectrum();
		while (newSamples >= frameGap)
		{
			newSamples -= frameGap;

			// transfer audio samples into the fftInBuffer
			float * inputBuffer = analyzer.getFrameInputBuffer();
			int frameSize = analyzer.getFrameSize();
			int frameStart = frameSize - newSamples;
			for (int i = 0; i < frameSize; i++)
				inputBuffer[i] = audioBuffer[frameStart + i];

			analyzer.processFrame();
			frequencySpectrum = analyzer.getFrequencySpectrum();
			frequencySpectrum = amplitudeFilter.applyFilter(frequencySpectrum);
			frequencySpectrum = domainShiftFilter.applyFilter(frequencySpectrum);
			frequencySpectrum = peakFilter.applyFilter(frequencySpectrum);
			frequencySpectrum = averageFilter.applyFilter(frequencySpectrum);
		}
		float * frequencyData = frequencySpectrum->data;

		// get view and projection matrices
		updateView2(view, sceneManager);
		float aspect = sceneManager->screenSize.y != 0.0f ? (float)sceneManager->screenSize.x / (float)sceneManager->screenSize.y : 1.0f;
		glm::mat4 projection = glm::perspective(glm::radians(75.0f), aspect, 0.1f, 100.0f);

		// make light
		glBindVertexArray(lightVAO);
		lightShader.update();
		lightShader.use();
		lightShader.setVec3("lightColor", lightColor);
		lightShader.setMat4("view", view->invMatrix);
		lightShader.setMat4("projection", projection);
		glm::mat4 lightModel; 
		//lightModel = glm::rotate(lightModel, (float)time * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//lightModel = glm::translate(lightModel, glm::vec3(3.0f, 0.0f, 0.0f));
		lightModel = glm::scale(lightModel, glm::vec3(objectScale * 1.0f));
		lightShader.setMat4("model", lightModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		// make cubes
		glBindVertexArray(particleVAO);
		cubeShader.update();
		cubeShader.use();
		glm::vec3 gamma3(gamma);
		cubeShader.setVec3("color1", glm::pow(color1, gamma3));
		cubeShader.setVec3("color2", glm::pow(color2, gamma3));
		cubeShader.setVec3("lightColor", glm::pow(lightColor, gamma3));
		cubeShader.setVec3("lightPos", glm::vec3(lightModel[3]));
		cubeShader.setVec3("viewPos", view->position);
		cubeShader.setMat4("view", view->invMatrix);
		cubeShader.setMat4("projection", projection);
		cubeShader.setFloat("ambientStrength", ambientStrength);
		cubeShader.setFloat("specularStrength", specularStrength);
		cubeShader.setFloat("shininess", shininess);
		cubeShader.setBool("blinn", blinn);
		cubeShader.setFloat("gamma", gamma);
		float freqAccumulation = 0.0f;
		for (int i = 0; i < numObjects; i++)
		{
			float x = (float)i / (float)numObjects;
			float time = (float)glfwGetTime() * timeScale;
			float freq = utl::getValueLerp(frequencyData, numFreqBins, x);
			freqAccumulation += freq;

			glm::mat4 model;
			model = glm::rotate(model, (time + x * objectRotationScalar) * glm::radians(90.0f), rotationAxis);
			glm::vec3 axis2 = glm::cross((glm::vec3)model[0], rotationAxis);
			//axis2 = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), rotationAxis); <- what i actually intended to do, but looks less cool
			model = glm::rotate(model, (time + x * objectRotationScalar2) * glm::radians(90.0f), axis2);

			model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f) * sin(time + x * objectTranslationScalar));
			model = glm::scale(model, glm::vec3(objectScale + freq));

			cubeShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// end frame stuff
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(sceneManager->window);
		
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	_CrtDumpMemoryLeaks();
	glfwTerminate();
	return 0;
}

static void updateView(View * view, SceneManager * sceneManager)
{
	static const float CAMERA_SENSITIVITY = 0.1f;
	static const glm::vec3 CAMERA_SPEED(4.0f, 3.0f, 4.0f);

	// only update the view when the mouse is active
	if (glfwGetInputMode(sceneManager->window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
		return;

	// get movement axis
	glm::vec3 movementAxis(0.0f);
	if (glfwGetKey(sceneManager->window, GLFW_KEY_A) == GLFW_PRESS)
		movementAxis[0] -= 1.0f;
	if (glfwGetKey(sceneManager->window, GLFW_KEY_D) == GLFW_PRESS)
		movementAxis[0] += 1.0f;
	if (glfwGetKey(sceneManager->window, GLFW_KEY_W) == GLFW_PRESS)
		movementAxis[2] -= 1.0f;
	if (glfwGetKey(sceneManager->window, GLFW_KEY_S) == GLFW_PRESS)
		movementAxis[2] += 1.0f;
	if (glfwGetKey(sceneManager->window, GLFW_KEY_SPACE) == GLFW_PRESS)
		movementAxis[1] += 1.0f;
	if (glfwGetKey(sceneManager->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		movementAxis[1] -= 1.0f;
	movementAxis = glm::length(movementAxis) > 0.0f ? glm::normalize(movementAxis) : movementAxis;

	// constrain y axis rotation (yaw) to [0, 360] and x axis rotation (pitch) to [-90, 90]
	view->eulerAngles.y = glm::mod(view->eulerAngles.y - sceneManager->deltaMousePos.x * CAMERA_SENSITIVITY, 360.0f);
	view->eulerAngles.x = glm::clamp(view->eulerAngles.x - sceneManager->deltaMousePos.y * CAMERA_SENSITIVITY, -90.0f, 90.0f);

	// perform rotation around y axis (yaw) and rotation around x axis (pitch)
	glm::mat4 intermediate = glm::rotate(glm::mat4(1.0f), glm::radians(view->eulerAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
	view->matrix = glm::rotate(intermediate, glm::radians(view->eulerAngles.x), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::vec3 velocity(intermediate * glm::vec4(movementAxis, 0.0f));
	velocity *= CAMERA_SPEED;
	// perform translation and calculate inverse matrix
	view->position += velocity * sceneManager->deltaTime;
	view->matrix[3] = glm::vec4(view->position, 1.0f);
	view->invMatrix = glm::inverse(view->matrix);
}

static void updateView2(View * view, SceneManager * sceneManager)
{
	static const float CAMERA_SENSITIVITY = 0.1f;
	static const float CAMERA_MAX_VELOCITY = 4.0f;
	static const float CAMERA_ACCEL = CAMERA_MAX_VELOCITY / 0.05f;
	static const float CAMERA_DECCEL = CAMERA_MAX_VELOCITY / 0.05f;
	static glm::vec3 velocity(0.0f);

	// disable control when mouse is active
	glm::vec3 movementAxis(0.0f);
	if (glfwGetInputMode(sceneManager->window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
	{
		// get movement axis
		if (glfwGetKey(sceneManager->window, GLFW_KEY_A) == GLFW_PRESS)
			movementAxis[0] -= 1.0f;
		if (glfwGetKey(sceneManager->window, GLFW_KEY_D) == GLFW_PRESS)
			movementAxis[0] += 1.0f;
		if (glfwGetKey(sceneManager->window, GLFW_KEY_W) == GLFW_PRESS)
			movementAxis[2] -= 1.0f;
		if (glfwGetKey(sceneManager->window, GLFW_KEY_S) == GLFW_PRESS)
			movementAxis[2] += 1.0f;
		if (glfwGetKey(sceneManager->window, GLFW_KEY_SPACE) == GLFW_PRESS)
			movementAxis[1] += 1.0f;
		if (glfwGetKey(sceneManager->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			movementAxis[1] -= 1.0f;
		movementAxis = glm::length(movementAxis) > 0.0f ? glm::normalize(movementAxis) : movementAxis;

		// constrain y axis rotation (yaw) to [0, 360] and x axis rotation (pitch) to [-90, 90]
		view->eulerAngles.y = glm::mod(view->eulerAngles.y - sceneManager->deltaMousePos.x * CAMERA_SENSITIVITY, 360.0f);
		view->eulerAngles.x = glm::clamp(view->eulerAngles.x - sceneManager->deltaMousePos.y * CAMERA_SENSITIVITY, -90.0f, 90.0f);
	}

	// perform rotation around y axis (yaw) and rotation around x axis (pitch)
	glm::mat4 intermediate = glm::rotate(glm::mat4(1.0f), glm::radians(view->eulerAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
	view->matrix = glm::rotate(intermediate, glm::radians(view->eulerAngles.x), glm::vec3(1.0f, 0.0f, 0.0f));

	// calculate deccelerated velocity
	
	// decellerate when the movement axis is 0, accelerate otherwise
	float speed = glm::length(velocity);
	if (glm::length(movementAxis) == 0.0f && speed != 0.0f)
		velocity = glm::clamp(speed - CAMERA_DECCEL * sceneManager->deltaTime, 0.0f, CAMERA_MAX_VELOCITY) * glm::normalize(velocity);
	else
	{
		glm::vec3 accelDir(intermediate * glm::vec4(movementAxis, 0.0f));
		velocity += accelDir * CAMERA_ACCEL * sceneManager->deltaTime;
		if (speed > CAMERA_MAX_VELOCITY)
			velocity = glm::normalize(velocity) * CAMERA_MAX_VELOCITY;
	}
	
	// perform translation and calculate inverse matrix
	view->position += velocity * sceneManager->deltaTime;
	view->matrix[3] = glm::vec4(view->position, 1.0f);
	view->invMatrix = glm::inverse(view->matrix);
}
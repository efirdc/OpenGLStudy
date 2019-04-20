#include <algorithm>
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "utilities.h"

#include "glm/glm.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "imgui_extras.h"
#include "imgui_color_gradient.h"

#include "Shader.h"
#include "SceneManager.h"
#include "StreamTexture.h"
#include "PingPongBuffer.h"

#include "SpectrumAnalyzer.h"
#include "SpectrumFilter.h"
#include "loopback.h"

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

	PingPongBuffer fluidBuffer(fluidWidth, fluidHeight);
	const float fluidBorder[4] = { 0.5f, 0.5f, 0.5f, 0.0f };
	const float densityBorder[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	const float quantitiesBorder[4] = { 0.0f, 0.5f, 0.5f, 0.0f };
	fluidBuffer.addTextureChannel(GL_TEXTURE0, fluidBorder);
	fluidBuffer.addTextureChannel(GL_TEXTURE1, densityBorder);
	fluidBuffer.addTextureChannel(GL_TEXTURE4, quantitiesBorder);

	const int gradientSize = 256;

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
	ImVec2 fAmpControlPoints[2] = { { 0.016f, 0.016f },{ 0.0f, 1.0f } };
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

	StreamTexture1D * densityColorCurve = new StreamTexture1D(GL_RGB32F, gradientSize, GL_RGB, GL_FLOAT, 3, 4, false);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_1D, densityColorCurve->textureID);

	// initialize frequency color gradient
	ImGradient densityGradient;
	densityGradient.getMarks().clear();
	densityGradient.addMark(1.0f, ImColor(0xFF, 0xFF, 0xCC));
	densityGradient.addMark(0.75f, ImColor(0xFF, 0xEA, 0x00));
	densityGradient.addMark(0.50f, ImColor(0x8E, 0x00, 0x00));
	densityGradient.addMark(0.25f, ImColor(0x22, 0x1D, 0x24));
	densityGradient.addMark(0.0f, ImColor(0x00, 0x00, 0x00));
	glm::vec3 * densityColors = (glm::vec3 *)densityColorCurve->getPixelBuffer();
	int numColors = densityColorCurve->width;
	for (int i = 0; i < numColors; i++)
	{
		ImVec4 color = densityGradient.getColorAt((float)i / (float)(numColors - 1));
		densityColors[i] = glm::vec3(color.x, color.y, color.z);
	}
	densityColorCurve->unmapPixelBuffer();
	densityColorCurve->getPixelBuffer(); // bug workadound. StreamTexture doesnt initialize properly without this
	densityColorCurve->unmapPixelBuffer();

	StreamTexture1D * frequencyTexture = new StreamTexture1D(GL_R32F, numFreqBins, GL_RED, GL_FLOAT, 1, 4, true);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_1D, frequencyTexture->textureID);
	float * frequencyPixelBuffer = (float *)frequencyTexture->getPixelBuffer();
	for (int i = 0; i < frequencyTexture->width; i++)
		frequencyPixelBuffer[i] = 0.0f;
	
	Shader displayShader("shaders/fluid/screenQuad.vs", "shaders/fluid/display.fs");
	Shader advectShader("shaders/fluid/screenQuad.vs", "shaders/fluid/advection.fs");
	Shader audioSpiralShader("shaders/fluid/screenQuad.vs", "shaders/fluid/audioSpiral.fs");
	Shader splatShader("shaders/fluid/screenQuad.vs", "shaders/fluid/simpleSplat.fs");
	Shader divergenceShader("shaders/fluid/screenQuad.vs", "shaders/fluid/divergence.fs");
	Shader pressureShader("shaders/fluid/screenQuad.vs", "shaders/fluid/pressure.fs");
	Shader subtractPressureShader("shaders/fluid/screenQuad.vs", "shaders/fluid/subtractPressure.fs");
	Shader curlShader("shaders/fluid/screenQuad.vs", "shaders/fluid/curl.fs");
	Shader vorticityShader("shaders/fluid/screenQuad.vs", "shaders/fluid/vorticity.fs");

	// Main loop
	sceneManager->newFrame();
	while (!glfwWindowShouldClose(sceneManager->window))
	{
		// new frame
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();
		sceneManager->newFrame();

		// Settings window
		static int pressureIterations = 50;
		static float timestep = 1.0f;
		static int displayMode = 5;
		static float mouseSplatRadius = 50.0f;
		static float mouseForce = 1.0;
		static float velocityDissipation = 1.0f;
		static float densityDissipation = 0.970f;
		static float spiralCurl = 0.010f;
		static float spiralSpin = 1.4f;
		static float spiralSplatRadius = 0.0002f;
		static float spiralVelocityAddScalar = 5.0f;
		static float spiralPressureAddScalar = 1.0f;
		static float spiralDensityAddScalar = 0.8f;
		static float vorticity = 1.0f;
		ImGui::Begin("Settings");
		{
			const char * displayModes[] = { "All", "Velocity", "Pressure", "Divergence", "Density", "DensityColor", "Curl", "Vorticity" };
			ImGui::Combo("display mode", &displayMode, displayModes, IM_ARRAYSIZE(displayModes));
			ImGui::SliderInt("pressure iterations", &pressureIterations, 1, 200);
			ImGui::SliderFloat("timestep", &timestep, 0.01f, 5.0f);
			standardTimestep = timestep / 60.0f;
			ImGui::SliderFloat("mouse radius", &mouseSplatRadius, 1.0f, 150.0f);
			ImGui::SliderFloat("mouse force", &mouseForce, 0.01f, 5.0f);
			ImGui::SliderFloat("velocity dissipation", &velocityDissipation, 0.9f, 1.0f);
			ImGui::SliderFloat("density dissipation", &densityDissipation, 0.9f, 1.0f);
			ImGui::SliderFloat("curl", &spiralCurl, 0.0f, 0.2f);
			ImGui::SliderFloat("spin", &spiralSpin, 0.0f, 20.0f);
			ImGui::SliderFloat("splat radius", &spiralSplatRadius, 0.0f, 0.0005f, "%.5f");
			ImGui::SliderFloat("velocity add scalar", &spiralVelocityAddScalar, 0.0f, 15.0f);
			ImGui::SliderFloat("pressure add scalar", &spiralPressureAddScalar, 0.0f, 100.0f);
			ImGui::SliderFloat("density add scalar", &spiralDensityAddScalar, 0.0f, 15.0f);
			ImGui::SliderFloat("vorticity", &vorticity, 0.0f, 10.0f);

			// Control the frequency color gradient
			bool changed = false;
			if (ImGui::TreeNode("Frequency Color Gradient"))
			{
				changed = ImGui::GradientEditor(&densityGradient);
				ImGui::TreePop();
			}
			if (changed)
			{
				glm::vec3 * colors = (glm::vec3 *)densityColorCurve->getPixelBuffer();
				int numColors = densityColorCurve->width;
				for (int i = 0; i < numColors; i++)
				{
					ImVec4 color = densityGradient.getColorAt((float)i / (float)(numColors - 1));
					colors[i] = glm::vec3(color.x, color.y, color.z);
				}
				densityColorCurve->unmapPixelBuffer();
			}

			// Control the frequency amplitude curve
			changed = false;
			if (ImGui::TreeNode("Frequency Amplitude Curve"))
			{
				changed = ImGui::Bezier("", fAmpControlPoints);
				ImGui::TreePop();
			}
			if (changed)
			{
				utl::bezierTable((glm::vec2 *)fAmpControlPoints, frequencyAmplitudePoints, bezierCurveSize);
				utl::curve2Dto1D(frequencyAmplitudePoints, bezierCurveSize, frequencyAmplitudeCurve, bezierCurveSize);
				amplitudeFilter.setAmplitudeCurve(frequencyAmplitudeCurve, bezierCurveSize);
			}

			// Control the frequency peak curve
			if (ImGui::TreeNode("Frequency Peak Curve"))
			{
				changed = ImGui::SliderFloat("Blur Radius", &peakRadius, 0.0f, 0.1f);
				changed |= ImGui::Bezier("", peakControlPoints);
				ImGui::TreePop();
			}
			if (changed)
			{
				delete[] peakCurve;
				peakCurveSize = (int)((float)numFreqBins * peakRadius);
				peakCurve = new float[peakCurveSize]();
				utl::bezierTable((glm::vec2 *)peakControlPoints, peakCurvePoints, bezierCurveSize);
				utl::curve2Dto1D(peakCurvePoints, bezierCurveSize, peakCurve, peakCurveSize);
				peakFilter.setPeakCurve(peakCurve, peakCurveSize);
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

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_1D, densityColorCurve->textureID);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_1D, frequencyTexture->textureID);

		bool showDemoWindow = true;
		//ImGui::ShowDemoWindow(&showDemoWindow);

		// Audio processing step
		static int newSamples = 0;
		newSamples += loopback_getSound(audioBuffer, numAudioSamples);
		static const FrequencySpectrum * frequencySpectrum = averageFilter.getFrequencySpectrum();
		while (newSamples >= frameGap)
		{
			newSamples -= frameGap;
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
		float * frequencyPixelBuffer = (float *)frequencyTexture->getPixelBuffer();
		for (int i = 0; i < frequencySpectrum->size; i++)
			frequencyPixelBuffer[i] = frequencyData[i];
		frequencyTexture->unmapPixelBuffer();

		// update shaders
		audioSpiralShader.update();

		// Splat step
		fluidBuffer.bind();
		splatShader.use();
		splatShader.setInt("fluid", 0);
		splatShader.setInt("density", 1);
		splatShader.setFloat("timestep", sceneManager->deltaTime / standardTimestep);
		splatShader.setVec2("pixelSize", 1.0f / glm::vec2(fluidWidth, fluidHeight));
		glm::vec2 texCoordMousePos = sceneManager->mousePos / sceneManager->screenSize;
		texCoordMousePos.y = 1.0f - texCoordMousePos.y;
		glm::vec2 fluidMouse = texCoordMousePos * glm::vec2(fluidWidth, fluidHeight);
		splatShader.setVec2("mousePosition", fluidMouse);
		splatShader.setVec2("mouseDelta", sceneManager->deltaMousePos * glm::vec2(1.0f, -1.0f));
		splatShader.setFloat("mouseForce", mouseForce);
		splatShader.setFloat("radius", mouseSplatRadius);
		splatShader.setFloat("leftMouseDown", sceneManager->leftMouseDown ? 1.0f : 0.0f);
		splatShader.setFloat("rightMouseDown", sceneManager->rightMouseDown ? 1.0f : 0.0f);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		fluidBuffer.swapTextureChannel(0);
		fluidBuffer.swapTextureChannel(1);

		// Audio spiral step
		// Dont run for the first 100 or so frames since the frequency data is garbage for a bit for some reason...
		if (sceneManager->frameNumber > 100) 
		{
			fluidBuffer.bind();
			audioSpiralShader.use();
			audioSpiralShader.setInt("fluid", 0);
			audioSpiralShader.setInt("density", 1);
			audioSpiralShader.setInt("frequency", 3);
			audioSpiralShader.setFloat("timestep", sceneManager->deltaTime / standardTimestep);
			audioSpiralShader.setFloat("utime", sceneManager->time);
			audioSpiralShader.setVec2("pixelSize", 1.0f / glm::vec2(fluidWidth, fluidHeight));
			audioSpiralShader.setFloat("curl", spiralCurl);
			audioSpiralShader.setFloat("spin", spiralSpin);
			audioSpiralShader.setFloat("splatRadius", spiralSplatRadius);
			audioSpiralShader.setFloat("velocityAddScalar", spiralVelocityAddScalar);
			audioSpiralShader.setFloat("pressureAddScalar", spiralPressureAddScalar);
			audioSpiralShader.setFloat("densityAddScalar", spiralDensityAddScalar);
			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			fluidBuffer.swapTextureChannel(0);
			fluidBuffer.swapTextureChannel(1);
		}
		
		// Advection step
		fluidBuffer.bind();
		advectShader.use();
		advectShader.setInt("fluid", 0);
		advectShader.setInt("density", 1);
		advectShader.setFloat("timestep", sceneManager->deltaTime / standardTimestep);
		advectShader.setVec2("pixelSize", 1.0f / glm::vec2(fluidWidth, fluidHeight));
		advectShader.setFloat("velocityDissipation", velocityDissipation);
		advectShader.setFloat("densityDissipation", densityDissipation);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		fluidBuffer.swapTextureChannel(0);
		fluidBuffer.swapTextureChannel(1);

		// Curl step
		fluidBuffer.bind();
		curlShader.use();
		curlShader.setInt("fluid", 0);
		curlShader.setInt("curl", 4);
		curlShader.setFloat("timestep", sceneManager->deltaTime / standardTimestep);
		curlShader.setVec2("pixelSize", 1.0f / glm::vec2(fluidWidth, fluidHeight));
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		fluidBuffer.swapTextureChannel(2);

		// Vorticity step
		fluidBuffer.bind();
		vorticityShader.use();
		vorticityShader.setInt("fluid", 0);
		vorticityShader.setInt("curl", 4);
		vorticityShader.setFloat("timestep", sceneManager->deltaTime / standardTimestep);
		vorticityShader.setVec2("pixelSize", 1.0f / glm::vec2(fluidWidth, fluidHeight));
		vorticityShader.setFloat("vorticityScalar", vorticity);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		fluidBuffer.swapTextureChannel(0);
		fluidBuffer.swapTextureChannel(2);

		// Divergence step
		fluidBuffer.bind();
		divergenceShader.use();
		divergenceShader.setInt("fluid", 0);
		divergenceShader.setFloat("timestep", sceneManager->deltaTime / standardTimestep);
		divergenceShader.setVec2("pixelSize", 1.0f / glm::vec2(fluidWidth, fluidHeight));
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		fluidBuffer.swapTextureChannel(0);

		// Pressure step
		for (int i = 0; i < pressureIterations; i++) {
			fluidBuffer.bind();
			pressureShader.use();
			pressureShader.setInt("fluid", 0);
			pressureShader.setFloat("timestep", sceneManager->deltaTime / standardTimestep);
			pressureShader.setVec2("pixelSize", 1.0f / glm::vec2(fluidWidth, fluidHeight));
			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			fluidBuffer.swapTextureChannel(0);
		}

		// Subtract pressure step
		fluidBuffer.bind();
		subtractPressureShader.use();
		subtractPressureShader.setInt("fluid", 0);
		subtractPressureShader.setFloat("timestep", sceneManager->deltaTime / standardTimestep);
		subtractPressureShader.setVec2("pixelSize", 1.0f / glm::vec2(fluidWidth, fluidHeight));
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		fluidBuffer.swapTextureChannel(0);

		// Use the default framebuffer
		sceneManager->sizeFramebufferToWindow();
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Display final texture on the default framebuffer
		fluidBuffer.bindTextures();
		displayShader.use();
		displayShader.setInt("fluid", 0);
		displayShader.setInt("density", 1);
		displayShader.setInt("curl", 4);
		displayShader.setInt("densityColorCurve", 2);
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
#include <algorithm>
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glDebug.h"

#include "utilities.h"

#include "glm/glm.hpp"

#include "nlohmann/json.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "imgui_extras.h"
#include "imgui_color_gradient.h"
#include "imgui_bezier_curve.h"

#include "Shader.h"
#include "SceneManager.h"
#include "StreamTexture.h"
#include "PingPongBuffer.h"

#include "SpectrumAnalyzer.h"
#include "SpectrumFilter.h"
#include "loopback.h"

using json = nlohmann::json;

struct Settings 
{
	int pressureIterations = 50;
	float timestep = 1.0f;
	float standardTimestep = timestep / 60.0f;
	int displayMode = 5;
	float velocityDissipation = 1.0f;
	float densityDissipation = 0.970f;
	bool vorticityEnabled = true;
	float vorticity = 1.0f;
	float mouseSplatRadius = 25.0f;
	float mouseVelocityAddScalar = 0.75f;
	float mousePressureAddScalar = 1.0f;
	float mouseDensityAddScalar = 0.4f;
	bool spiralEnabled = true;
	float spiralCurl = 0.010f;
	float spiralSpin = 1.4f;
	float spiralSplatRadius = 0.0002f;
	float spiralVelocityAddScalar = 5.0f;
	float spiralPressureAddScalar = 1.0f;
	float spiralDensityAddScalar = 0.8f;
};

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
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

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

	glDebug_init();

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
	const int fluidWidth = 640;
	const int fluidHeight = 360;
	const int gradientSize = 256;
	const int frameSize = 4096;
	const int numSpectrumsInAverage = 18;
	const int numFreqBins = 1024;
	const float domainShiftFactor = 10.0f;

	PingPongBuffer fluidBuffer(fluidWidth, fluidHeight);
	const float borderValues[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	fluidBuffer.addTextureChannel(GL_TEXTURE0, borderValues);
	fluidBuffer.addTextureChannel(GL_TEXTURE1, borderValues);
	fluidBuffer.addTextureChannel(GL_TEXTURE4, borderValues);

	SpectrumAnalyzer analyzer(frameSize);

	loopback_init();
	int frameGap = 128;
	int numAudioSamples = analyzer.getFrameSize() * 2;
	const int bezierCurveSize = 4096;
	float * audioBuffer = new float[numAudioSamples]();

	// initialize frequency amplitude curve
	ImVec2 fAmpControlPoints[2] = { { 0.016f, 0.016f },{ 0.0f, 1.0f } };
	ImBezierCurve frequencyAmplitudeCurve(fAmpControlPoints, bezierCurveSize, bezierCurveSize);

	// initialize peak smoothing curve
	float peakRadius = 0.05f;
	int peakCurveSize = (int)((float)numFreqBins * peakRadius);
	ImVec2 peakControlPoints[2] = { { 1.00f, 0.00f },{ 0.0f, 1.00f } };
	ImBezierCurve peakShapingCurve(peakControlPoints, peakCurveSize, bezierCurveSize);

	AmplitudeFilter amplitudeFilter(frequencyAmplitudeCurve.curve1D, frequencyAmplitudeCurve.curve1DSize);
	DomainShiftFilter domainShiftFilter(domainShiftFactor, numFreqBins);
	PeakFilter peakFilter(peakShapingCurve.curve1D, peakShapingCurve.curve1DSize);
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

	json j;
	Settings settings;

	// Main loop
	sceneManager->newFrame();
	while (!glfwWindowShouldClose(sceneManager->window))
	{
		// new frame
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();
		sceneManager->newFrame();

		// Settings window
		
		ImGui::Begin("Settings");
		{
			ImGui::PushItemWidth(-150);

			const char * displayModes[] = { "All", "Velocity", "Pressure", "Divergence", "Density", "DensityColor", "Curl", "Vorticity" };
			ImGui::Combo("display mode", &settings.displayMode, displayModes, IM_ARRAYSIZE(displayModes));
			ImGui::SliderInt("pressure iterations", &settings.pressureIterations, 1, 200);
			ImGui::SliderFloat("timestep", &settings.timestep, 0.01f, 5.0f);
			settings.standardTimestep = settings.timestep / 60.0f;
			ImGui::SliderFloat("velocity dissipation", &settings.velocityDissipation, 0.9f, 1.0f);
			ImGui::SliderFloat("density dissipation", &settings.densityDissipation, 0.9f, 1.0f);
			ImGui::Checkbox("vorticity enabled", &settings.vorticityEnabled);
			ImGui::SliderFloat("vorticity", &settings.vorticity, 0.0f, 10.0f);

			if (ImGui::TreeNode("Mouse Settings"))
			{
				ImGui::SliderFloat("radius", &settings.mouseSplatRadius, 1.0f, 150.0f);
				ImGui::SliderFloat("velocity add scalar", &settings.mouseVelocityAddScalar, 0.00f, 15.0f);
				ImGui::SliderFloat("pressure add scalar", &settings.mousePressureAddScalar, 0.00f, 100.0f);
				ImGui::SliderFloat("density add scalar", &settings.mouseDensityAddScalar, 0.0f, 15.0f);
				ImGui::TreePop();
			}
			
			if (ImGui::TreeNode("Audio Visualiser Spiral"))
			{
				ImGui::Checkbox("enabled", &settings.spiralEnabled);
				ImGui::SliderFloat("curl", &settings.spiralCurl, 0.0f, 0.2f);
				ImGui::SliderFloat("spin", &settings.spiralSpin, 0.0f, 20.0f);
				ImGui::SliderFloat("splat radius", &settings.spiralSplatRadius, 0.0f, 0.0005f, "%.5f");
				ImGui::SliderFloat("velocity add scalar", &settings.spiralVelocityAddScalar, 0.0f, 15.0f);
				ImGui::SliderFloat("pressure add scalar", &settings.spiralPressureAddScalar, 0.0f, 100.0f);
				ImGui::SliderFloat("density add scalar", &settings.spiralDensityAddScalar, 0.0f, 15.0f);
				ImGui::TreePop();
			}

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
				densityColorCurve->getPixelBuffer();
				densityColorCurve->unmapPixelBuffer();
			}

			// Control the frequency amplitude curve
			changed = false;
			if (ImGui::TreeNode("Frequency Amplitude Curve"))
			{
				changed = ImGui::Bezier("", frequencyAmplitudeCurve.controlPoints);
				ImGui::TreePop();
			}
			if (changed)
			{
				frequencyAmplitudeCurve.recalculate();
				amplitudeFilter.setAmplitudeCurve(frequencyAmplitudeCurve.curve1D, frequencyAmplitudeCurve.curve1DSize);
			}

			// Control the frequency peak curve
			if (ImGui::TreeNode("Frequency Peak Curve"))
			{
				changed = ImGui::SliderFloat("Blur Radius", &peakRadius, 0.0f, 0.1f);
				changed |= ImGui::Bezier("", peakShapingCurve.controlPoints);
				ImGui::TreePop();
			}
			if (changed)
			{
				peakCurveSize = (int)((float)numFreqBins * peakRadius);
				peakShapingCurve.curve1DSize = peakCurveSize;
				peakShapingCurve.recalculate();
				peakFilter.setPeakCurve(peakShapingCurve.curve1D, peakShapingCurve.curve1DSize);
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
		ImGui::ShowDemoWindow(&showDemoWindow);

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
		splatShader.setUniform("fluid", 0);
		splatShader.setUniform("density", 1);
		splatShader.setUniform("pixelSize", 1.0f / glm::vec2(fluidWidth, fluidHeight));
		glm::vec2 texCoordMousePos = sceneManager->mousePos / sceneManager->screenSize;
		texCoordMousePos.y = 1.0f - texCoordMousePos.y;
		glm::vec2 fluidMouse = texCoordMousePos * glm::vec2(fluidWidth, fluidHeight);
		splatShader.setUniform("mousePosition", fluidMouse);
		splatShader.setUniform("mouseDelta", sceneManager->deltaMousePos * glm::vec2(1.0f, -1.0f));
		splatShader.setUniform("velocityAddScalar", settings.mouseVelocityAddScalar);
		splatShader.setUniform("pressureAddScalar", settings.mousePressureAddScalar);
		splatShader.setUniform("densityAddScalar", settings.mouseDensityAddScalar);
		splatShader.setUniform("radius", settings.mouseSplatRadius);
		splatShader.setUniform("leftMouseDown", sceneManager->leftMouseDown ? 1.0f : 0.0f);
		splatShader.setUniform("rightMouseDown", sceneManager->rightMouseDown ? 1.0f : 0.0f);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		fluidBuffer.swapTextureChannel(0);
		fluidBuffer.swapTextureChannel(1);

		// Audio spiral step
		// Dont run for the first 100 or so frames since the frequency data is garbage for a bit for some reason...
		if (sceneManager->frameNumber > 100 && settings.spiralEnabled)
		{
			fluidBuffer.bind();
			audioSpiralShader.use();
			audioSpiralShader.setUniform("fluid", 0);
			audioSpiralShader.setUniform("density", 1);
			audioSpiralShader.setUniform("frequency", 3);
			audioSpiralShader.setUniform("timestep", sceneManager->deltaTime / settings.standardTimestep);
			audioSpiralShader.setUniform("utime", sceneManager->time);
			audioSpiralShader.setUniform("curl", settings.spiralCurl);
			audioSpiralShader.setUniform("spin", settings.spiralSpin);
			audioSpiralShader.setUniform("splatRadius", settings.spiralSplatRadius);
			audioSpiralShader.setUniform("velocityAddScalar", settings.spiralVelocityAddScalar);
			audioSpiralShader.setUniform("pressureAddScalar", settings.spiralPressureAddScalar);
			audioSpiralShader.setUniform("densityAddScalar", settings.spiralDensityAddScalar);
			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			fluidBuffer.swapTextureChannel(0);
			fluidBuffer.swapTextureChannel(1);
		}
		
		// Advection step
		fluidBuffer.bind();
		advectShader.use();
		advectShader.setUniform("fluid", 0);
		advectShader.setUniform("density", 1);
		advectShader.setUniform("timestep", sceneManager->deltaTime / settings.standardTimestep);
		advectShader.setUniform("pixelSize", 1.0f / glm::vec2(fluidWidth, fluidHeight));
		advectShader.setUniform("velocityDissipation", settings.velocityDissipation);
		advectShader.setUniform("densityDissipation", settings.densityDissipation);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		fluidBuffer.swapTextureChannel(0);
		fluidBuffer.swapTextureChannel(1);

		if (settings.vorticityEnabled)
		{
			// Curl step
			fluidBuffer.bind();
			curlShader.use();
			curlShader.setUniform("fluid", 0);
			curlShader.setUniform("pixelSize", 1.0f / glm::vec2(fluidWidth, fluidHeight));
			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			fluidBuffer.swapTextureChannel(2);

			// Vorticity step
			fluidBuffer.bind();
			vorticityShader.use();
			vorticityShader.setUniform("fluid", 0);
			vorticityShader.setUniform("curl", 4);
			vorticityShader.setUniform("timestep", sceneManager->deltaTime / settings.standardTimestep);
			vorticityShader.setUniform("pixelSize", 1.0f / glm::vec2(fluidWidth, fluidHeight));
			vorticityShader.setUniform("vorticityScalar", settings.vorticity);
			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			fluidBuffer.swapTextureChannel(0);
			fluidBuffer.swapTextureChannel(2);
		}
		
		// Divergence step
		fluidBuffer.bind();
		divergenceShader.use();
		divergenceShader.setUniform("fluid", 0);
		divergenceShader.setUniform("pixelSize", 1.0f / glm::vec2(fluidWidth, fluidHeight));
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		fluidBuffer.swapTextureChannel(0);

		// Pressure step
		for (int i = 0; i < settings.pressureIterations; i++) 
		{
			fluidBuffer.bind();
			pressureShader.use();
			pressureShader.setUniform("fluid", 0);
			pressureShader.setUniform("pixelSize", 1.0f / glm::vec2(fluidWidth, fluidHeight));
			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			fluidBuffer.swapTextureChannel(0);
		}

		// Subtract pressure step
		fluidBuffer.bind();
		subtractPressureShader.use();
		subtractPressureShader.setUniform("fluid", 0);
		subtractPressureShader.setUniform("pixelSize", 1.0f / glm::vec2(fluidWidth, fluidHeight));
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
		displayShader.setUniform("fluid", 0);
		displayShader.setUniform("density", 1);
		displayShader.setUniform("curl", 4);
		displayShader.setUniform("densityColorCurve", 2);
		displayShader.setUniform("displayMode", settings.displayMode);
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
#include <iostream>
#include <algorithm>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "utilities.h"

#include "glm/glm.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "imgui_extras.h"
#include "imgui_color_gradient.h"

#include "Shader.h"
#include "StreamTexture.h"
#include "SceneManager.h"

#include "loopback.h"
#include "SpectrumAnalyzer.h"
#include "SpectrumFilter.h"

int audioVisualizer()
{
	// Struct to hold scene data
	SceneManager * sceneManager = new SceneManager;
	sceneManager->captureMouse = false;

	// Initialize GLFW, set version to 3.3, tell OpenGL that we want to use the core profile
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window object and error check, make the window the curent context, then bind the window resize callback function
	sceneManager->window = glfwCreateWindow(1600, 900, "Audio Visualizer", NULL, NULL);
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

	// Make shader
	Shader audioShader("shaders/audioVisualizer.vs", "shaders/audioVisualizer.fs");

	// Set up data
	float vertices[] = {
		// positions         // texture coords
		1.0f,  1.0f,  0.0f,   1.0f, 1.0f, // top right
		1.0f, -1.0f,  0.0f,   1.0f, 0.0f, // bottom right
	   -1.0f, -1.0f,  0.0f,   0.0f, 0.0f, // bottom left
	   -1.0f,  1.0f,  0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	// Generate VAO, VBO, and EBO
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind the VAO, VBO and EBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Set the position and texture vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	
	// Setup SpectrumAnalyzer and set default filter settings
	int numSpectrumsInAverage = 18;
	int numFreqBins = 1024;
	float domainShiftFactor = 10.0f;
	SpectrumAnalyzer analyzer = SpectrumAnalyzer(4096);

	// Constant sizes
	const int bezierCurveSize = 4096;
	const int gradientSize = 256;
	const int soundTextureSize = 1024;

	// initialize stream textures
	StreamTexture1D * soundTexture = new StreamTexture1D(GL_R32F, soundTextureSize, GL_RED, GL_FLOAT, 1, 4, true);
	StreamTexture1D * frequencyTexture = new StreamTexture1D(GL_R32F, numFreqBins, GL_RED, GL_FLOAT, 1, 4, true);
	StreamTexture1D * frequencyColorCurve = new StreamTexture1D(GL_RGB32F, gradientSize, GL_RGB, GL_FLOAT, 3, 4, false);
	StreamTexture1D * lightColorCurve = new StreamTexture1D(GL_RGB32F, gradientSize, GL_RGB, GL_FLOAT, 3, 4, false);

	// initialize frequency amplitude curve
	float * frequencyAmplitudeCurve = new float[bezierCurveSize]();
	ImVec2 fAmpControlPoints[2] = { { 0.01f, 0.01f },{ 0.0f, 0.75f } };
	glm::vec2 frequencyAmplitudePoints[bezierCurveSize];
	utl::bezierTable((glm::vec2 *)fAmpControlPoints, frequencyAmplitudePoints, bezierCurveSize);
	utl::curve2Dto1D(frequencyAmplitudePoints, bezierCurveSize, frequencyAmplitudeCurve, bezierCurveSize);

	// initialize peak smoothing curve
	float peakRadius = 0.04f;
	int peakCurveSize = (int)((float)numFreqBins * peakRadius);
	float * peakCurve = new float[peakCurveSize]();
	ImVec2 peakControlPoints[2] = { { 1.00f, 0.00f },{ 0.35f, 1.00f } };
	glm::vec2 peakCurvePoints[bezierCurveSize];
	utl::bezierTable((glm::vec2 *)peakControlPoints, peakCurvePoints, bezierCurveSize);
	utl::curve2Dto1D(peakCurvePoints, bezierCurveSize, peakCurve, peakCurveSize);

	// Initialize spectrum filters
	AmplitudeFilter amplitudeFilter(frequencyAmplitudeCurve, bezierCurveSize);
	DomainShiftFilter domainShiftFilter(domainShiftFactor, numFreqBins);
	PeakFilter peakFilter(peakCurve, peakCurveSize);
	AverageFilter averageFilter(numSpectrumsInAverage);

	// Setup loopback audio capture
	loopback_init();
	int frameGap = 128;
	int numAudioSamples = analyzer.getFrameSize() * 2;
	float * audioBuffer = new float[numAudioSamples]();

	// initialize frequency color gradient
	ImGradient frequencyGradient;
	frequencyGradient.getMarks().clear();
	frequencyGradient.addMark(0.0f, ImColor(0x0E, 0x0E, 0x34));
	frequencyGradient.addMark(0.25f, ImColor(0xFF, 0x26, 0x26));
	frequencyGradient.addMark(0.5f, ImColor(0xEE, 0xBF, 0x1B));
	frequencyGradient.addMark(1.0f, ImColor(0xFF, 0xF1, 0xAD));
	glm::vec3 * freqColors = (glm::vec3 *)frequencyColorCurve->getPixelBuffer();
	int numColors = frequencyColorCurve->width;
	for (int i = 0; i < numColors; i++)
	{
		ImVec4 color = frequencyGradient.getColorAt((float)i / (float)(numColors - 1));
		freqColors[i] = glm::vec3(color.x, color.y, color.z);
	}
	frequencyColorCurve->unmapPixelBuffer();
	frequencyColorCurve->getPixelBuffer(); // bug workadound. StreamTexture doesnt initialize properly without this
	frequencyColorCurve->unmapPixelBuffer();

	// initialize light gradient
	ImGradient lightGradient;
	lightGradient.getMarks().clear();
	lightGradient.addMark(0.0f, ImColor(0x00, 0x00, 0x00));
	lightGradient.addMark(0.8f, ImColor(0x33, 0x33, 0x33));
	lightGradient.addMark(1.0f, ImColor(0xFF, 0xFF, 0xFF));
	glm::vec3 * lightColors = (glm::vec3 *)lightColorCurve->getPixelBuffer();
	numColors = lightColorCurve->width;
	for (int i = 0; i < numColors; i++)
	{
		ImVec4 color = lightGradient.getColorAt((float)i / (float)(numColors - 1));
		lightColors[i] = glm::vec3(color.x, color.y, color.z);
	}
	lightColorCurve->unmapPixelBuffer();
	lightColorCurve->getPixelBuffer();
	lightColorCurve->unmapPixelBuffer();

	// tell opengl for each sampler to which texture unit it belongs to
	audioShader.use();
	audioShader.setInt("soundTexture", soundTexture->textureID);
	audioShader.setInt("frequencyTexture", frequencyTexture->textureID);
	audioShader.setInt("frequencyColorCurve", frequencyColorCurve->textureID);
	audioShader.setInt("lightColorCurve", lightColorCurve->textureID);

	// Main loop
	sceneManager->newFrame();
	while (!glfwWindowShouldClose(sceneManager->window))
	{
		// new frame
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();
		sceneManager->newFrame();

		// Debug window
		ImGui::Begin("Settings");
		static float lightHeight = 0.0f;
		{
			ImGui::PushItemWidth(-190);

			// Control audio frame size size with arrow buttons
			int ival = analyzer.getFrameSize();
			if (ImGui::expArrowButtons("audio frame size: %d", &ival, 2, 65536))
			{
				analyzer.setFrameSize(ival);
				numAudioSamples = ival * 2;
				delete[] audioBuffer;
				audioBuffer = new float[numAudioSamples]();
			}
			ImGui::SameLine(); ImGui::ShowHelpMarker("Number of audio samples used in the fourier transform.\nHigher values have more frequency information, but less time information.");

			// Control audio frame rate with arrow buttons
			int audioFPS = loopback_samplesPerSec() / frameGap;
			if (ImGui::SliderInt("audio frames per sec", &audioFPS, 20, 600))
			{
				audioFPS = utl::clamp(audioFPS, 1, 3000);
				frameGap = loopback_samplesPerSec() / audioFPS;
			}
			ImGui::SameLine(); ImGui::ShowHelpMarker("Controls how many fourier transforms happen per second.\nThis is essentially the framerate of the frequency spectrum.");

			// Control number of audio frames with a slider
			ival = averageFilter.getNumSpectrumsInAverage();
			if (ImGui::SliderInt("audio frames used", &ival, 1, 40))
				averageFilter.setNumSpectrumsInAverage(utl::clamp(ival, 1, 200));
			ImGui::SameLine(); ImGui::ShowHelpMarker("The final displayed frequency spectrum is an average of this many spectrums.\nRaise to increase smoothness.");

			// Display the frame gap and total audio time.
			int totalSamples = analyzer.getFrameSize() + frameGap * averageFilter.getNumSpectrumsInAverage();
			ImGui::Text("time of utilized audio: %.3f sec", (float)totalSamples / (float)loopback_samplesPerSec());

			// Control domain shift with a slider
			float fval = domainShiftFilter.getDomainShiftFactor();
			ImGui::SliderFloat("log domain shift factor", &fval, 1.0f, 10.0f);
			domainShiftFilter.setDomainShiftFactor(fval);
			ImGui::SameLine(); ImGui::ShowHelpMarker("Shifts frequency domain onto a logarithmic scale.\
				\n1.0: all frequency bins are spaced evenly.\
				\n10.0: frequency bins are spaced at powers of 10\
				\nIt looks good to use values greater than 10.0 with very large frame sizes. CTRL + CLICK the slider to enter a value manually.");

			// Control light height with a slider
			ImGui::SliderFloat("light height", &lightHeight, 0.0f, 1.0f);

			// Control the frequency amplitude curve
			bool changed = false;
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

			// Control the frequency color gradient
			changed = false;
			if (ImGui::TreeNode("Frequency Color Gradient"))
			{
				changed = ImGui::GradientEditor(&frequencyGradient);
				ImGui::TreePop();
			}
			if (changed)
			{
				glm::vec3 * colors = (glm::vec3 *)frequencyColorCurve->getPixelBuffer();
				int numColors = frequencyColorCurve->width;
				for (int i = 0; i < numColors; i++)
				{
					ImVec4 color = frequencyGradient.getColorAt((float)i / (float)(numColors - 1));
					colors[i] = glm::vec3(color.x, color.y, color.z);
				}
				frequencyColorCurve->unmapPixelBuffer();
			}

			// Control the light color gradient
			changed = false;
			if (ImGui::TreeNode("Light Color Gradient"))
			{
				changed = ImGui::GradientEditor(&lightGradient);
				ImGui::TreePop();
			}
			if (changed)
			{
				glm::vec3 * colors = (glm::vec3 *)lightColorCurve->getPixelBuffer();
				int numColors = lightColorCurve->width;
				for (int i = 0; i < numColors; i++)
				{
					ImVec4 color = lightGradient.getColorAt((float)i / (float)(numColors - 1));
					colors[i] = glm::vec3(color.x, color.y, color.z);
				}
				lightColorCurve->unmapPixelBuffer();
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

		bool showDemoWindow = true;
		//ImGui::ShowDemoWindow(&showDemoWindow);

		// clear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Capture system audio, and keep track of the number of new samples
		static int newSamples = 0;
		newSamples += loopback_getSound(audioBuffer, numAudioSamples);

		// Main audio processing loop. This runs every time there is enough new audio samples to process the next audio frame.
		static const FrequencySpectrum * frequencySpectrum = averageFilter.getFrequencySpectrum();
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

		// Transfer audio samples from audio buffer into the soundTexture pixel buffer
		float * soundPixelBuffer = (float *)soundTexture->getPixelBuffer();
		for (int i = 0; i < soundTextureSize; i++)
		{
			float t = (float)i / (float)soundTextureSize;
			soundPixelBuffer[i] = utl::getValueLerp(audioBuffer, numAudioSamples, t);
		}
		soundTexture->unmapPixelBuffer();

		// transfer frequency data into the frequency texture pixel buffer
		float * frequencyPixelBuffer = (float *)frequencyTexture->getPixelBuffer();
		for (int i = 0; i < frequencySpectrum->size; i++)
			frequencyPixelBuffer[i] = frequencyData[i];
		frequencyTexture->unmapPixelBuffer();

		// Update the shader, use it, and set uniforms
		audioShader.update();
		audioShader.use();
		audioShader.setInt("soundTexture", soundTexture->textureID);
		audioShader.setInt("frequencyTexture", frequencyTexture->textureID);
		audioShader.setInt("frequencyColorCurve", frequencyColorCurve->textureID);
		audioShader.setInt("lightColorCurve", lightColorCurve->textureID);
		audioShader.setFloat("time", (float)glfwGetTime());
		audioShader.setVec2("texturePixelSize", 1.0f / (float)sceneManager->screenSize.x, 1.0f / (float)sceneManager->screenSize.y);
		glm::vec2 mouseTextureCoords = sceneManager->mousePos / sceneManager->screenSize;
		audioShader.setVec2("mousePos", mouseTextureCoords.x, 1.0f - mouseTextureCoords.y);
		audioShader.setFloat("lightHeight", lightHeight);

		// Bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_1D, soundTexture->textureID);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_1D, frequencyTexture->textureID);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_1D, frequencyColorCurve->textureID);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_1D, lightColorCurve->textureID);

		// Draw the texture
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//swap the buffers
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(sceneManager->window);
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}
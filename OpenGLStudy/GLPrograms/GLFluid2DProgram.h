#pragma once
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <random>

#include "GLProgram.h"

#include "Shader.h"

#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include "xtensor/xarray.hpp"
#include "xtensor/xnpy.hpp"
#include "stb/stb_image_write.h"
//#include <xtensor-io/xnpz.hpp>

#include "utilities.h"
#include "Texture.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "ColorGradientTexture.h"


class GLFluid2DProgram : public GLProgram
{
public:
	Shader advection;
	Shader divergence;
	Shader pressure;
	Shader subtractPressureGradient;
	Shader render;
	
	unsigned int quadVAO{};

	SlabTexture velocityTexture;
	SlabTexture pressureTexture;
	SlabTexture densityTexture;
	
	float timestep = 1.0;
	int numPressureIterations = 60;
	float velocityDissipation = 1.0f;
	float densityDissipation = 1.0f;

	int downSample = 3;
	glm::ivec2 simulationSize;

	glm::vec3 color1{ 0.0f, 0.0f, 0.0f };
	glm::vec3 color2{ 1.0f, 1.0f, 1.0f };

	glm::vec4 initialState{ 0.0f, 0.0f, 0.0f, 0.0f };

	unsigned int numCaptureFrames = 6;
	unsigned int captureFrame = numCaptureFrames;
	xt::xarray<float> captureArray{xt::xarray<float>::shape_type{1, 1, 1}};
	//ImGradient lightGradient;
	//ColorGradientTexture lightGradientTexture;

	GLFluid2DProgram() :
		GLProgram(4, 4, true,768, 768, "Fluid 2D", true),
		simulationSize(screenSize / downSample),
		velocityTexture{
			{ 0, GL_TEXTURE_2D, glm::ivec3(simulationSize.x, simulationSize.y, 0), GL_RGBA32F, GL_RG, GL_FLOAT,
				GL_LINEAR, GL_CLAMP_TO_BORDER, true, GL_WRITE_ONLY, {0.0, 0.0, 0.0, 0.0}}
		},
		pressureTexture{
				{ 1, GL_TEXTURE_2D, glm::ivec3(simulationSize.x, simulationSize.y, 0), GL_RG32F, GL_RG, GL_FLOAT,
					GL_LINEAR, GL_CLAMP_TO_BORDER, true, GL_WRITE_ONLY, {0.0, 0.0, 0.0, 0.0}}
		},
		densityTexture{
				{ 2, GL_TEXTURE_2D, glm::ivec3(simulationSize.x, simulationSize.y, 0), GL_R32F, GL_RED, GL_FLOAT,
				GL_LINEAR, GL_CLAMP_TO_BORDER, true, GL_WRITE_ONLY, {0.0, 0.0, 0.0, 0.0}}
		},
		render("shaders/fluid2D/render.vert", "shaders/fluid2D/render.frag"),
		advection(GL_COMPUTE_SHADER, "shaders/fluid2D/advection.comp"),
		divergence(GL_COMPUTE_SHADER, "shaders/fluid2D/divergence.comp"),
		pressure(GL_COMPUTE_SHADER, "shaders/fluid2D/pressure.comp"),
		subtractPressureGradient(GL_COMPUTE_SHADER, "shaders/fluid2D/subtractPressureGradient.comp")
	{
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

		glGenVertexArrays(1, &quadVAO);
		glBindVertexArray(quadVAO);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		resetTextures();

		bindGlobalUniforms();

		keyInputs[GLFW_KEY_S];
	}

	void resetTextures()
	{
		glm::ivec3 size = glm::ivec3(simulationSize.x, simulationSize.y, 0);
		velocityTexture.setSize(size);
		pressureTexture.setSize(size);
		densityTexture.setSize(size);
		velocityTexture.clearImage(initialState);
		pressureTexture.clearImage(initialState);
		densityTexture.clearImage(initialState);
	}

	void update() override
	{
		menu();

		if (screenSize / downSample != simulationSize)
		{
			simulationSize = screenSize / downSample;
			resetTextures();
		}

		if (captureFrame < numCaptureFrames)
		{
			int size = simulationSize.x * simulationSize.y;
			std::vector<float> captureBuffer(size);
			glGetTextureImage(densityTexture.source.ID, 0, densityTexture.defn.format, densityTexture.defn.type, size * sizeof(float), captureBuffer.data());
			for (int j = 0; j < simulationSize.y; j++)
			for (int i = 0; i < simulationSize.x; i++)
				captureArray(captureFrame, j, i) = captureBuffer[j * simulationSize.x + i];

			if (captureFrame == numCaptureFrames - 1)
			{
				int size = simulationSize.x * simulationSize.y;
				std::vector<glm::vec2> captureBuffer(size);
				glGetTextureImage(velocityTexture.source.ID, 0, velocityTexture.defn.format, velocityTexture.defn.type, size * sizeof(glm::vec2), captureBuffer.data());
				for (int j = 0; j < simulationSize.y; j++)
					for (int i = 0; i < simulationSize.x; i++)
					{
						captureArray(captureFrame + 1, j, i) = captureBuffer[j * simulationSize.x + i].x;
						captureArray(captureFrame + 2, j, i) = captureBuffer[j * simulationSize.x + i].y;
					}
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<> dis(1000000, INT_MAX);;
				std::string fileName = "fluid2D_dataset/snapshot_" + std::to_string(dis(gen)) + ".npy";
				xt::dump_npy(fileName, captureArray);

				
			}
			
			captureFrame++;
		}
		

		auto fluidStep = [this](auto& computeShader)
		{
			computeShader.update();
			computeShader.use();
			glDispatchCompute(simulationSize.x / 16 + 1, simulationSize.y / 16 + 1, 1);
			glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
		};

		fluidStep(advection);

		velocityTexture.swap();
		densityTexture.swap();
		//pressureTexture.swap();

		fluidStep(divergence);
		pressureTexture.swap();

		for (int i = 0; i < numPressureIterations; i++)
		{
			fluidStep(pressure);
			pressureTexture.swap();
		}

		fluidStep(subtractPressureGradient);
		velocityTexture.swap();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		const float aspect = screenSize.y != 0.0f ? (float)screenSize.x / (float)screenSize.y : 1.0f;

		render.update();
		render.use();
		render.setUniform("aspect", aspect);
		render.setUniform("renderMode", 0);

		if (keyInputs[GLFW_KEY_1].held)
			render.setUniform("renderMode", 1);
		else if (keyInputs[GLFW_KEY_2].held)
			render.setUniform("renderMode", 2);
		else if (keyInputs[GLFW_KEY_3].held)
			render.setUniform("renderMode", 3);

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void bindGlobalUniforms()
	{
		Shader::bindGlobalUniform("velocityImage", &velocityTexture.defn.textureUnit);
		Shader::bindGlobalUniform("velocitySampler", &velocityTexture.defn.textureUnit);

		Shader::bindGlobalUniform("pressureImage", &pressureTexture.defn.textureUnit);
		Shader::bindGlobalUniform("pressureSampler", &pressureTexture.defn.textureUnit);

		Shader::bindGlobalUniform("densityImage", &densityTexture.defn.textureUnit);
		Shader::bindGlobalUniform("densitySampler", &densityTexture.defn.textureUnit);

		Shader::bindGlobalUniform("time", &time);
		Shader::bindGlobalUniform("deltaTime", &deltaTime);
		Shader::bindGlobalUniform("screenSize", &screenSize);
		Shader::bindGlobalUniform("screenMousePos", &mousePos);
		Shader::bindGlobalUniform("prevScreenMousePos", &prevMousePos);
		Shader::bindGlobalUniform("leftMouseDown", &mouseInputs[GLFW_MOUSE_BUTTON_1].held);
		Shader::bindGlobalUniform("rightMouseDown", &mouseInputs[GLFW_MOUSE_BUTTON_2].held);
		Shader::bindGlobalUniform("mouseOnUI", &(imguiIO->WantCaptureMouse));

		Shader::bindGlobalUniform("timestep", &timestep);

		Shader::bindGlobalUniform("simulationSize", &simulationSize);
		Shader::bindGlobalUniform("velocityDissipation", &velocityDissipation);
		Shader::bindGlobalUniform("densityDissipation", &densityDissipation);

		Shader::bindGlobalUniform("color1", &color1);
		Shader::bindGlobalUniform("color2", &color2);

		//Shader::bindGlobalUniform("lightGradientSampler", &lightGradientTexture.textureUnit);
	}

	void menu()
	{
		ImGuiWindowFlags windowFlags = 0;
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
			windowFlags |= ImGuiWindowFlags_NoInputs;

		ImGui::Begin("Settings", nullptr, windowFlags);
		{

			if (ImGui::Button("reset fluid"))
				resetTextures();

			if (ImGui::Button("take snapshot") || keyInputs[GLFW_KEY_S].pressed)
			{
				captureFrame = 0;
				captureArray.resize({ numCaptureFrames + 2, (uint)simulationSize.y, (uint)simulationSize.x });
			}
			
			ImGui::SliderFloat("timestep", &timestep, 0.0f, 1.0f);
			ImGui::SliderInt("num pressure iterations", &numPressureIterations, 1, 100);
			ImGui::SliderFloat("velocity dissipation", &velocityDissipation, 0.9f, 1.0f);
			ImGui::SliderFloat("density dissipation", &densityDissipation, 0.9f, 1.0f);
			ImGui::SliderInt("downsample", &downSample, 1, 16);


			ImGui::ColorEdit3("color1", (float*)&color1);
			ImGui::ColorEdit3("color2", (float*)&color2);

			static int lastError = 0;
			const int currentError = glGetError();
			if (currentError)
				lastError = currentError;
			ImGui::Text("Current OpenGL Error Code: %d", currentError);
			ImGui::Text("Last OpenGL Error Code: %d", lastError);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			ImGui::Text("Left mouse down: %d Right mouse down: %d", mouseInputs[GLFW_MOUSE_BUTTON_1].held, mouseInputs[GLFW_MOUSE_BUTTON_2].held);

			ImGui::Text("Mouse pos x: %.3f, y: %.3f", mousePos.x, mousePos.y);
			glm::vec2 realMousePos = glm::vec2(mousePos.x, screenSize.y - mousePos.y) / glm::vec2(screenSize) * glm::vec2(simulationSize);
			ImGui::Text("Mouse splat pos %.3f, %.3f", realMousePos.x, realMousePos.y);

			//lightGradientTexture.Menu("Color Gradient", lightGradient);
		}
		ImGui::End();

		//bool showDemoWindow = false;
		//ImGui::ShowDemoWindow(&showDemoWindow);
	}
};

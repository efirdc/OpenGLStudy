#pragma once

#include <iostream>
#include <math.h>
#include <map>
#include <array>

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
#include "imgui_preset_menu.h"
#include "imgui_boost_serialization.h"
#include "glmSerialization.h"

#include "Shader.h"
#include "ComputeShader.h"
#include "SceneManager.h"
#include "StreamTexture.h"
#include "View.h"
#include "ColorGradientTexture.h"
#include "GLProgram.h"

#include "utilities.h"

#include <boost/serialization/version.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>



class Texture3D
{
public:
	unsigned int ID;
	unsigned int textureUnit;
	GLenum internalFormat;
	glm::vec3 size;
	GLenum format;
	GLenum filter;
	GLenum wrap;
	bool image;

	Texture3D(
		unsigned int textureUnit, GLenum internalFormat, glm::vec3 size, GLenum format, GLenum filter,
		GLenum wrap, bool image) :
		textureUnit(textureUnit), internalFormat(internalFormat), size(size), format(format), filter(filter),
		wrap(wrap), image(image)
	{
		glGenTextures(1, &ID);
		initialize();
	}

	void setSize(glm::vec3 newSize)
	{
		size = newSize;
		initialize();
	}

	void initialize()
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_3D, ID);
		glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, size.x, size.y, size.z, 0, format, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, filter);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, wrap);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, wrap);
		if (image)
			glBindImageTexture(textureUnit, ID, 0, GL_FALSE, 0, GL_READ_WRITE, internalFormat);
	}

	void bind()
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_3D, ID);
	}
};

class GLFeedbackProgram : public GLProgram {
public:
	struct FluidSplat {
		float radius, velocity, pressure, density;
		void bindGlobalUniforms(std::string uniformName) 
		{
			Shader::bindGlobalUniform(uniformName + ".radius", &radius);
			Shader::bindGlobalUniform(uniformName + ".velocity", &velocity);
			Shader::bindGlobalUniform(uniformName + ".pressure", &pressure);
			Shader::bindGlobalUniform(uniformName + ".density", &density);
		}
		void Menu(const char * name)
		{
			if (ImGui::TreeNode(name))
			{
				ImGui::SliderFloat("radius", &radius, 1.0f, 150.0f);
				ImGui::SliderFloat("velocity add scalar", &velocity, 0.00f, 15.0f);
				ImGui::SliderFloat("pressure add scalar", &pressure, 0.00f, 100.0f);
				ImGui::SliderFloat("density add scalar", &density, 0.0f, 15.0f);
				ImGui::TreePop();
			}
		}
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & radius & velocity & pressure & density;
		}
	};

	struct Settings {
		glm::ivec3 gridSize = { 64, 64, 64 };
		int numPressureIterations = 12;
		float velocityDissipation = 1.0f;
		float densityDissipation = 0.970f;
		bool vorticityEnabled = true;
		float vorticityScalar = 0.1f;
		float timeScale = 1.0f;
		FluidSplat mouseFluidSplat = { 25.0f, 0.75f, 1.0f, 0.4f };
		FluidSplat externalFluidSplat = { 8.0f, 30.0f, 1.0f, 0.1f };
		ImGradient fluidGradient;
		
		glm::vec3 backgroundColor{ 0.0f };
		float rayStepSize = 1.0f;
		float shadowStepSize = 4.0f;
		float maxShadowingLength = 12.0f;
		float maxDensity = 8.0f;
		float directionalLightExtinction = 20.0f;
		float ambientLightExtinction = 12.0f;

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			if (version > 0)
				ar & gridSize & backgroundColor & rayStepSize & shadowStepSize & maxShadowingLength
					& maxDensity & directionalLightExtinction & ambientLightExtinction;
			ar & numPressureIterations & velocityDissipation & densityDissipation & vorticityEnabled
				& vorticityScalar & timeScale & mouseFluidSplat & externalFluidSplat & fluidGradient;
		}
	} settings;
	
	View view;
	Shader raymarch;
	ComputeShader compute;
	ComputeShader advection, curl, vorticity, divergence, pressure, subtractPressureGradient;
	ImguiPresetMenu<Settings> presetMenu;
	ColorGradientTexture fluidGradientTexture;
	
	Texture3D velocityTexture{ 0, GL_RGBA32F, settings.gridSize, GL_RGB, GL_LINEAR, GL_CLAMP_TO_EDGE, true };
	//Texture3D pressureTexture{ 1, GL_R32F, settings.gridSize, GL_RED, GL_LINEAR, GL_CLAMP_TO_EDGE, true };
	Texture3D curlTexture{ 2, GL_RGBA32F, settings.gridSize, GL_RGBA, GL_LINEAR, GL_CLAMP_TO_EDGE, true };
	Texture3D densityTexture{ 3, GL_RGBA32F, settings.gridSize, GL_RGBA, GL_LINEAR, GL_CLAMP_TO_EDGE, true };
	
	unsigned int quadVAO{};

	GLFeedbackProgram() :
		GLProgram(4, 3, true, 1600, 900, "Fluid sim", true),
		raymarch("shaders/raymarch/raymarch.vert", "shaders/raymarch/feedback.frag"),
		compute("shaders/compute/feedback.comp"),
		advection("shaders/compute/advection.comp"),
		curl("shaders/compute/curl.comp"),
		vorticity("shaders/compute/vorticity.comp"),
		divergence("shaders/compute/divergence.comp"),
		pressure("shaders/compute/pressure.comp"),
		subtractPressureGradient("shaders/compute/subtractPressureGradient.comp"),
		presetMenu(settings, "fluidSettings.txt"),
		fluidGradientTexture(4, settings.fluidGradient)
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

		bindGlobalUniforms();
	}

	void initializeFluidTextures()
	{
		velocityTexture.setSize(settings.gridSize);
		//pressureTexture.setSize(settings.gridSize); 
		curlTexture.setSize(settings.gridSize);
		densityTexture.setSize(settings.gridSize);
	}

	void update() override
	{
		menu();

		compute.update();
		compute.use();
		glDispatchCompute(settings.gridSize.x / 8, settings.gridSize.y / 8, settings.gridSize.z / 8);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		
		/*
		auto fluidStep = [](auto& computeShader, glm::ivec3 & gridSize)
		{
			computeShader.update();
			computeShader.use();
			glDispatchCompute(gridSize.x / 8, gridSize.y / 8, gridSize.z / 8);
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		};
		fluidStep(advection, settings.gridSize);
		fluidStep(curl, settings.gridSize);
		fluidStep(vorticity, settings.gridSize);
		fluidStep(divergence, settings.gridSize);
		for (int i = 0; i < settings.numPressureIterations; i++)
			fluidStep(pressure, settings.gridSize);
		fluidStep(subtractPressureGradient, settings.gridSize);
		*/
		
		// clear stuff
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// get view and projection matrices
		view.updateView2(window, deltaMousePos, deltaTime);
		const float aspect = screenSize.y != 0.0f ? screenSize.x / screenSize.y : 1.0f;
		glm::mat4 projection = glm::perspective(glm::radians(75.0f), aspect, 0.1f, 100.0f);

		raymarch.update();
		raymarch.use();
		raymarch.setUniform("aspect", aspect);

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	
	void bindGlobalUniforms()
	{
		Shader::bindGlobalUniform("time", &time);
		Shader::bindGlobalUniform("deltaTime", &deltaTime);
		Shader::bindGlobalUniform("view", &(view.matrix));
		
		Shader::bindGlobalUniform("velocityImage", &velocityTexture.textureUnit);
		Shader::bindGlobalUniform("velocitySampler", &velocityTexture.textureUnit);
		//Shader::bindGlobalUniform("pressureImage", &pressureTexture.textureUnit);
		//Shader::bindGlobalUniform("pressureSampler", &pressureTexture.textureUnit);
		Shader::bindGlobalUniform("curlImage", &curlTexture.textureUnit);
		Shader::bindGlobalUniform("curlSampler", &curlTexture.textureUnit);
		Shader::bindGlobalUniform("densityImage", &densityTexture.textureUnit);
		Shader::bindGlobalUniform("densitySampler", &densityTexture.textureUnit);
		Shader::bindGlobalUniform("cloudColorCurve", &fluidGradientTexture.textureUnit);

		Shader::bindGlobalUniform("fluidSize", &settings.gridSize);
		Shader::bindGlobalUniform("numPressureIterations", &settings.numPressureIterations);
		Shader::bindGlobalUniform("velocityDissipation", &settings.velocityDissipation);
		Shader::bindGlobalUniform("densityDissipation", &settings.densityDissipation);
		Shader::bindGlobalUniform("vorticityScalar", &settings.vorticityScalar);
		Shader::bindGlobalUniform("timeScale", &settings.timeScale);

		Shader::bindGlobalUniform("backgroundColor", &settings.backgroundColor);
		Shader::bindGlobalUniform("rayStepSize", &settings.rayStepSize);
		Shader::bindGlobalUniform("shadowStepSize", &settings.shadowStepSize);
		Shader::bindGlobalUniform("maxShadowingLength", &settings.maxShadowingLength);
		Shader::bindGlobalUniform("maxDensity", &settings.maxDensity);
		Shader::bindGlobalUniform("directionalLightExtinction", &settings.directionalLightExtinction);
		Shader::bindGlobalUniform("ambientLightExtinction", &settings.ambientLightExtinction);

		settings.mouseFluidSplat.bindGlobalUniforms("mouseSplat");
		settings.externalFluidSplat.bindGlobalUniforms("externalSplat");
	}

	void menu()
	{

		ImGuiWindowFlags windowFlags = 0;
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
			windowFlags |= ImGuiWindowFlags_NoInputs;

		ImGui::Begin("Settings (Press CTRL to activate mouse)", nullptr, windowFlags);
		{
			ImGui::Text("Use WASD to move, SPACE to rise, LEFT SHIFT to fall.");
			const bool presetLoaded = presetMenu.Menu();
			if (presetLoaded)
			{
				fluidGradientTexture.upload(settings.fluidGradient);
				initializeFluidTextures();
			}
				
			if (ImGui::TreeNode("Fluid Behavior"))
			{
				static glm::ivec3 gridSizeTemp = settings.gridSize;
				ImGui::SliderInt3("fluid size", (int*)&gridSizeTemp, 16, 256);
				const glm::ivec3 localWorkGroupSize(16, 8, 8);
				gridSizeTemp = (gridSizeTemp / localWorkGroupSize) * localWorkGroupSize;
				if (ImGui::Button("Set Fluid Size"))
				{
					settings.gridSize = gridSizeTemp;
					initializeFluidTextures();
				}
				ImGui::SliderInt("pressure iterations", &settings.numPressureIterations, 1, 200);
				ImGui::SliderFloat("timeScale", &settings.timeScale, 0.01f, 5.0f);
				ImGui::SliderFloat("velocity dissipation", &settings.velocityDissipation, 0.9f, 1.0f);
				ImGui::SliderFloat("density dissipation", &settings.densityDissipation, 0.9f, 1.0f);
				ImGui::Checkbox("vorticity enabled", &settings.vorticityEnabled);
				ImGui::SliderFloat("vorticity", &settings.vorticityScalar, 0.0f, 1.0f);
				ImGui::TreePop();
			}
			settings.externalFluidSplat.Menu("External Forces");
			settings.mouseFluidSplat.Menu("Mouse Interaction");

			if (ImGui::TreeNode("Rendering"))
			{
				fluidGradientTexture.Menu("Color Gradient", settings.fluidGradient);
				ImGui::ColorEdit3("background color", (float*)&settings.backgroundColor);
				ImGui::SliderFloat("ray step size", &settings.rayStepSize, 0.1f, 4.0f);
				ImGui::SliderFloat("shadow step size", &settings.shadowStepSize, 0.1f, 4.0f);
				ImGui::SliderFloat("max shadowing length", &settings.maxShadowingLength, 1.0f, 256.0f);
				ImGui::SliderFloat("max density accumulation", &settings.maxDensity, 2.0f, 20.0f);
				ImGui::SliderFloat("directional light extinction", &settings.directionalLightExtinction, 1.0, 100.0f);
				ImGui::SliderFloat("ambient light extinction", &settings.ambientLightExtinction, 1.0f, 100.0f);
				ImGui::TreePop();
			}
			

			static int lastError = 0;
			const int currentError = glGetError();
			if (currentError)
				lastError = currentError;
			ImGui::Text("Current Opengl Error Code: %d", currentError);
			ImGui::Text("Last Opengl Error Code: %d", lastError);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
		ImGui::End();
	}
};

BOOST_CLASS_VERSION(GLFeedbackProgram::Settings, 1)



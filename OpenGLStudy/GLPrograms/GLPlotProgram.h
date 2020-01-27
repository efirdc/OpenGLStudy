#pragma once
#include "GLProgram.h"
#include "View.h"
#include "Shader.h"

#include "utilities.h"
#include "imGuIZMOquat.h"

class GLPlotProgram : public GLProgram
{
public:
	View view;
	Shader raymarch;
	unsigned int quadVAO{};

	GLPlotProgram() :
		GLProgram(4, 4, true, 1600, 900, "Plots!", true),
		raymarch("shaders/raymarch/raymarch.vert", "shaders/raymarch/plot_raymarch.frag")
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

	void update() override
	{
		menu();

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

	struct ScatteringData
	{
		glm::vec3 extinction;
		glm::vec3 scattering;
		float phaseScalar;
		void bindGlobalUniforms(std::string uniformName)
		{
			Shader::bindGlobalUniform(uniformName + ".extinction", &extinction);
			Shader::bindGlobalUniform(uniformName + ".scattering", &scattering);
			Shader::bindGlobalUniform(uniformName + ".phaseScalar", &phaseScalar);
		}
	};

	glm::vec3 backgroundColor{ 0.0f };
	float rayStepSize = 1.0f;
	float shadowStepSize = 4.0f;
	float maxShadowingLength = 60.0f;

	glm::vec3 scattering{ 1.0f, 2.0f, 4.0f };
	glm::vec3 absorption{ 0.75f, 0.5f, 0.0f };
	glm::vec3 directionalLightLuminance{ 1.0f, 1.0f, 1.0f };
	glm::vec3 directionalLightDirection{ -1.0, -1.0, -1.0 };

	int phaseMode = 2;
	int mieMode = 0;
	bool mieMultiLobe = true;
	float mieG1 = 0.8f;
	float mieG2 = 0.25f;
	float mieLobeMix = 0.5f;

	int multiScatteringOctaves = 3;
	float octaveExtinctionDecay = 0.5;
	float octaveScatteringDecay = 0.5;
	float octavePhaseDecay = 0.5;

	glm::vec3 boxMinimum{-64.0f, -64.0f, -64.0f};
	glm::vec3 boxMaximum{ 64.0f, 64.0f, 64.0f };
	float densityMean;
	float densityDistribution;
	
    #define MAX_SCATTERING_OCTAVES 8
	ScatteringData scatteringData[MAX_SCATTERING_OCTAVES];

	void initializeScatteringData()
	{
		scatteringData[0].extinction = absorption + scattering;
		scatteringData[0].scattering = scattering;
		scatteringData[0].phaseScalar = 1.0f;

		for (int i = 1; i < multiScatteringOctaves; i++)
		{
			scatteringData[i].extinction = scatteringData[0].extinction * glm::pow(octaveExtinctionDecay, i);
			scatteringData[i].scattering = scatteringData[0].scattering * glm::pow(octaveScatteringDecay, i);
			scatteringData[i].phaseScalar = scatteringData[0].phaseScalar * glm::pow(octavePhaseDecay, i);
		}
	}

	void bindGlobalUniforms()
	{
		Shader::bindGlobalUniform("time", &time);
		Shader::bindGlobalUniform("deltaTime", &deltaTime);
		Shader::bindGlobalUniform("view", &(view.matrix));

		Shader::bindGlobalUniform("screenMousePos", &mousePos);
		Shader::bindGlobalUniform("prevScreenMousePos", &prevMousePos);
	
		Shader::bindGlobalUniform("leftMouseDown", &mouseInputs[GLFW_MOUSE_BUTTON_1].held);
		Shader::bindGlobalUniform("rightMouseDown", &mouseInputs[GLFW_MOUSE_BUTTON_2].held);

		Shader::bindGlobalUniform("backgroundColor", &backgroundColor);
		Shader::bindGlobalUniform("rayStepSize", &rayStepSize);
		Shader::bindGlobalUniform("shadowStepSize", &shadowStepSize);
		Shader::bindGlobalUniform("maxShadowingLength", &maxShadowingLength);

		Shader::bindGlobalUniform("scattering", &scattering);
		Shader::bindGlobalUniform("absorption", &absorption);
		Shader::bindGlobalUniform("dirLight.color", &directionalLightLuminance);
		Shader::bindGlobalUniform("dirLight.direction", &directionalLightDirection);

		Shader::bindGlobalUniform("phaseMode", &phaseMode);
		Shader::bindGlobalUniform("mieMode", &mieMode);
		Shader::bindGlobalUniform("mieMultiLobe", &mieMultiLobe);
		Shader::bindGlobalUniform("mieG1", &mieG1);
		Shader::bindGlobalUniform("mieG2", &mieG2);
		Shader::bindGlobalUniform("mieLobeMix", &mieLobeMix);

		Shader::bindGlobalUniform("boxMinimum", &boxMinimum);
		Shader::bindGlobalUniform("boxMaximum", &boxMaximum);
		Shader::bindGlobalUniform("densityMean", &densityMean);
		Shader::bindGlobalUniform("densityDistribution", &densityDistribution);

		Shader::bindGlobalUniform("multiScatteringOctaves", &multiScatteringOctaves);
		for (int i = 0; i < MAX_SCATTERING_OCTAVES; i++)
			scatteringData[i].bindGlobalUniforms("scatteringData[" + std::to_string(i) + "]");
	}

	void menu()
	{
		ImGuiWindowFlags windowFlags = 0;
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
			windowFlags |= ImGuiWindowFlags_NoInputs;

		ImGui::Begin("Settings (Press CTRL to activate mouse)", nullptr, windowFlags);
		{
			ImGui::Text("Use WASD to move, SPACE to rise, LEFT SHIFT to fall.");
			if (ImGui::TreeNode("Rendering"))
			{
				ImGui::ColorEdit3("background color", (float*)&backgroundColor);

				ImGui::SliderFloat("ray step size", &rayStepSize, 0.1f, 4.0f);
				rayStepSize = utl::max(rayStepSize, 0.05f);
				ImGui::SliderFloat("shadow step size", &shadowStepSize, 0.1f, 4.0f);
				shadowStepSize = utl::max(shadowStepSize, 0.05f);
				ImGui::SliderFloat("max shadowing length", &maxShadowingLength, 1.0f, 512.0f);

				static glm::vec3 extinction = scattering + absorption;
				static glm::vec3 albedo = scattering / extinction;

				const int flags = ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR;
				bool changed = ImGui::ColorEdit3("absorption", (float*)&absorption, flags);
				changed |= ImGui::ColorEdit3("scattering", (float*)&scattering, flags);
				if (changed)
				{
					extinction = scattering + absorption;
					albedo = scattering / extinction;
					initializeScatteringData();
				}

				changed = ImGui::ColorEdit3("extinction", (float*)&extinction, flags);
				changed |= ImGui::ColorEdit3("albedo", (float*)&albedo, flags);
				if (changed)
				{
					scattering = albedo * extinction;
					absorption = extinction - scattering;
					initializeScatteringData();
				}

				ImGui::ColorEdit3("directional light luminance", (float*)&directionalLightLuminance, flags);
				ImGui::gizmo3D("light dir", directionalLightDirection);
				directionalLightDirection = glm::normalize(directionalLightDirection);

				const char* phaseModes[] = { "Isotropic", "Rayleigh", "Mie" };
				ImGui::Combo("phase mode", &phaseMode, phaseModes, IM_ARRAYSIZE(phaseModes));
				if (phaseMode == 2)
				{
					const char* mieModes[] = { "Henyey-Greenstein", "Schlick Approximation" };
					ImGui::Combo("mie mode", &mieMode, mieModes, IM_ARRAYSIZE(mieModes));
					bool changed = false;
					changed = ImGui::SliderInt("multi scattering octaves", &multiScatteringOctaves, 1, 8);
					multiScatteringOctaves = glm::clamp(multiScatteringOctaves, 1, 8);
					changed |= ImGui::SliderFloat("octave extinction decay", &octaveExtinctionDecay, 0.0f, 1.0f);
					changed |= ImGui::SliderFloat("octave scattering decay", &octaveScatteringDecay, 0.0f, 1.0f);
					changed |= ImGui::SliderFloat("octave phase decay", &octavePhaseDecay, 0.0f, 1.0f);
					if (changed)
						initializeScatteringData();
					ImGui::Checkbox("multi lobe", &mieMultiLobe);
					if (mieMultiLobe)
					{
						char str0[128] = "https://www.desmos.com/calculator/zdkjxh8msf";
						ImGui::InputText("multi lobe phase graph", str0, IM_ARRAYSIZE(str0));
					}
					ImGui::SliderFloat("mie g1", &mieG1, 0.0, 1.0);
					if (mieMultiLobe)
					{
						ImGui::SliderFloat("mie g2", &mieG2, 0.0, 1.0);
						ImGui::SliderFloat("mie lobe mix", &mieLobeMix, 0.0, 1.0);
					}
				}

				ImGui::SliderFloat("density mean", &densityMean, -20.0f, 20.0f);
				ImGui::SliderFloat("density distribution", &densityDistribution, 0.01f, 5.0f);

				ImGui::SliderFloat3("box maximum", (float *)&boxMaximum, 0.0f, 128.0f);
				ImGui::SliderFloat3("box minimum", (float*)&boxMinimum, -128.0f, 0.0f);
				ImGui::TreePop();
			}
			static int lastError = 0;
			const int currentError = glGetError();
			if (currentError)
				lastError = currentError;
			ImGui::Text("Current OpenGL Error Code: %d", currentError);
			ImGui::Text("Last OpenGL Error Code: %d", lastError);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			ImGui::Text("Left mouse down: %d Right mouse down: %d", mouseInputs[GLFW_MOUSE_BUTTON_1].held, mouseInputs[GLFW_MOUSE_BUTTON_2].held);
			ImGui::Text("Mouse pos x: %.3f, y: %.3f", mousePos.x, mousePos.y);
		}
		ImGui::End();

		//bool showDemoWindow = false;
		//ImGui::ShowDemoWindow(&showDemoWindow);
	}
};

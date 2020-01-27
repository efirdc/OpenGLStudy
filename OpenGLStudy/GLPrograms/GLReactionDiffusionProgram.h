#pragma once
#include "GLProgram.h"

#include "Shader.h"

#include "utilities.h"
#include "ComputeShader.h"
#include "Texture.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "ColorGradientTexture.h"

class GLReactionDiffusionProgram : public GLProgram
{
public:
	ComputeShader reactionDiffusion;
	Shader renderShader;
	unsigned int quadVAO{};

	SlabTexture slabTexture;

	glm::vec4 diffusionRates{ 1.0f, 0.5f, 0.0f, 0.0f };
	float feed = 0.055;
	float kill = 0.062;
	float timestep = 1.0;
	int iterations = 1;

	int downSample = 4;
	ivec2 simulationSize;

	glm::vec3 color1{ 0.0f, 0.0f, 0.0f };
	glm::vec3 color2{ 1.0f, 1.0f, 1.0f };

	glm::vec4 initialState{ 1.0f, 0.0f, 0.0f, 0.0f };

	ImGradient lightGradient;
	ColorGradientTexture lightGradientTexture;

	GLReactionDiffusionProgram() :
		GLProgram(4, 4, true, 1600, 900, "Reaction Diffusion", true),
		simulationSize(screenSize / downSample),
		slabTexture{
		{ 0, GL_TEXTURE_2D, glm::ivec3(simulationSize.x, simulationSize.y, 0), GL_RGBA32F, GL_RGBA, GL_FLOAT,
			GL_LINEAR, GL_REPEAT, true, GL_WRITE_ONLY, {0.0, 0.0, 0.0, 0.0}}
		},
		lightGradientTexture{1, lightGradient},
		reactionDiffusion("shaders/reaction_diffusion/reactionDiffusion.comp"),
		renderShader("shaders/reaction_diffusion/reactionDiffusion.vert", "shaders/reaction_diffusion/reactionDiffusion.frag")
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

		slabTexture.clearImage(initialState);

		bindGlobalUniforms();
	}

	void update() override
	{
		menu();

		if (screenSize / downSample != simulationSize)
		{
			simulationSize = screenSize / downSample;
			slabTexture.setSize(glm::ivec3(simulationSize.x, simulationSize.y, 0));
			slabTexture.clearImage(initialState);
		}

		slabTexture.bind();
		for (int i = 0; i < iterations; i++)
		{
			reactionDiffusion.update();
			reactionDiffusion.use();
			glDispatchCompute(simulationSize.x / 16 + 1, simulationSize.y / 16 + 1, 1);
			glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
			slabTexture.swap();
		}
		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		const float aspect = screenSize.y != 0.0f ? (float)screenSize.x / (float)screenSize.y : 1.0f;

		renderShader.update();
		renderShader.use();
		renderShader.setUniform("aspect", aspect);

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void bindGlobalUniforms()
	{
		Shader::bindGlobalUniform("rdImage", &slabTexture.defn.textureUnit);
		Shader::bindGlobalUniform("rdSampler", &slabTexture.defn.textureUnit);
		
		Shader::bindGlobalUniform("time", &time);
		Shader::bindGlobalUniform("deltaTime", &deltaTime);
		Shader::bindGlobalUniform("screenSize", &screenSize);
		Shader::bindGlobalUniform("screenMousePos", &mousePos);
		Shader::bindGlobalUniform("prevScreenMousePos", &prevMousePos);
		Shader::bindGlobalUniform("leftMouseDown", &mouseInputs[GLFW_MOUSE_BUTTON_1].held);
		Shader::bindGlobalUniform("rightMouseDown", &mouseInputs[GLFW_MOUSE_BUTTON_2].held);

		Shader::bindGlobalUniform("diffusionRates", &diffusionRates);
		Shader::bindGlobalUniform("feed", &feed);
		Shader::bindGlobalUniform("kill", &kill);
		Shader::bindGlobalUniform("timestep", &timestep);

		Shader::bindGlobalUniform("simulationSize", &simulationSize);

		Shader::bindGlobalUniform("color1", &color1);
		Shader::bindGlobalUniform("color2", &color2);

		Shader::bindGlobalUniform("lightGradientSampler", &lightGradientTexture.textureUnit);
	}

	void menu()
	{
		ImGuiWindowFlags windowFlags = 0;
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
			windowFlags |= ImGuiWindowFlags_NoInputs;

		ImGui::Begin("Settings", nullptr, windowFlags);
		{
			ImGui::SliderFloat4("diffusion rates", (float*)&diffusionRates, 0.0f, 1.0f);
			ImGui::SliderFloat("feed", &feed, 0.0f, 0.3f);
			ImGui::SliderFloat("kill", &kill, 0.0f, 0.3f);
			ImGui::SliderFloat("timestep", &timestep, 0.0f, 1.0f);
			ImGui::SliderInt("iterations", &iterations, 1, 100);

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

			ImGui::ColorEdit3("color1", (float*)&color1);
			ImGui::ColorEdit3("color2", (float*)&color2);

			ImGui::SliderFloat4("initial state", (float*)&initialState, 0.0f, 1.0f); ImGui::SameLine();
			if(ImGui::Button("Set"))
				slabTexture.clearImage(initialState);

			lightGradientTexture.Menu("Color Gradient", lightGradient);
		}
		ImGui::End();

		//bool showDemoWindow = false;
		//ImGui::ShowDemoWindow(&showDemoWindow);
	}
};

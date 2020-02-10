#pragma once
#include "GLProgram.h"
#include "Shader.h"

#include "utilities.h"
#include "Texture.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "ColorGradientTexture.h"
#include "View.h"
#include "Texture.h"

#include "utilities.h"

class GLComputeParticlesProgram : public GLProgram
{
public:
	View view;
	Shader particleUpdate;
	Shader initParticleMap;
	//Shader particleSort;
	Shader renderShader;

	SlabTexture particleMap1;
	SlabTexture particleMap2;

	glm::mat4 projection;
	
	unsigned int particleVAO{};
	unsigned int sourceParticleSSBO, destParticleSSBO;
	unsigned int numParticles = 65536;
	unsigned int particleStructSize = 8;

	float particleSize = 2.0f;
	float timestep = 1.0;

	glm::ivec3 simulationSize{ 128, 128, 128 };
	//unsigned int particlesPerCell = 8;

	glm::vec3 color1{ 1.0f, 1.0f, 1.0f };
	glm::vec3 color2{ 1.0f, 1.0f, 1.0f };

	bool initParticleMapThisFrame = true;

	GLComputeParticlesProgram() :
		GLProgram(4, 4, true, 1600, 900, "Compute Particles", true),
		particleMap1{
				{ 0, GL_TEXTURE_3D, simulationSize, GL_RGBA32UI, GL_RGBA_INTEGER, GL_UNSIGNED_INT,
				GL_NEAREST, GL_CLAMP_TO_BORDER, true, GL_WRITE_ONLY, {0.0, 0.0, 0.0, 0.0}}
		},
		particleMap2{
					{ 1, GL_TEXTURE_3D, simulationSize, GL_RGBA32UI, GL_RGBA_INTEGER, GL_UNSIGNED_INT,
					GL_NEAREST, GL_CLAMP_TO_BORDER, true, GL_WRITE_ONLY, {0.0, 0.0, 0.0, 0.0}}
		},
		particleUpdate(GL_COMPUTE_SHADER, "shaders/particles/particles.comp"),
		initParticleMap(GL_COMPUTE_SHADER, "shaders/particles/particles_init.comp"),
		renderShader("shaders/particles/particles.vert", "shaders/particles/particles.geom", "shaders/particles/particles.frag")
	{
		/*
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
		*/

		GLint size;
		glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &size);
		std::cout << "GL_MAX_SHADER_STORAGE_BLOCK_SIZE is " << size << " bytes." << std::endl;
		
		glGenVertexArrays(1, &particleVAO);
		glBindVertexArray(particleVAO);
	
		auto genSSBO = [this](unsigned int& id, int size)
		{
			glGenBuffers(1, &id);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
			glBufferData(GL_SHADER_STORAGE_BUFFER, size * sizeof(float), NULL, GL_DYNAMIC_COPY);
		};
		genSSBO(sourceParticleSSBO, particleStructSize * numParticles);
		genSSBO(destParticleSSBO, particleStructSize * numParticles);
		//genSSBO(particleMapSSBO, simulationSize.x * simulationSize.y * simulationSize.z * particlesPerCell);

		//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, particleMapSSBO);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);

		bindGlobalUniforms();
	}

	void update() override
	{
		menu();

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, sourceParticleSSBO);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, destParticleSSBO);

		particleUpdate.update();
		particleUpdate.use();
		glDispatchCompute(numParticles / 512, 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		// clear stuff
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// get view and projection matrices
		view.updateView2(window, deltaMousePos, deltaTime);
		const float aspect = screenSize.y != 0.0f ? (float)screenSize.x / (float)screenSize.y : 1.0f;
		projection = glm::perspective(glm::radians(75.0f), aspect, 0.1f, 1000.0f);

		renderShader.update();
		renderShader.use();
		renderShader.setUniform("aspect", aspect);
		
		//glBindVertexArray(particleVAO);
		//glBindBuffer(GL_ARRAY_BUFFER, destParticleSSBO);
		//glVertexAttribPointer(0, 1, GL_UINT, GL_FALSE, particleStructSize * sizeof(float), (void*)0);
		//glEnableVertexAttribArray(0);
		glDrawArrays(GL_POINTS, 0, numParticles);

		std::swap(sourceParticleSSBO, destParticleSSBO);
	}

	void bindGlobalUniforms()
	{
		Shader::bindGlobalUniform("view", &(view.invMatrix));
		Shader::bindGlobalUniform("projection", &projection);

		Shader::bindGlobalUniform("time", &time);
		Shader::bindGlobalUniform("deltaTime", &deltaTime);
		Shader::bindGlobalUniform("screenSize", &screenSize);
		Shader::bindGlobalUniform("screenMousePos", &mousePos);
		Shader::bindGlobalUniform("prevScreenMousePos", &prevMousePos);
		Shader::bindGlobalUniform("leftMouseDown", &mouseInputs[GLFW_MOUSE_BUTTON_1].held);
		Shader::bindGlobalUniform("rightMouseDown", &mouseInputs[GLFW_MOUSE_BUTTON_2].held);

		Shader::bindGlobalUniform("timestep", &timestep);

		Shader::bindGlobalUniform("simulationSize", &simulationSize);
		Shader::bindGlobalUniform("numParticles", &numParticles);

		Shader::bindGlobalUniform("color1", &color1);
		Shader::bindGlobalUniform("color2", &color2);

		Shader::bindGlobalUniform("particleSize", &particleSize);

		Shader::bindGlobalUniform("particleMapImage1", &particleMap1.defn.textureUnit);
		Shader::bindGlobalUniform("particleMapImage2", &particleMap2.defn.textureUnit);
		Shader::bindGlobalUniform("particleMapSampler1", &particleMap1.defn.textureUnit);
		Shader::bindGlobalUniform("particleMapSampler2", &particleMap2.defn.textureUnit);
	}

	void menu()
	{
		ImGuiWindowFlags windowFlags = 0;
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
			windowFlags |= ImGuiWindowFlags_NoInputs;

		ImGui::Begin("Settings", nullptr, windowFlags);
		{
			static glm::ivec3 gridSizeTemp = simulationSize;
			ImGui::SliderInt3("simulation size", (int*)&gridSizeTemp, 8, 256);
			const glm::ivec3 localWorkGroupSize(8, 8, 8);
			gridSizeTemp = (gridSizeTemp / localWorkGroupSize) * localWorkGroupSize;
			if (ImGui::Button("Set simulation size"))
			{
				simulationSize = gridSizeTemp;
				//Shader::setGlobalDefinition("SIMULATION_SIZE_X", std::to_string(simulationSize.x));
				//Shader::setGlobalDefinition("SIMULATION_SIZE_Y", std::to_string(simulationSize.y));
				//Shader::setGlobalDefinition("SIMULATION_SIZE_Z", std::to_string(simulationSize.z));
				//glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleMapSSBO);
				//glBufferData(GL_SHADER_STORAGE_BUFFER, simulationSize.x * simulationSize.y * simulationSize.z * particlesPerCell * sizeof(float), NULL, GL_DYNAMIC_COPY);
			}

			ImGui::SliderFloat("timestep", &timestep, 0.0f, 1.0f);
			ImGui::SliderFloat("particle size", &particleSize, 0.5f, 10.0f);

			int numParticlesTemp = numParticles;
			if (ImGui::SliderInt("num particles", &numParticlesTemp, 65536, 16777216))
			{
				numParticles = max(numParticlesTemp, 0);
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, sourceParticleSSBO);
				glBufferData(GL_SHADER_STORAGE_BUFFER, particleStructSize * numParticles * sizeof(float), NULL, GL_DYNAMIC_COPY);
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, destParticleSSBO);
				glBufferData(GL_SHADER_STORAGE_BUFFER, particleStructSize * numParticles * sizeof(float), NULL, GL_DYNAMIC_COPY);
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
			glm::vec2 realMousePos = glm::vec2(mousePos.x, screenSize.y - mousePos.y) / glm::vec2(screenSize) * glm::vec2(simulationSize);
			ImGui::Text("Mouse splat pos %.3f, %.3f", realMousePos.x, realMousePos.y);

			ImGui::ColorEdit3("color1", (float*)&color1);
			ImGui::ColorEdit3("color2", (float*)&color2);
		}
		ImGui::End();
	}
};

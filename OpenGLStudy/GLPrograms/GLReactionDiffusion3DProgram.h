#pragma once
#include "GLProgram.h"
#include "Shader.h"

#include "utilities.h"
#include "ComputeShader.h"
#include "Texture.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "ColorGradientTexture.h"
#include "View.h"

#include "utilities.h"

class GLReactionDiffusion3DProgram : public GLProgram
{
public:
	View view;
	ComputeShader reactionDiffusion;
	Shader renderShader;
	unsigned int quadVAO{};

	SlabTexture slabTexture;

	glm::vec4 diffusionRates{ 0.5f, 0.25f, 0.0f, 0.0f };
	float feed = 0.065;
	float kill = 0.066;
	float timestep = 1.0;
	int iterations = 1;

	glm::ivec3 simulationSize{88, 88, 88};

	glm::vec3 color1{ 1.0f, 1.0f, 1.0f };
	glm::vec3 color2{ 1.0f, 1.0f, 1.0f };

	glm::vec4 initialState{ 1.0f, 0.0f, 0.0f, 0.0f };

	ImGradient lightGradient;
	ColorGradientTexture lightGradientTexture;

	float rayStepSize = 1.0f;
	float levelSurface = 0.2f;

	glm::vec3 mouseSplatPos, prevMouseSplatPos;
	bool mouseSplatActive;

	glm::vec3 directionalLightLuminance{ 4.0f, 4.0f, 4.0f };
	glm::vec3 directionalLightDirection{ -1.0, -1.0, -1.0 };

	glm::vec3 albedo{1.0, 0.0, 0.0};
	float metallic = 0.688;
	float roughness = 0.465;
	float ao = 1.0;

	GLReactionDiffusion3DProgram() :
		GLProgram(4, 4, true, 1600, 900, "Reaction Diffusion 3D", true),
		slabTexture{
			{ 0, GL_TEXTURE_3D, simulationSize, GL_RGBA32F, GL_RGBA, GL_FLOAT,
			GL_LINEAR, GL_REPEAT, true, GL_WRITE_ONLY, {0.0, 0.0, 0.0, 0.0}}
	},
		lightGradientTexture{ 1, lightGradient },
		reactionDiffusion("shaders/reaction_diffusion/reactionDiffusion3D.comp"),
		renderShader("shaders/reaction_diffusion/reactionDiffusion3D.vert", "shaders/reaction_diffusion/reactionDiffusion3D.frag")
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

		directionalLightDirection = glm::normalize(directionalLightDirection);

		glGenVertexArrays(1, &quadVAO);
		glBindVertexArray(quadVAO);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		slabTexture.clearImage(initialState);

		bindGlobalUniforms();
	}


	vec2 intersectBox(vec3 rayOrigin, vec3 rayDir, vec3 boxMin, vec3 boxMax)
	{
		vec3 tMin = (boxMin - rayOrigin) / rayDir;
		vec3 tMax = (boxMax - rayOrigin) / rayDir;
		vec3 t1 = min(tMin, tMax);
		vec3 t2 = max(tMin, tMax);
		float tNear = max(max(t1.x, t1.y), t1.z);
		float tFar = min(min(t2.x, t2.y), t2.z);
		return vec2(tNear, tFar);
	}

	bool intersectPlane(const vec3& planeNorm, const vec3& planePos, const vec3& rayPos, const vec3& rayDir, float& t)
	{
		// assuming vectors are all normalized
		float denom = dot(planeNorm, rayDir);
		if (denom > 1e-6)
		{
			vec3 deltaPlane = planePos - rayPos;
			t = dot(deltaPlane, planeNorm) / denom;
			return t >= 0.0f;
		}
		return false;
	}

	void updateMouseSplat(float aspect)
	{
		prevMouseSplatPos = mouseSplatPos;

		vec3 Eye = vec3(view.matrix[3]);
		vec2 viewMouse = mousePos / glm::vec2(screenSize);
		viewMouse.y = 1.0f - viewMouse.y;
		viewMouse = viewMouse * 2.0f - vec2(1.0f);
		viewMouse.x *= aspect;
		vec3 worldMousePos = view.matrix * vec4(viewMouse, -2.0f, 1.0f);
		vec3 mouseRayDir = normalize(worldMousePos - Eye);

		vec3 boxCenter = vec3(0.0f);
		vec3 planeNorm = normalize(vec3(boxCenter - Eye));

		float intersectDistance = 0.0f;
		bool intersection = intersectPlane(planeNorm, boxCenter, Eye, mouseRayDir, intersectDistance);
		mouseSplatActive = intersection;
		mouseSplatPos = Eye + mouseRayDir * intersectDistance;
	}

	void update() override
	{
		menu();

		slabTexture.bind();
		for (int i = 0; i < iterations; i++)
		{
			reactionDiffusion.update();
			reactionDiffusion.use();
			glDispatchCompute(simulationSize.x / 8, simulationSize.y / 8, simulationSize.z / 8);
			glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
			slabTexture.swap();
		}
		
		// clear stuff
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// get view and projection matrices
		view.updateView2(window, deltaMousePos, deltaTime);
		const float aspect = screenSize.y != 0.0f ? (float)screenSize.x / (float)screenSize.y : 1.0f;
		glm::mat4 projection = glm::perspective(glm::radians(75.0f), aspect, 0.1f, 100.0f);
		updateMouseSplat(aspect);

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
		Shader::bindGlobalUniform("view", &(view.matrix));
		
		Shader::bindGlobalUniform("time", &time);
		Shader::bindGlobalUniform("deltaTime", &deltaTime);
		Shader::bindGlobalUniform("screenSize", &screenSize);
		Shader::bindGlobalUniform("screenMousePos", &mousePos);
		Shader::bindGlobalUniform("prevScreenMousePos", &prevMousePos);
		Shader::bindGlobalUniform("mouseSplatPos", &mouseSplatPos);
		Shader::bindGlobalUniform("prevMouseSplatPos", &prevMouseSplatPos);
		Shader::bindGlobalUniform("mouseSplatActive", &mouseSplatActive);
		Shader::bindGlobalUniform("leftMouseDown", &mouseInputs[GLFW_MOUSE_BUTTON_1].held);
		Shader::bindGlobalUniform("rightMouseDown", &mouseInputs[GLFW_MOUSE_BUTTON_2].held);

		Shader::bindGlobalUniform("diffusionRates", &diffusionRates);
		Shader::bindGlobalUniform("feed", &feed);
		Shader::bindGlobalUniform("kill", &kill);
		Shader::bindGlobalUniform("timestep", &timestep);

		Shader::bindGlobalUniform("simulationSize", &simulationSize);
		Shader::bindGlobalUniform("rayStepSize", &rayStepSize);

		Shader::bindGlobalUniform("color1", &color1);
		Shader::bindGlobalUniform("color2", &color2);

		Shader::bindGlobalUniform("lightGradientSampler", &lightGradientTexture.textureUnit);

		Shader::bindGlobalUniform("dirLight.color", &directionalLightLuminance);
		Shader::bindGlobalUniform("dirLight.direction", &directionalLightDirection);

		Shader::bindGlobalUniform("levelSurface", &levelSurface);

		Shader::bindGlobalUniform("physicalMaterial.albedo", &albedo);
		Shader::bindGlobalUniform("physicalMaterial.metallic", &metallic);
		Shader::bindGlobalUniform("physicalMaterial.roughness", &roughness);
		Shader::bindGlobalUniform("physicalMaterial.ao", &ao);
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
				slabTexture.setSize(simulationSize);
			}
			ImGui::SliderFloat4("diffusion rates", (float*)&diffusionRates, 0.0f, 1.0f);
			const char* items[] = { "Negative bubbles (sigma)", "Bubbles (rho)",
				"Precritical bubbles (rho/kappa)", "Worms and loops (kappa)", "Stable solitons (nu)", "The U-Skate World (pi)",
				"Worms (mu)", "Worms join into maze (kappa)", "Negatons (iota)", "Turing patterns (delta)",
				"Chaos to Turing negatons (beta)", "Fingerprints (theta/kappa)", "Chaos with negatons (beta/delta)",
				"Spots and worms (eta)", "Self-replicating spots (lambda)", "Super-resonant mazes (theta)", "Mazes (kappa)",
				"Mazes with some chaos (gamma)", "Chaos (beta)", "Pulsating solitons (zeta)", "Warring microbes (epsilon)",
				"Spots and loops (alpha)", "Moving spots (alpha)", "Waves (xi)"
			};
			static int itemCurrent = -1;

			if (ImGui::Combo("presets", &itemCurrent, items, IM_ARRAYSIZE(items)))
			{
				const glm::vec2 fkValues[] = {
					{  0.098,     0.0555   }, // Negative bubbles (sigma)
					{  0.098,     0.057    }, // Positive bubbles (rho)
					{  0.082,     0.059    }, // Precritical bubbles (rho/kappa)
					{  0.082,     0.060    }, // Worms and loops (kappa)
					{  0.074,     0.064    }, // Stable solitons (nu)
					{  0.062,     0.0609   }, // The U-Skate World (pi)
					{  0.058,     0.065    }, // Worms (mu)
					{  0.046,     0.063    }, // Worms join into maze (kappa)
					{  0.046,     0.0594   }, // Negatons (iota)
					{  0.042,     0.059    }, // Turing patterns (delta)
					{  0.039,     0.058    }, // Chaos to Turing negatons (beta)
					{  0.037,     0.06     }, // Fingerprints (theta/kappa)
					{  0.0353,    0.0566   }, // Chaos with negatons (beta/delta)
					{  0.034,     0.0618   }, // Spots and worms (eta)
					{  0.03,      0.063    }, // Self-replicating spots (lambda)
					{  0.03,      0.0565   }, // Super-resonant mazes (theta)
					{  0.029,     0.057    }, // Mazes (kappa)
					{  0.026,     0.055    }, // Mazes with some chaos (gamma)
					{  0.026,     0.051    }, // Chaos (beta)
					{  0.025,     0.06     }, // Pulsating solitons (zeta)
					{  0.022,     0.059    }, // Warring microbes (epsilon)
					{  0.018,     0.051    }, // Spots and loops (alpha)
					{  0.014,     0.054    }, // Moving spots (alpha)
					{  0.014,     0.045    }
				};
				glm::vec2 preset = fkValues[itemCurrent];
				feed = preset.x;
				kill = preset.y;
			}
			ImGui::SliderFloat("feed", &feed, 0.0f, 0.3f);
			ImGui::SliderFloat("kill", &kill, 0.0f, 0.3f);
			ImGui::SliderFloat("timestep", &timestep, 0.0f, 1.0f);
			ImGui::SliderInt("iterations", &iterations, 1, 100);
			ImGui::SliderFloat("ray step size", &rayStepSize, 0.1f, 4.0f);
			rayStepSize = utl::max(rayStepSize, 0.05f);

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
			if (ImGui::Button("Set"))
				slabTexture.clearImage(initialState);

			lightGradientTexture.Menu("Color Gradient", lightGradient);

			const int flags = ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR;
			ImGui::ColorEdit3("directional light luminance", (float*)&directionalLightLuminance, flags);
			ImGui::SliderFloat("level surface", &levelSurface, 0.0f, 0.3f);
			ImGui::gizmo3D("light dir", directionalLightDirection);
			directionalLightDirection = glm::normalize(directionalLightDirection);

			ImGui::ColorEdit3("albedo", (float*)&albedo, flags);
			ImGui::SliderFloat("metallic", &metallic, 0.0f, 1.0f);
			ImGui::SliderFloat("roughness", &roughness, 0.0f, 1.0f);
			ImGui::SliderFloat("ao", &ao, 0.0f, 1.0f);
		}
		ImGui::End();

		bool showDemoWindow = false;
		ImGui::ShowDemoWindow(&showDemoWindow);
	}
};

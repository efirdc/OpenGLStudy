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
#include "SceneManager.h"

#include "utilities.h"

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

int raymarch()
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
	sceneManager->window = glfwCreateWindow(1600, 900, "Raymarch test", NULL, NULL);
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

	// Setup ImGui
	ImGui::CreateContext();
	sceneManager->imguiIO = &ImGui::GetIO(); (void)sceneManager->imguiIO;
	ImGui_ImplGlfwGL3_Init(sceneManager->window, true);
	ImGui::StyleColorsDark();

	// View
	View * view = new View;

	Shader raymarch("shaders/raymarch/raymarch.vert", "shaders/raymarch/raymarch.frag");
	raymarch.use();

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

	static glm::vec3 clearColor(0.0f);

	// Main loop
	sceneManager->newFrame();
	while (!glfwWindowShouldClose(sceneManager->window))
	{
		// new frame
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();
		sceneManager->newFrame();

		// clear stuff
		glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// get view and projection matrices
		updateView2(view, sceneManager);
		float aspect = sceneManager->screenSize.y != 0.0f ? (float)sceneManager->screenSize.x / (float)sceneManager->screenSize.y : 1.0f;
		glm::mat4 projection = glm::perspective(glm::radians(75.0f), aspect, 0.1f, 100.0f);

		raymarch.update();
		raymarch.use();
		raymarch.setUniform("view", view->matrix);
		raymarch.setUniform("aspect", aspect);
		//raymarch.setUniform("time", sceneManager->time);

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// end frame stuff
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(sceneManager->window);
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
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
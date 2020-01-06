#include "GLProgram.h"

bool GLProgram::sceneRunning = false;

void GLProgram::run()
{
	if (sceneRunning)
	{
		std::cout << "Only one scene can run at once." << std::endl;
		return;
	}
	sceneRunning = true;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(screenSize.x, screenSize.y, windowName.c_str(), NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	// Initialize GLAD to get OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	glEnable(GL_DEPTH_TEST);

	if (glDebug)
		glDebug_enable();

	// Setup ImGui
	ImGui::CreateContext();
	imguiIO = &ImGui::GetIO(); (void)imguiIO;
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();

	newFrame();
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();

		newFrame();
		update();

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}

	terminate();
	glfwTerminate();
	sceneRunning = false;
}

void GLProgram::newFrame()
{
	// update framebuffer size
	sizeFramebufferToWindow();

	// update time
	float newTime = glfwGetTime();
	deltaTime = newTime - time;
	time = newTime;
	frameNumber += 1;

	// update mouse
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	glm::vec2 newMousePos((float)mouseX, (float)mouseY);
	deltaMousePos = newMousePos - mousePos;
	prevMousePos = mousePos;
	mousePos = newMousePos;
	leftMouseDown = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;
	rightMouseDown = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS;

	// handle switching between camera mode and mouse active mode
	if (captureMouse) 
	{
		bool mouseInWindow = glm::all(glm::greaterThan(mousePos, glm::vec2(0.0)) && glm::lessThan(mousePos, screenSize));
		bool leftMouseClicked = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;
		bool leftCtrl = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;
		if (leftMouseClicked)
		{
			if (mouseInWindow && !imguiIO->WantCaptureMouse)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			else
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		if (leftCtrl)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void GLProgram::sizeFramebufferToWindow()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	int frameBufferWidth, frameBufferHeight;
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	glViewport(0, 0, frameBufferWidth, frameBufferHeight);
	screenSize = glm::vec2((float)frameBufferWidth, (float)frameBufferHeight);
}

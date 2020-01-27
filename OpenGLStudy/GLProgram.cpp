#include "GLProgram.h"

GLProgram::GLProgram(
	int versionMajor,
	int versionMinor,
	bool captureMouse,
	int windowWidth,
	int windowHeight,
	std::string windowName,
	bool glDebug
) :
	versionMajor(versionMajor),
	versionMinor(versionMinor),
	captureMouse(captureMouse),
	screenSize(windowWidth, windowHeight),
	windowName(windowName),
	glDebug(glDebug)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	if (glDebug)
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	window = glfwCreateWindow(screenSize.x, screenSize.y, windowName.c_str(), NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	//glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

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

	mouseInputs[GLFW_MOUSE_BUTTON_1];
	mouseInputs[GLFW_MOUSE_BUTTON_2];
	keyInputs[GLFW_KEY_LEFT_CONTROL];
}


void GLProgram::run()
{
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
}

GLProgram::~GLProgram()
{
	glfwTerminate();
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

	updateInputData();

	// handle switching between camera mode and mouse active mode
	if (captureMouse) 
	{
		bool mouseInWindow = glm::all(glm::greaterThan(mousePos, glm::vec2(0.0)) && glm::lessThan(mousePos, glm::vec2(screenSize)));

		/*
		if (mouseInputs[GLFW_MOUSE_BUTTON_1].held)
		{
			if (mouseInWindow && !imguiIO->WantCaptureMouse)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			else
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}*/
		if (keyInputs[GLFW_KEY_LEFT_CONTROL].pressed)
		{
			int inputMode = glfwGetInputMode(window, GLFW_CURSOR);
			if (inputMode == GLFW_CURSOR_NORMAL && !imguiIO->WantCaptureMouse && mouseInWindow)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			else if (inputMode == GLFW_CURSOR_DISABLED)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
			
	}
}

void GLProgram::sizeFramebufferToWindow()
{
	int frameBufferWidth, frameBufferHeight;
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	glViewport(0, 0, frameBufferWidth, frameBufferHeight);
	screenSize = glm::ivec2(frameBufferWidth, frameBufferHeight);
}

void GLProgram::updateInputData()
{
	for (auto& elem : mouseInputs)
	{
		InputData& data = elem.second;
		int button = elem.first;
		bool held = glfwGetMouseButton(window, button) == GLFW_PRESS;
		data.pressed = held && !data.held;
		data.held = held;
	}

	for (auto& elem : keyInputs)
	{
		InputData& data = elem.second;
		int key = elem.first;
		bool held = glfwGetKey(window, key) == GLFW_PRESS;
		data.pressed = held && !data.held;
		data.held = held;
	}
}
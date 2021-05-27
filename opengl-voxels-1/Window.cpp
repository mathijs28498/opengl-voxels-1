#include "Window.h"

Window::Window(int width, int height, const char* title, glm::vec4 backgroundColor) {
	this->backgroundColor = backgroundColor;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	window = glfwCreateWindow(width, height, title, nullptr, nullptr);

	if (window == NULL) {
		throw std::runtime_error("Failed to create GLFW window!");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("Failed to initialize GLAD");
	}

	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
	glfwSwapInterval(0);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);

	// TODO: Fix the vertices
	glEnable(GL_CULL_FACE); 
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);
}

Window::~Window() {
	glfwTerminate();
}

void Window::beginLoop() {
	glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::endLoop() {
	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool Window::shouldClose() {
	return glfwWindowShouldClose(window);
}
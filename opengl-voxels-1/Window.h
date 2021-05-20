#pragma once

#include "Camera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <stdexcept>

class Window {
public:
	GLFWwindow* window;

	Window(int width, int height, const char* title, glm::vec4 backgroundColor);
	~Window();
	bool shouldClose();

	void beginLoop();
	void endLoop();
	

private:
	glm::vec4 backgroundColor;
	static void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		Camera* cam = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
		cam->setWidthHeight(width, height);
	}
};
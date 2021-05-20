#include "Window.h"
#include "GLHelperFunctions.h"
#include "Model.h"

#include <iostream>
//
//#include <glad/glad.h>
//#include<GLFW/glfw3.h>

const int WIDTH = 800;
const int HEIGHT = 600;
const char* TITLE = "Voxels everywhere";

void processInput(GLFWwindow* window) {
	Camera* cam = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	cam->moveCamera(window);
}

int main() {
	try {
		Window window(WIDTH, HEIGHT, TITLE, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		Camera cam(window.window, glm::vec3(0, 1.5f, -2), 0.06f, 0.1f, WIDTH, HEIGHT);

		Model plane(glm::vec3(0.1f), 20, 20, 20);
		Shader simpleShader("shaders/simple.vert", "shaders/simple.frag");

		while (!window.shouldClose()) {
			processInput(window.window);
			window.beginLoop();


			glUseProgram(simpleShader.ID);
			cam.setUniforms(&simpleShader);
			plane.draw(&simpleShader);

			window.endLoop();
		}
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}
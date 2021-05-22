#include "Window.h"
#include "GLHelperFunctions.h"
#include "Chunk.h"

#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;
const char* TITLE = "Voxels everywhere";
const int xAmount = 100;
const int zAmount = 100;

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
		Camera cam(window.window, glm::vec3(0, 1.5f, 2), 0.1f * 5, 5, 0.1f, WIDTH, HEIGHT);
		//Camera cam(window.window, glm::vec3(0, 1.5f, 2), 0.1f * 1, 5, 0.1f, WIDTH, HEIGHT);

		Shader simpleShaderGeom("shaders/instancing.vert", "shaders/instancing.geom", "shaders/instancing.frag");
		const float voxSize = 0.25f;

		std::vector<Chunk>chunks{
			Chunk("", xAmount, zAmount, glm::vec2(0, 0), voxSize),

			Chunk("", xAmount, zAmount, glm::vec2(xAmount, 0), voxSize),
			Chunk("", xAmount, zAmount, glm::vec2(0, zAmount), voxSize),
			Chunk("", xAmount, zAmount, glm::vec2(xAmount, zAmount), voxSize),

			Chunk("", xAmount, zAmount, glm::vec2(-xAmount, 0), voxSize),
			Chunk("", xAmount, zAmount, glm::vec2(0, -zAmount), voxSize),
			Chunk("", xAmount, zAmount, glm::vec2(-xAmount, -zAmount), voxSize),

			Chunk("", xAmount, zAmount, glm::vec2(xAmount, -zAmount), voxSize),
			Chunk("", xAmount, zAmount, glm::vec2(-xAmount, zAmount), voxSize),
		};

		double lastTime = glfwGetTime();
		double lastTime2 = glfwGetTime();

		while (!window.shouldClose()) {
			// Measure speed
			double currentTime = glfwGetTime();
			double diff = (currentTime - lastTime) * 1000.0;
			lastTime = currentTime;
			if (currentTime - lastTime2 > 1.0) {
				printf("%f ms/frame\n", diff);
				lastTime2 = currentTime;
			}

			// TODO: Make input a callback
			processInput(window.window);
			window.beginLoop();

			simpleShaderGeom.use();
			cam.setUniforms(&simpleShaderGeom);

			for (Chunk chunk : chunks) 	{
				chunk.draw(&simpleShaderGeom);
			}

			window.endLoop();

		}
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}
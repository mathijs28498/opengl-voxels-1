#include "Window.h"
#include "GLHelperFunctions.h"
#include "Chunk.h"
#include "Octree.h"

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
		Camera cam(window.window, glm::vec3(0, 1.5f, 2), 0.001f, 5, 0.1f, WIDTH, HEIGHT);

		Shader voxelShader("shaders/voxelInstancing.vert", "shaders/voxelInstancing.geom", "shaders/voxelInstancing.frag");
		Shader boundingBoxShader("shaders/octreeBoundingBox.vert", "shaders/octreeBoundingBox.geom", "shaders/octreeBoundingBox.frag");

		Octree tree = Octree({0, 0, 0}, 1024);
		tree.insert({ {0, 0, 0}, {1, 0, 0} });
		tree.insert({ {1, 0, 0}, {1, 0, 0} });
		tree.insert({ {2, 0, 0}, {1, 0, 0} });
		tree.calculateBoundingBoxVAO();
		tree.calculateVoxelVAO();
		Chunk chunk(100, 100, {0, 0}, VOX_SIZE);

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


			//// TODO: Make input a callback
			processInput(window.window);
			window.beginLoop();

			boundingBoxShader.use();
			cam.setUniforms(&boundingBoxShader);
			tree.drawBoundingBoxes(&boundingBoxShader);

			voxelShader.use();
			cam.setUniforms(&voxelShader);
			tree.drawVoxels(&voxelShader);

			//chunk.draw(&voxelShader);

			window.endLoop();

		}
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}
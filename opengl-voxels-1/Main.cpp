#include "Window.h"
#include "GLHelperFunctions.h"
#include "Octree.h"
#include "Model.h"
#include "Scene.h"

#include <glm/gtx/rotate_vector.hpp>

#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;
const char* TITLE = "Voxels everywhere";
const int xAmount = 100;
const int zAmount = 100;
bool showBoundingBoxes = false;

void processInput(GLFWwindow* window) {
	Camera* cam = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		showBoundingBoxes = true;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		showBoundingBoxes = false;
	}
	cam->moveCamera(window);
}

int main() {
	try {
		Window window(WIDTH, HEIGHT, TITLE, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		Camera cam(window.window, glm::vec3(4, 5, 10), 0.01f, 5, 0.1f, WIDTH, HEIGHT);

		Shader terrainVoxelShader("shaders/voxelInstancingTerrain.vert", "shaders/voxelInstancing.geom", "shaders/voxelInstancing.frag");
		Shader voxelShader("shaders/voxelInstancing.vert", "shaders/voxelInstancing.geom", "shaders/voxelInstancing.frag");
		Shader boundingBoxShader("shaders/octreeBoundingBox.vert", "shaders/octreeBoundingBox.geom", "shaders/octreeBoundingBox.frag");

		uint32_t octreeSize = 256;

		std::vector<Octree> trees;
		trees.push_back(Octree({ 0, 0, 0 }, octreeSize));
		/*trees.push_back(Octree({ 0, 0, -static_cast<int32_t>(octreeSize) * 1 }, octreeSize));
		trees.push_back(Octree({ 0, 0, -static_cast<int32_t>(octreeSize) * 2 }, octreeSize));
		trees.push_back(Octree({ 0, 0, -static_cast<int32_t>(octreeSize) * 3 }, octreeSize));
		trees.push_back(Octree({ 0, 0, -static_cast<int32_t>(octreeSize) * 4 }, octreeSize));
		trees.push_back(Octree({ 0, 0, -static_cast<int32_t>(octreeSize) * 5 }, octreeSize));
		trees.push_back(Octree({ 0, 0, -static_cast<int32_t>(octreeSize) * 6 }, octreeSize));
		trees.push_back(Octree({ static_cast<int32_t>(octreeSize), 0, 0 }, octreeSize));
		trees.push_back(Octree({ static_cast<int32_t>(octreeSize), 0, -static_cast<int32_t>(octreeSize) * 1 }, octreeSize));
		trees.push_back(Octree({ static_cast<int32_t>(octreeSize), 0, -static_cast<int32_t>(octreeSize) * 2 }, octreeSize));
		trees.push_back(Octree({ static_cast<int32_t>(octreeSize), 0, -static_cast<int32_t>(octreeSize) * 3 }, octreeSize));
		trees.push_back(Octree({ static_cast<int32_t>(octreeSize), 0, -static_cast<int32_t>(octreeSize) * 4 }, octreeSize));
		trees.push_back(Octree({ static_cast<int32_t>(octreeSize), 0, -static_cast<int32_t>(octreeSize) * 5 }, octreeSize));
		trees.push_back(Octree({ static_cast<int32_t>(octreeSize), 0, -static_cast<int32_t>(octreeSize) * 6 }, octreeSize));*/
		std::cout << octreeSize * octreeSize * 2 * trees.size() << '\n';
		for (size_t i = 0; i < trees.size(); i++) {
			trees[i].makeNoiseTerrain();
			trees[i].calculateBoundingBoxVAO();
			trees[i].calculateVoxelVAO();
		}
		

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

			for (size_t i = 0; i < trees.size(); i++) {
				if (showBoundingBoxes) {
					trees[i].drawBoundingBoxes(&boundingBoxShader, &cam);
				}
				trees[i].drawVoxels(&terrainVoxelShader, &cam);
			}

			window.endLoop();

		}
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}
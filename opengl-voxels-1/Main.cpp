#include "Window.h"
#include "GLHelperFunctions.h"
#include "Octree.h"
#include "Model.h"
#include "Scene.h"
#include "Component.h"

#include <glm/gtx/rotate_vector.hpp>

#include <iostream>

#define quote(x) #x

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
		Camera cam(window.window, glm::vec3(4, 5, 10), 0.001f, 5, 0.1f, WIDTH, HEIGHT);

		//Shader terrainVoxelShader("shaders/voxelInstancingTerrain.vert", "shaders/voxelInstancing.geom", "shaders/voxelInstancing.frag");
		Shader voxelShader("shaders/voxelInstancing.vert", "shaders/voxelInstancing.geom", "shaders/voxelInstancing.frag");
		//Shader boundingBoxShader("shaders/octreeBoundingBox.vert", "shaders/octreeBoundingBox.geom", "shaders/octreeBoundingBox.frag");

		Scene scene;

		std::vector<Voxel> voxels{
			{{10, 0, 0}, {1, 1, 1}},
			{{10, 1, 0}, {1, 1, 1}},
			{{10, 2, 0}, {1, 1, 1}},
			{{10, 3, 0}, {1, 1, 1}},
			{{10, 4, 0}, {1, 1, 1}},
			{{10, 5, 0}, {1, 1, 1}},
			{{10, 7, 0}, {1, 1, 1}},
			{{10, 8, 0}, {1, 1, 1}},
			{{10, 9, 0}, {1, 1, 1}},
			{{10, 10, 0}, {1, 1, 1}},
			{{10, 11, 0}, {1, 1, 1}},
			{{10, 12, 0}, {1, 1, 1}},
			{{10, 14, 0}, {1, 1, 1}},
			{{10, 17, 0}, {1, 1, 1}},
			{{10, 19, 0}, {1, 1, 1}},
		};

		uint32_t VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, voxels.size() * sizeof(Voxel), voxels.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Voxel), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Voxel), (void*)offsetof(Voxel, color));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);


		Entity entity;
		entity.insertComponent(new Transform{ { 0, 0, 0 } });
		entity.insertComponent(new CameraComp{ cam });
		entity.insertComponent(new VoxelRenderer{ &voxelShader, &cam, VAO, (uint32_t) voxels.size() });

		scene.addEntity(&entity);

		double lastTime = glfwGetTime();
		double lastTime2 = glfwGetTime();

		scene.start();
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

			scene.update();

			window.endLoop();

		}
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}
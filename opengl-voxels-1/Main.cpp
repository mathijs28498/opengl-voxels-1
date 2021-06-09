#include "Window.h"
#include "GLHelperFunctions.h"
#include "Octree.h"
#include "Model.h"
#include "Scene.h"
#include "Component.h"

#include <glm/gtx/rotate_vector.hpp>

#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;
const char* TITLE = "Voxels everywhere";
const int xAmount = 100;
const int zAmount = 100;
bool showBoundingBoxes = false;

void processInput(GLFWwindow* window) {
	//Camera* cam = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		showBoundingBoxes = true;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		showBoundingBoxes = false;
	}
	//cam->moveCamera(window);
}

int main() {

	try {
		Window window(WIDTH, HEIGHT, TITLE, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		Camera cam(window.window, glm::vec3(30, 15, 20), 0.01f, 5, 0.1f, WIDTH, HEIGHT);


		Shader terrainVoxelShader("shaders/voxelInstancingTerrain.vert", "shaders/voxelInstancing.geom", "shaders/voxelInstancing.frag");
		Shader voxelShader("shaders/voxelInstancing.vert", "shaders/voxelInstancing.geom", "shaders/voxelInstancing.frag");
		Shader boundingBoxShader("shaders/octreeBoundingBox.vert", "shaders/octreeBoundingBox.geom", "shaders/octreeBoundingBox.frag");

		Model model("C:/Users/mathi/Downloads/chromeDownloads/MagicaVoxel-0.99.6.2-win32/MagicaVoxel-0.99.6.2-win32/vox/monu10.vox");

		Scene scene;
		//scene.setMainCameraComponent(new CameraComp(&cam));

		Entity* cameraEntity = new Entity;
		cameraEntity->insertComponent(new CameraComp(&cam));
		cameraEntity->insertComponent(new KeyInput);
		cameraEntity->insertComponent(new MouseCursorInput);
		scene.addEntity(cameraEntity);

		window.setScenePointer(&scene);

		Octree octree({0, 0, 0}, 256);
		octree.makeNoiseTerrain();
		octree.calculateVoxelVAO();
		octree.calculateBoundingBoxVAO();

		Entity* entity = new Entity;
		entity->insertComponent(new Transform{ {0, 0, 0} });
		entity->insertComponent(new KeyInput);
		VoxelRenderer vr = octree.getVoxelRenderer(&terrainVoxelShader, &cam);
		BoundingBoxRenderer br = octree.getBoundingBoxRenderer(&boundingBoxShader, &cam, false);
		entity->insertComponent(&vr);
		entity->insertComponent(&br);
		scene.addEntity(entity);

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
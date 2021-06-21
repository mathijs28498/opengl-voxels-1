#include "Headers/Window.h"
#include "Headers/Global/GLHelperFunctions.h"
#include "Headers/Objects/Octree.h"
#include "Headers/Objects/Model.h"
#include "Headers/Scene/Scene.h"
#include "Headers/ECS/Component.h"
#include "Headers/ECS/OctreeECS.h"

#include <glm/gtx/rotate_vector.hpp>

#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;
const char* TITLE = "Voxels everywhere";

void addCameraEntity(Scene& scene, Camera* cam) {
	Entity* cameraEntity = new Entity;
	cameraEntity->insertComponent(new CameraComp(cam));
	cameraEntity->insertComponent(new KeyInputComp);
	cameraEntity->insertComponent(new MouseCursorInputComp);
	scene.addEntity(cameraEntity);
}

void addOctreeEntity(Scene& scene, Shader* terrainVoxelShader, Shader* boundingBoxShader, Camera* cam) {
	Octree* octree = new Octree({ 0, 0, 0 }, 256);
	octree->makeNoiseTerrain();
	octree->calculateVoxelVAO();
	octree->calculateBoundingBoxVAO();

	Entity* entity = new Entity;
	entity->insertComponent(new TransformComp{ {0, 0, 0} });
	entity->insertComponent(new KeyInputComp);
	entity->insertComponent(octree->getVoxelRenderer(terrainVoxelShader, cam));
	entity->insertComponent(octree->getBoundingBoxRenderer(boundingBoxShader, cam, false));
	scene.addEntity(entity);
}


void addModelEntity(Scene& scene, glm::vec3 pos, Model* model, Shader* terrainVoxelShader, Camera* cam) {
	Entity* entity = new Entity;
	entity->insertComponent(new TransformComp(pos));
	entity->insertComponent(model->getVoxelRenderer(terrainVoxelShader, cam));
	scene.addEntity(entity);
}

void addOctreeCompEntity(Scene& scene, Shader* terrainVoxelShader, Camera* cam) {
	Entity* entity = new Entity;
	entity->insertComponent(new OctreeComp({ 0, 0, 0 }, 256));
	entity->insertComponent(new VoxelRendererComp(terrainVoxelShader, cam, 0, 0));
	entity->insertComponent(new TransformComp());
	scene.addEntity(entity);
}

int main() {

	try {
		Window window(WIDTH, HEIGHT, TITLE, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		Camera cam(window.window, glm::vec3(0, 15, 20), 0.01f, 5, 0.1f, WIDTH, HEIGHT);


		Shader terrainVoxelShader("shaders/voxelInstancingTerrain.vert", "shaders/voxelInstancing.geom", "shaders/voxelInstancing.frag");
		Shader voxelShader("shaders/voxelInstancing.vert", "shaders/voxelInstancing.geom", "shaders/voxelInstancing.frag");
		Shader boundingBoxShader("shaders/octreeBoundingBox.vert", "shaders/octreeBoundingBox.geom", "shaders/octreeBoundingBox.frag");

		/*Model monu("models/monu10.vox");
		Model teapot("models/teapot.vox");*/

		Scene scene;
		window.setScenePointer(&scene);
		addCameraEntity(scene, &cam);
		/*addOctreeEntity(scene, &terrainVoxelShader, &boundingBoxShader, &cam);
		addModelEntity(scene, glm::vec3(30 * VOX_SIZE, 45*VOX_SIZE, 30 * VOX_SIZE), &monu, &voxelShader, &cam);
		addModelEntity(scene, glm::vec3(100 * VOX_SIZE, 45*VOX_SIZE, 0), &teapot, &voxelShader, &cam);*/
		addOctreeCompEntity(scene, &terrainVoxelShader, &cam);

		// TODO: Make better benchmark
		// TODO: Make a deltaTime variable
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
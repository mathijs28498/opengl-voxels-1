#include "Headers/Window.h"
#include "Headers/Global/GLHelperFunctions.h"
#include "Headers/Objects/Octree.h"
#include "Headers/Objects/Model.h"
#include "Headers/Scene/Scene.h"
#include "Headers/ECS/Component.h"
#include "Headers/ECS/OctreeECS.h"

#include <glm/gtx/rotate_vector.hpp>

#include <iostream>
#include <chrono>
#include <thread>

const int WIDTH = 800;
const int HEIGHT = 600;
const char* TITLE = "Voxels everywhere";

Entity* addCameraEntity(Scene& scene, Camera* cam) {
	Entity* entity = new Entity;
	//cameraEntity->insertComponent(new TransformComp({ 0, 0, 0 }));
	entity->insertComponent(new TransformComp({ 0, 15, 20 }));
	entity->insertComponent(new CameraComp(cam));
	entity->insertComponent(new KeyInputComp);
	entity->insertComponent(new MouseCursorInputComp);
	//scene.addEntity(cameraEntity);
	addEntityEvent.notify(entity);
	return entity;
}

void addModelEntity(Scene& scene, glm::vec3 pos, Model* model, Shader* terrainVoxelShader, Camera* cam) {
	Entity* entity = new Entity;
	entity->insertComponent(new TransformComp(pos));
	entity->insertComponent(model->getVoxelRenderer(terrainVoxelShader, cam));
	//scene.addEntity(entity);
	addEntityEvent.notify(entity);
}

void addOctreeCompEntity(Scene& scene, Shader* terrainVoxelShader, Shader* boundingBoxShader, Camera* cam, TransformComp* transform) {
	//Entity* entity = new Entity;
	//entity->insertComponent(new OctreeComp({ 0, 0, 0 }, 256, transform));
	//entity->insertComponent(new VoxelRendererComp(terrainVoxelShader, cam, 0, 0));
	//entity->insertComponent(new BoundingBoxRendererComp(boundingBoxShader, cam, 0, 0, false));
	//entity->insertComponent(new KeyInputComp());
	//entity->insertComponent(new TransformComp());
	////scene.addEntity(entity);
	//addEntityEvent.notify(entity);
	
	Entity* entity2 = new Entity;
	entity2->insertComponent(new OctreeComp({ 1, 0, 0 }, 256, transform));
	entity2->insertComponent(new VoxelRendererComp(terrainVoxelShader, cam, 0, 0));
	entity2->insertComponent(new BoundingBoxRendererComp(boundingBoxShader, cam, 0, 0, false));
	entity2->insertComponent(new KeyInputComp());
	entity2->insertComponent(new TransformComp());
	//scene.addEntity(entity);
	addEntityEvent.notify(entity2);

	/*Entity* entity = new Entity;
	entity->insertComponent(new OctreeHandlerComp(transform));
	addEntityEvent.notify(entity);*/
}

int main() {

	try {
		Window window(WIDTH, HEIGHT, TITLE, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		Camera cam(window.window, 0.01f, 5, 0.1f, WIDTH, HEIGHT);


		Shader terrainVoxelShader("shaders/voxelInstancingTerrain.vert", "shaders/voxelInstancing.geom", "shaders/voxelInstancing.frag");
		Shader voxelShader("shaders/voxelInstancing.vert", "shaders/voxelInstancing.geom", "shaders/voxelInstancing.frag");
		Shader boundingBoxShader("shaders/octreeBoundingBox.vert", "shaders/octreeBoundingBox.geom", "shaders/octreeBoundingBox.frag");

		/*Model monu("models/monu10.vox");
		Model teapot("models/teapot.vox");*/

		Scene scene;
		window.setScenePointer(&scene);
		Entity* cameraEntity = addCameraEntity(scene, &cam);
		//addModelEntity(scene, glm::vec3(30 * VOX_SIZE, 45*VOX_SIZE, 30 * VOX_SIZE), &monu, &voxelShader, &cam);
		//addModelEntity(scene, glm::vec3(100 * VOX_SIZE, 45*VOX_SIZE, 0), &teapot, &voxelShader, &cam);
		addOctreeCompEntity(scene, &terrainVoxelShader, &boundingBoxShader, &cam, 
			static_cast<TransformComp*>(cameraEntity->getComponent(gcn(TransformComp()))));

		// TODO: Make better benchmark
		// TODO: Make a deltaTime variable

		scene.start();

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
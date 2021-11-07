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
	entity->insertComponent(new TransformComp({ -10, 3, 4 }));
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

void addOctreeCompEntity(Scene& scene, Shader* terrainVoxelShader, Shader* boundingBoxShader, Camera* cam, TransformComp* transform, TransformComp* markerCubeTransform) {
	Entity* entity = new Entity;
	VoxelRendererComp* voxelRenderer = new VoxelRendererComp(terrainVoxelShader, cam, 0, 0);
	BoundingBoxRendererComp* boundingBoxRenderer = new BoundingBoxRendererComp(boundingBoxShader, cam, 0, 0, false);
	entity->insertComponent(new OctreeHandlerComp(voxelRenderer, boundingBoxRenderer, transform));
	addEntityEvent.notify(entity);

	Entity* entity2 = new Entity;
	entity2->insertComponent(new RayCastComp(cam, markerCubeTransform));
	entity2->insertComponent(new KeyInputComp());
	addEntityEvent.notify(entity2);
}

VoxelRendererComp* voxelRendererRaycastTest;
TransformComp* createRaycastTestEntity(std::vector<Voxel>* voxels, Shader* terrainVoxelShader, Camera* cam) {
	uint32_t VAO = createVoxelVAO(voxels->data(), voxels->size());

	Entity* entity = new Entity();
	voxelRendererRaycastTest = new VoxelRendererComp(terrainVoxelShader, cam, VAO, voxels->size());
	TransformComp* transform = new TransformComp({ 0, 0, 0 });
	entity->insertComponent(voxelRendererRaycastTest);
	entity->insertComponent(transform);
	addEntityEvent.notify(entity);
	return transform;
}


TransformComp* addMarkerCube(Shader* wireframeShader, Camera* cam) {
	Voxel voxel = { 0x01000000, 0xFF0000FF };

	uint32_t VAO = createVoxelVAO(&voxel, 1);

	Entity* entity = new Entity();

	VoxelRendererComp* markerCube = new VoxelRendererComp(wireframeShader, cam, VAO, 1);
	TransformComp* transform = new TransformComp({ 0, 0, 0 });
	entity->insertComponent(markerCube);
	entity->insertComponent(transform);
	addEntityEvent.notify(entity);

	return transform;
}



TransformComp* raycastTestTransform;
Camera* camP;

std::vector<Voxel> voxels = {
			{ 0x01000000, 0xFFFFFF00 },
			{ 0x02020000, 0xFFFFFF00 },
			{ 0x03050000, 0xFFFFFF00 },
			{ 0x040A0000, 0xFFFFFF00 },
};

int main() {
	try {
		Window window = { WIDTH, HEIGHT, TITLE, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) };
		Camera cam = { window.window, 0.01f, 5, 0.1f, WIDTH, HEIGHT };

		camP = &cam;

		//glfwSetMouseButtonCallback(window.window, mouseButtonCallback);

		Shader terrainVoxelShader("shaders/voxelInstancingTerrain.vert", "shaders/voxelInstancing.geom", "shaders/voxelInstancing.frag");
		Shader wireframeShader("shaders/wireframeInstancing.vert", "shaders/wireframeInstancing.geom", "shaders/wireframeInstancing.frag");
		Shader boundingBoxShader("shaders/octreeBoundingBox.vert", "shaders/octreeBoundingBox.geom", "shaders/octreeBoundingBox.frag");

		Scene scene;
		window.setScenePointer(&scene);
		Entity* cameraEntity = addCameraEntity(scene, &cam);

		TransformComp* markerCubeTransform = addMarkerCube(&wireframeShader, &cam);
		addOctreeCompEntity(scene, &terrainVoxelShader, &boundingBoxShader, &cam,
			getComponentOfEntity<TransformComp>(cameraEntity), markerCubeTransform);

		//raycastTestTransform = createRaycastTestEntity(&voxels, &terrainVoxelShader, &cam);
		

		// TODO: Make better benchmark
		// TODO: Make a deltaTime variable

		//scene.start();

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

			//// TODO: Make fixed update work
			scene.fixedUpdate();

			window.endLoop();

		}
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}
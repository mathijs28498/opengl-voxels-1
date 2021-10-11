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

void addOctreeCompEntity(Scene& scene, Shader* terrainVoxelShader, Shader* boundingBoxShader, Camera* cam, TransformComp* transform) {
	Entity* entity = new Entity;
	VoxelRendererComp* voxelRenderer = new VoxelRendererComp(terrainVoxelShader, cam, 0, 0);
	BoundingBoxRendererComp* boundingBoxRenderer = new BoundingBoxRendererComp(boundingBoxShader, cam, 0, 0, false);
	entity->insertComponent(new OctreeHandlerComp(voxelRenderer, boundingBoxRenderer, transform));
	addEntityEvent.notify(entity);

	Entity* entity2 = new Entity;
	entity2->insertComponent(new RayCastComp(cam));
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

VoxelRendererComp* markerCube;
TransformComp* createMarkerCube(Shader* wireframeShader, Camera* cam) {
	Voxel voxel = { 0x01000000, 0xFF0000FF };

	uint32_t VAO = createVoxelVAO(&voxel, 1);

	Entity* entity = new Entity();

	markerCube = new VoxelRendererComp(wireframeShader, cam, VAO, 1);
	TransformComp* transform = new TransformComp({ 0, 0, 0 });
	entity->insertComponent(markerCube);
	entity->insertComponent(transform);
	addEntityEvent.notify(entity);

	return transform;
}

struct VoxelAABB {
	glm::vec3 min, max;
}; 

glm::vec3 getVoxelPosition(const Voxel& voxel, const glm::vec3& modelPosition) {
	glm::vec3 position = {
		voxel.positionInt & 0xFF,
		(voxel.positionInt >> 8) & 0xFF,
		(voxel.positionInt >> 16) & 0xFF,
	};

	return position * VOX_SIZE + modelPosition;
}

VoxelAABB getVoxelAABB(Voxel& voxel, glm::vec3& modelPosition) {
	glm::vec3 pos = getVoxelPosition(voxel, modelPosition);
	float sizeMult = std::pow(2, (voxel.positionInt >> 24) - 1);

	VoxelAABB voxelAABB = {
		pos - VOX_SIZE / 2,
		pos + VOX_SIZE * sizeMult - VOX_SIZE / 2,
	};
	return voxelAABB;
}

void swap(float& a, float& b) {
	float temp = a;
	a = b;
	b = temp;
}

float rayVoxelIntersect(const VoxelAABB& aabb, const Ray& r) {
	float t0 = (aabb.min.x - r.origin.x) / r.direction.x;
	float t1 = (aabb.max.x - r.origin.x) / r.direction.x;

	float t2 = (aabb.min.y - r.origin.y) / r.direction.y;
	float t3 = (aabb.max.y - r.origin.y) / r.direction.y;

	float t4 = (aabb.min.z - r.origin.z) / r.direction.z;
	float t5 = (aabb.max.z - r.origin.z) / r.direction.z;

	float tmin = std::max(std::max(std::min(t0, t1), std::min(t2, t3)), std::min(t4, t5));
	float tmax = std::min(std::min(std::max(t0, t1), std::max(t2, t3)), std::max(t4, t5));

	if (tmax < 0 || tmin > tmax) 
		return -1;

	if (tmin < 0) 
		return tmax;

	return tmin;
}

struct VoxelIntersect {
	size_t indexVoxel;
	Voxel* voxel;
	glm::vec3 intersecPoint;
};

bool doRayCast(std::vector<Voxel>* voxels, TransformComp* voxelTransform, Ray& ray, VoxelIntersect& intersecPointOut) {
	if (voxels->size() < 1)
		return false;

	std::vector<VoxelAABB> voxelAABBs;
	voxelAABBs.resize(voxels->size());

	float intersecDist = -1;
	size_t indexIntersected = -1;

	for (size_t i = 0; i < voxels->size(); i++) {
		VoxelAABB voxelAABB = getVoxelAABB((*voxels)[i], voxelTransform->position);
		float d = rayVoxelIntersect(voxelAABB, ray);

		if (d > 0 && (intersecDist < 0 || d < intersecDist)) {
			indexIntersected = i;
			intersecDist = d;
		}
	}

	bool isIntersected = intersecDist > 0;

	//VoxelAABB box = getVoxelAABB((*voxels)[3], voxelTransform->position);

	intersecPointOut = {
		indexIntersected,
		isIntersected ? &(*voxels)[indexIntersected] : nullptr,
		ray.origin + ray.direction * intersecDist
	};
	return isIntersected;
}

TransformComp* raycastTestTransform;
TransformComp* markerCubeTransform;
Camera* camP;

std::vector<Voxel> voxels = {
			{ 0x01000000, 0xFFFFFF00 },
			{ 0x02020000, 0xFFFFFF00 },
			{ 0x03050000, 0xFFFFFF00 },
			{ 0x040A0000, 0xFFFFFF00 },
};

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		Ray ray = camP->getCameraRay(100);
		VoxelIntersect voxelIntersect;
		if (doRayCast(&voxels, raycastTestTransform, ray, voxelIntersect)) {
			markerCubeTransform->position = voxelIntersect.intersecPoint;
			voxels.erase(voxels.begin() + voxelIntersect.indexVoxel);
			editVoxelVAO(voxels.data(), voxels.size(), voxelRendererRaycastTest->VAO);
		}
	}

}


int main() {

	try {
		Window window = { WIDTH, HEIGHT, TITLE, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) };
		Camera cam = { window.window, 0.001f, 5, 0.1f, WIDTH, HEIGHT };

		camP = &cam;

		glfwSetMouseButtonCallback(window.window, mouseButtonCallback);

		Shader terrainVoxelShader("shaders/voxelInstancingTerrain.vert", "shaders/voxelInstancing.geom", "shaders/voxelInstancing.frag");
		Shader wireframeShader("shaders/voxelInstancingTerrain.vert", "shaders/wireframeInstancing.geom", "shaders/wireframeInstancing.frag");
		Shader boundingBoxShader("shaders/octreeBoundingBox.vert", "shaders/octreeBoundingBox.geom", "shaders/octreeBoundingBox.frag");

		Scene scene;
		window.setScenePointer(&scene);
		Entity* cameraEntity = addCameraEntity(scene, &cam);
		/*addOctreeCompEntity(scene, &terrainVoxelShader, &boundingBoxShader, &cam,
			static_cast<TransformComp*>(cameraEntity->getComponent(gcn(TransformComp()))));*/

		raycastTestTransform = createRaycastTestEntity(&voxels, &terrainVoxelShader, &cam);
		markerCubeTransform = createMarkerCube(&wireframeShader, &cam);
		

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
			//Ray ray = cam.getCameraRay(100);
			//doRayCast(&voxels, markerCubeTransform, ray);
			Ray ray = camP->getCameraRay(100);
			VoxelIntersect voxelIntersect;
			if (doRayCast(&voxels, raycastTestTransform, ray, voxelIntersect)) {
				Voxel newMarkerCube{ voxelIntersect.voxel->positionInt & 0xFF000000, 0xFF0000FF };
				editVoxelVAO(&newMarkerCube, 1, markerCube->VAO);
				markerCubeTransform->position = getVoxelPosition(*voxelIntersect.voxel, { 0, 0, 0 });
			} else {
				markerCubeTransform->position = voxelIntersect.intersecPoint;
			}

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
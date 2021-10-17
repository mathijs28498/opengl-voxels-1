#include "../Headers/ECS/OctreeECS.h"
#include "../Headers/Observer pattern/SceneObserver.h"
#include "../Headers/Global/GLHelperFunctions.h"

#include <glm/gtx/norm.hpp>
#include <mutex>
#include <chrono>
#include <array>

constexpr uint32_t LOD1 = 1;
constexpr uint32_t LOD2 = 2;
constexpr uint32_t LOD3 = 3;
//constexpr uint32_t LOD4 = 5;

std::mutex mtx;

void createVoxelTerrain(OctreeComp* octree, BoundingBoxRendererComp* renderer) {
	octree->octree.makeNoiseTerrain(octree->pos);
	octree->octree.calculateBoundingBoxVAO();
	octree->octree.fillBoundingBoxRenderer(renderer);
	//octree->terrainCreationPromise.set_value(true);
	octree->threadBusy = true;
}

void OctreeSystem::start(Entity* entity) {
	OctreeComp* octree = getComponentOfEntity<OctreeComp>(entity);
	TransformComp* transform = getComponentOfEntity<TransformComp>(entity);
	BoundingBoxRendererComp* renderer = getComponentOfEntity<BoundingBoxRendererComp>(entity);

	octree->center = {
		octree->pos[0] * octree->octree.size * VOX_SIZE,
		octree->pos[1] * octree->octree.size * VOX_SIZE,
		octree->pos[2] * octree->octree.size * VOX_SIZE,
	};

	transform->position = {
		(octree->pos[0] * octree->octree.size - octree->octree.size / 2) * VOX_SIZE,
		(octree->pos[1] * octree->octree.size - octree->octree.size / 2) * VOX_SIZE,
		(octree->pos[2] * octree->octree.size - octree->octree.size / 2) * VOX_SIZE,
	};

	octree->terrainCreationThread = std::thread(createVoxelTerrain, octree, renderer);
	std::future<bool> future = octree->terrainCreationPromise.get_future();
}

void OctreeSystem::update(Entity* entity) {
	OctreeComp* octree = getComponentOfEntity<OctreeComp>(entity);
	if (!octree->threadBusy)
		return;

	bool useLOD = false;

	TransformComp* transform = getComponentOfEntity<TransformComp>(entity);
	VoxelRendererComp* renderer = getComponentOfEntity<VoxelRendererComp>(entity);

	float distThreshold0 = octree->octree.size * VOX_SIZE;
	float distThreshold1 = distThreshold0 * 3;
	float distSq = glm::distance2(octree->center, octree->cameraTransform->position);

	if (distSq < distThreshold0 * distThreshold0 || !useLOD) {
		if (octree->currentLOD != LOD1) {
			octree->currentLOD = LOD1;
			octree->octree.fillVoxelRenderer(renderer, octree->currentLOD);
		}
	} else if (distSq < distThreshold1 * distThreshold1) {
		if (octree->currentLOD != LOD2) {
			octree->currentLOD = LOD2;
			octree->octree.fillVoxelRenderer(renderer, octree->currentLOD);
		}
	} else {
		if (octree->currentLOD != LOD3) {
			octree->currentLOD = LOD3;
			octree->octree.fillVoxelRenderer(renderer, octree->currentLOD);
		}
	}
}

bool isChunkActive(std::vector<int32_t> curGridPos, OctreeHandlerComp* octreeHandler) {
	for (size_t i = 0; i < octreeHandler->chunks.size(); i++) {
		std::vector<int32_t> chunkGridPos = octreeHandler->chunks[i]->pos;
		if (curGridPos[0] == chunkGridPos[0] && curGridPos[1] == chunkGridPos[1] && curGridPos[2] == chunkGridPos[2]) {
			return true;
		}
	}
	return false;
}

void OctreeHandlerSystem::update(Entity* entity) {
	OctreeHandlerComp* octreeHandler = getComponentOfEntity<OctreeHandlerComp>(entity);
	glm::vec3 camPos = octreeHandler->cameraTransform->position;
	glm::vec3 tempCamPos = { camPos.x, 0, camPos.z };


	static bool tempBool = false;
	std::vector<int32_t> curGridPos = {
		(int32_t)floor((camPos.x + (OCTREE_SIZE / 2 * VOX_SIZE)) / (OCTREE_SIZE * VOX_SIZE)), 0, (int32_t)floor((camPos.z + (OCTREE_SIZE / 2 * VOX_SIZE)) / (OCTREE_SIZE * VOX_SIZE)),
	};
	if (!isChunkActive(curGridPos, octreeHandler)) {
		tempBool = true;
		Entity* entity = new Entity;
		OctreeComp* octreeComp = new OctreeComp(curGridPos, OCTREE_SIZE, octreeHandler->cameraTransform);
		entity->insertComponent(octreeComp);
		entity->insertComponent(new VoxelRendererComp(octreeHandler->voxelRenderer->shader, octreeHandler->voxelRenderer->camera, 0, 0));
		entity->insertComponent(new BoundingBoxRendererComp(octreeHandler->boundingBoxRenderer->shader, octreeHandler->voxelRenderer->camera, 0, 0, false));
		entity->insertComponent(new KeyInputComp());
		entity->insertComponent(new TransformComp());
		octreeHandler->chunks.push_back(octreeComp);
		addEntityEvent.notify(entity);
	}
}


void RayCastSystem::fixedUpdate(Entity* entity) {
	RayCastComp* rayCast = getComponentOfEntity<RayCastComp>(entity);
	KeyInputComp* keyInput = getComponentOfEntity<KeyInputComp>(entity);

	if (keyInput->keyRepeat[GLFW_KEY_Q] || true) {
		std::string octreeCompName = gcn(OctreeComp());

		Ray ray = rayCast->cam->getCameraRay(10);

		std::vector<Entity*> entities;
		getEntitiesWithComponentEvent.notify(&entities, octreeCompName);

		std::vector<Octree*> octrees(entities.size());

		VoxelCollision collision;
		bool isCollision = false;
		glm::vec3 collisionOctreePos;
		for (size_t i = 0; i < entities.size(); i++) {
			OctreeComp* octreeComp = (OctreeComp*)entities[i]->getComponent(octreeCompName);
			glm::vec3 octreePos = { octreeComp->pos[0], octreeComp->pos[1], octreeComp->pos[2] };
			if (octreeComp->octree.rayCastCollision(ray, octreePos, collision)) {
				if (!isCollision) isCollision = true;
				collisionOctreePos = octreePos;
			}
		}

		if (isCollision) {
			Voxel newMarkerCube{ collision.voxel->positionInt & 0xFF000000, 0xFF0000FF };
			//editVoxelVAO(&newMarkerCube, 1, markerCube->VAO);
			rayCast->markerCubeTransform->position = collision.voxel->getModelPosition(getRealOctreePos(collisionOctreePos));
		} else {
			// TODO: Remove marker cube when not collided
			//collision->position = collision.intersecPoint;
		}

	}

}

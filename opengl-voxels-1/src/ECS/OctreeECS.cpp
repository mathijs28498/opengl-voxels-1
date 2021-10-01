#include "../Headers/ECS/OctreeECS.h"
#include "../Headers/Observer pattern/SceneObserver.h"

#include <glm/gtx/norm.hpp>
#include <mutex>
#include <chrono>

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
	OctreeComp* octree = getComponentFromEntity<OctreeComp>(entity);
	TransformComp* transform = getComponentFromEntity<TransformComp>(entity);
	BoundingBoxRendererComp* renderer = getComponentFromEntity<BoundingBoxRendererComp>(entity);

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
	OctreeComp* octree = getComponentFromEntity<OctreeComp>(entity);
	if (!octree->threadBusy)
		return;

	bool useLOD = false;

	TransformComp* transform = getComponentFromEntity<TransformComp>(entity);
	VoxelRendererComp* renderer = getComponentFromEntity<VoxelRendererComp>(entity);

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
	OctreeHandlerComp* octreeHandler = getComponentFromEntity<OctreeHandlerComp>(entity);
	glm::vec3 camPos = octreeHandler->cameraTransform->position;
	glm::vec3 tempCamPos = { camPos.x, 0, camPos.z };


	static bool tempBool = false;
	std::vector<int32_t> curGridPos = {
		(int32_t)floor((camPos.x + (256 / 2 * VOX_SIZE)) / (256 * VOX_SIZE)), 0, (int32_t)floor((camPos.z + (256 / 2 * VOX_SIZE)) / (256 * VOX_SIZE)),
	};
	if (!isChunkActive(curGridPos, octreeHandler)) {
		tempBool = true;
		Entity* entity = new Entity;
		OctreeComp* octreeComp = new OctreeComp(curGridPos, 256, octreeHandler->cameraTransform);
		entity->insertComponent(octreeComp);
		entity->insertComponent(new VoxelRendererComp(octreeHandler->voxelRenderer->shader, octreeHandler->voxelRenderer->camera, 0, 0));
		entity->insertComponent(new BoundingBoxRendererComp(octreeHandler->boundingBoxRenderer->shader, octreeHandler->voxelRenderer->camera, 0, 0, false));
		entity->insertComponent(new KeyInputComp());
		entity->insertComponent(new TransformComp());
		octreeHandler->chunks.push_back(octreeComp);
		addEntityEvent.notify(entity);
	}
}
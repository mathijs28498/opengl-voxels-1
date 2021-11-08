#include "../Headers/ECS/OctreeECS.h"
#include "../Headers/Observer pattern/SceneObserver.h"
#include "../Headers/Global/GLHelperFunctions.h"

#include <glm/gtx/norm.hpp>
#include <mutex>
#include <chrono>
#include <array>

#define LOD1 1
#define LOD2 2
#define LOD3 3

int chunkRange = 0;

std::mutex mtx;
std::vector<Octree*> getOctreesFromEntities(const std::vector<Entity*> chunkEntities) {
	std::vector<Octree*> octrees(chunkEntities.size());

	for (size_t i = 0; i < chunkEntities.size(); i++) {
		octrees[i] = &getComponentOfEntity<OctreeComp>(chunkEntities[i])->octree;
	}

	return octrees;
}

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

bool isChunkActive(std::array<int32_t, 3> curGridPos, OctreeHandlerComp* octreeHandler) {
	for (Entity* chunkEntity : octreeHandler->chunkEntities) {
		OctreeComp* chunk = getComponentOfEntity<OctreeComp>(chunkEntity);
		std::array<int32_t, 3> chunkGridPos = chunk->pos;
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
	std::vector<int32_t> curGridPos = {
				(int32_t)floor((camPos.x + (OCTREE_SIZE / 2 * VOX_SIZE)) / (OCTREE_SIZE * VOX_SIZE)), 0, (int32_t)floor((camPos.z + (OCTREE_SIZE / 2 * VOX_SIZE)) / (OCTREE_SIZE * VOX_SIZE)),
	};
	for (int x = -chunkRange; x <= chunkRange; x++) {
		for (int z = -chunkRange; z <= chunkRange; z++) {
			std::array<int32_t, 3> curGridPosOffset = { curGridPos[0] + x, curGridPos[1], curGridPos[2] + z };

			if (!isChunkActive(curGridPosOffset, octreeHandler)) {
				Entity* entity = new Entity;
				OctreeComp* octreeComp = new OctreeComp(curGridPosOffset, OCTREE_SIZE, octreeHandler->cameraTransform);
				octreeComp->octree.setSiblings(getOctreesFromEntities(octreeHandler->chunkEntities));
				entity->insertComponent(octreeComp);
				entity->insertComponent(new VoxelRendererComp(octreeHandler->voxelRenderer->shader, octreeHandler->voxelRenderer->camera, 0, 0));
				entity->insertComponent(new BoundingBoxRendererComp(octreeHandler->boundingBoxRenderer->shader, octreeHandler->voxelRenderer->camera, 0, 0, false));
				entity->insertComponent(new KeyInputComp());
				entity->insertComponent(new TransformComp());
				octreeHandler->chunkEntities.push_back(entity);
				addEntityEvent.notify(entity);
			}
		}
	}
}


void RayCastSystem::fixedUpdate(Entity* entity) {
	RayCastComp* rayCast = getComponentOfEntity<RayCastComp>(entity);
	KeyInputComp* keyInput = getComponentOfEntity<KeyInputComp>(entity);
	
	std::string octreeCompName = gcn(OctreeComp());
	std::string voxelRendererCompName = gcn(VoxelRendererComp());

	Ray ray = rayCast->cam->getCameraRay(1000);

	std::vector<Entity*> entities;
	getEntitiesWithComponentEvent.notify(&entities, octreeCompName);

	std::vector<Octree*> octrees(entities.size());

	VoxelCollision collision;
	bool isCollision = false;
	glm::vec3 collisionOctreePos;
	OctreeComp* collisionOctreeComp = nullptr;
	VoxelRendererComp* collisionVoxelRendererComp = nullptr;
	for (size_t i = 0; i < entities.size(); i++) {
		VoxelRendererComp* voxelRendererComp = (VoxelRendererComp*)entities[i]->getComponent(voxelRendererCompName);
		if (voxelRendererComp->voxelAmount == 0)
			continue;

		OctreeComp* octreeComp = (OctreeComp*)entities[i]->getComponent(octreeCompName);

		glm::vec3 octreePos = { octreeComp->pos[0], octreeComp->pos[1], octreeComp->pos[2] };
		if (octreeComp->octree.rayCastCollision(ray, octreePos, collision)) {
			if (!isCollision) isCollision = true;
			collisionOctreePos = octreePos;
			collisionOctreeComp = (OctreeComp*)entities[i]->getComponent(octreeCompName);
			collisionVoxelRendererComp = voxelRendererComp;
		}
	}

	if (isCollision) {
		Voxel newMarkerCube{ collision.voxel->positionInt & 0xFF000000, 0xFF0000FF };
		rayCast->markerCubeTransform->position = collision.voxel->getModelPosition(getRealOctreePos(collisionOctreePos));
	} else {
		// TODO: Remove marker cube when not collided
	}

	if ((keyInput->keyRepeat[GLFW_KEY_E] || keyInput->keyPress[GLFW_KEY_Q]) && collisionOctreeComp != nullptr) {
		float power = 10;
		collisionOctreeComp->octree.removeVoxels(intToBytes3(collision.voxel->positionInt), power);
		collisionOctreeComp->octree.fillVoxelRenderer(collisionVoxelRendererComp, 1, true);
	}
}

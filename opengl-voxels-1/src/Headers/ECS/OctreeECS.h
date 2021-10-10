#pragma once
#include "Component.h"
#include "SustainedSystem.h"
#include "EventSystem.h"
#include "../Objects/Octree.h"

#include <list>
#include <thread>
#include <future>

#include <iostream>

struct OctreeComp : public Component {
	OctreeComp() {};
	OctreeComp(std::vector<int> pos, uint32_t size, TransformComp* cameraTransform) :
		pos(pos), octree(Octree(pos, size)), cameraTransform(cameraTransform), currentLOD(0) {};
	std::vector<int> pos;
	glm::vec3 center;
	Octree octree;
	TransformComp* cameraTransform;
	uint32_t currentLOD;

	bool threadBusy = false;
	std::thread terrainCreationThread; 
	std::promise<bool> terrainCreationPromise;
	std::future<bool> terrainCreationFuture;
};

// TODO: Make the renderer comps as seperate components, not in this one
struct OctreeHandlerComp : public Component {
	OctreeHandlerComp() {};
	OctreeHandlerComp(VoxelRendererComp* VoxelRenderer, BoundingBoxRendererComp* boundingBoxRenderer, TransformComp* cameraTransform) :
		voxelRenderer(VoxelRenderer), boundingBoxRenderer(boundingBoxRenderer), cameraTransform(cameraTransform) {};
	VoxelRendererComp* voxelRenderer;
	BoundingBoxRendererComp* boundingBoxRenderer;
	TransformComp* cameraTransform;
	std::vector<OctreeComp*> chunks;
};

struct RayCastComp : public Component {
	RayCastComp() {};
	RayCastComp(Camera* cam) : cam(cam) {};
	
	Camera* cam;
};

class OctreeHandlerSystem : public SustainedSystem {
public:
	OctreeHandlerSystem() : SustainedSystem({ gcn(OctreeHandlerComp()) }) {}

	void update(Entity* entity);
};

class OctreeSystem : public SustainedSystem {
public:
	OctreeSystem() : SustainedSystem({ gcn(TransformComp()), gcn(OctreeComp()) , gcn(VoxelRendererComp()), gcn(BoundingBoxRendererComp()) }) {};

	void start(Entity* entity);
	void update(Entity* entity);
};

class RayCastSystem : public SustainedSystem {
public:
	RayCastSystem() : SustainedSystem({ gcn(RayCastComp()), gcn(KeyInputComp()) }) {};

	void fixedUpdate(Entity* entity);
};
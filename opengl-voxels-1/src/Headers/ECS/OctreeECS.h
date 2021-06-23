#pragma once
#include "Component.h"
#include "SustainedSystem.h"
#include "EventSystem.h"
#include "../Objects/Octree.h"

#include <list>

struct OctreeComp : public Component {
	OctreeComp() {};
	OctreeComp(std::vector<int> pos, uint32_t size, TransformComp* cameraTransform) :
		pos(pos), octree(Octree(pos, size)), cameraTransform(cameraTransform), currentLOD(0) {};
	std::vector<int> pos;
	glm::vec3 center;
	Octree octree;
	TransformComp* cameraTransform;
	uint32_t currentLOD;
};

struct OctreeHandlerComp : public Component {
	OctreeHandlerComp() {};
	OctreeHandlerComp(VoxelRendererComp* VoxelRenderer, BoundingBoxRendererComp* boundingBoxRenderer, TransformComp* cameraTransform) :
		voxelRenderer(VoxelRenderer), boundingBoxRenderer(boundingBoxRenderer), cameraTransform(cameraTransform) {};
	VoxelRendererComp* voxelRenderer;
	BoundingBoxRendererComp* boundingBoxRenderer;
	TransformComp* cameraTransform;
	std::vector<OctreeComp*> chunks;
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
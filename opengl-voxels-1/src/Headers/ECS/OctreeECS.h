#pragma once
#include "Component.h"
#include "SustainedSystem.h"
#include "EventSystem.h"
#include "../Objects/Octree.h"

struct OctreeComp : public Component {
	OctreeComp() {};
	OctreeComp(std::vector<int> pos, uint32_t size, TransformComp* cameraTransform) : 
		octree(Octree(pos, size)), cameraTransform(cameraTransform), currentLOD(0) {};
	Octree octree;
	TransformComp* cameraTransform;
	uint32_t currentLOD;
};

class OctreeSystem : public SustainedSystem {
public:
	OctreeSystem() : SustainedSystem({ gcn(TransformComp()), gcn(OctreeComp()) , gcn(VoxelRendererComp()) }) {};

	void start(Entity* entity);
	void update(Entity* entity);
};

class OctreeInsertSystem : public EventSystem {
public:
	OctreeInsertSystem() : EventSystem({ gcn(OctreeComp()) }) {};
};
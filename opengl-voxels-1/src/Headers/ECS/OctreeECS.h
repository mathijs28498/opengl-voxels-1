#pragma once
#include "Component.h"
#include "SustainedSystem.h"
#include "EventSystem.h"
#include "../Objects/Octree.h"

#include <list>

struct OctreeComp : public Component {
	OctreeComp() {};
	OctreeComp(std::vector<int> pos, uint32_t size, TransformComp* cameraTransform) :
		octree(Octree(pos, size)), cameraTransform(cameraTransform), currentLOD(0) {};
	Octree octree;
	TransformComp* cameraTransform;
	uint32_t currentLOD;
};

struct OctreeHandlerComp : public Component {
	OctreeHandlerComp() {};
	OctreeHandlerComp(TransformComp* cameraComp) : cameraComp(cameraComp) {};
	TransformComp* cameraComp;
	std::list<OctreeComp*> chunks;
};

class OctreeHandlerSystem : public SustainedSystem {
public:
	OctreeHandlerSystem() : SustainedSystem({ gcn(OctreeHandlerSystem()) }) {};

	void update(Entity* entity);
};

class OctreeSystem : public SustainedSystem {
public:
	OctreeSystem() : SustainedSystem({ gcn(TransformComp()), gcn(OctreeComp()) , gcn(VoxelRendererComp()), gcn(BoundingBoxRendererComp()) }) {};

	void start(Entity* entity);
	void update(Entity* entity);
};
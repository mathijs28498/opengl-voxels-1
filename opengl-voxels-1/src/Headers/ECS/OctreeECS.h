#pragma once
#include "Component.h"
#include "SustainedSystem.h"
#include "EventSystem.h"
#include "../Objects/Octree.h"

struct OctreeComp : public Component {
	OctreeComp() {};
	OctreeComp(std::vector<int> pos, uint32_t octreeSize) : pos(pos), octreeSize(octreeSize) {};
	std::vector<int> pos;
	uint32_t octreeSize;
};

class OctreeSystem : public SustainedSystem {
public:
	OctreeSystem() : SustainedSystem({ gcn(TransformComp()), gcn(OctreeComp()) , gcn(VoxelRendererComp()) }) {};

	void SustainedSystem::start(Entity* entity);
	void SustainedSystem::update(Entity* entity);
	void SustainedSystem::fixedUpdate(Entity* entity) {}
};

class OctreeInsertSystem : public EventSystem {
public:
	OctreeInsertSystem() : EventSystem({ gcn(OctreeComp()) }) {};
};
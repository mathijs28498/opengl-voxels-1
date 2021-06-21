#pragma once
#include "Component.h"
#include "SustainedSystem.h"
#include "EventSystem.h"

struct OctreeComp : public Component {
	OctreeComp() {};
};

class OctreeSystem : public SustainedSystem {
public:
	OctreeSystem() : SustainedSystem({ typeid(OctreeComp).name() , typeid(VoxelRendererComp).name() }) {};

	void SustainedSystem::start(Entity* entity);
	void SustainedSystem::update(Entity* entity);
	void SustainedSystem::fixedUpdate(Entity* entity) {}
};

class OctreeInsertSystem : public EventSystem {
public:
	OctreeInsertSystem() : EventSystem({ typeid(OctreeComp).name(), typeid(VoxelRendererComp).name() }) {};
};
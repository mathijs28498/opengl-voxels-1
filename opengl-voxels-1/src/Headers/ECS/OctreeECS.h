#pragma once
#include "Component.h"
#include "SustainedSystem.h"
#include "EventSystem.h"

struct OctreeComp : public Component {

};

class OctreeSystem : public SustainedSystem {
	OctreeSystem() : SustainedSystem({ typeid(OctreeComp).name() , typeid(VoxelRendererComp).name() }) {};

	void SustainedSystem::start(Entity* entity);
	void SustainedSystem::update(Entity* entity);
	void SustainedSystem::fixedUpdate(Entity* entity) {}
};

class OctreeInputSystem : public EventSystem {
	OctreeInputSystem() : EventSystem({ typeid(TransformComp).name(), typeid(VoxelRendererComp).name() }) {};
};
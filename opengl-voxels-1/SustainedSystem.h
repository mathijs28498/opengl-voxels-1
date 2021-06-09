#pragma once

#include "Component.h"
#include "System.h"

#include <vector>

// TODO: Make systems that are only called on events
class SustainedSystem : public System {
protected:
	SustainedSystem(std::vector<std::string> requirements) : System(requirements) {};
};

class VoxelRenderSystem : public SustainedSystem {
public:
	VoxelRenderSystem() : SustainedSystem({ typeid(Transform).name(), typeid(VoxelRenderer).name() }) {};

protected:
	void SustainedSystem::start(Entity* entity);
	void SustainedSystem::update(Entity* entity);
	void SustainedSystem::fixedUpdate(Entity* entity);
};

class MainCameraSystem : public SustainedSystem {
public:
	MainCameraSystem() : SustainedSystem({ typeid(MainCamera).name() }) {};

protected:
	void SustainedSystem::start(Entity* entity);
	void SustainedSystem::update(Entity* entity);
	void SustainedSystem::fixedUpdate(Entity* entity);
};
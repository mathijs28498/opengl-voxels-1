#pragma once

#include "Component.h"
#include "System.h"

#include <vector>

// TODO: Make systems that are only called on events
class SustainedSystem : public System {
public:
	void doStart(Entity* entity) {
		if (hasComponents(entity))
			start(entity);
	};

	void doUpdate(Entity* entity) {
		if (hasComponents(entity))
			update(entity);
	};

	void doFixedUpdate(Entity* entity) {
		if (hasComponents(entity))
			update(entity);
	};

protected:
	SustainedSystem(std::vector<std::string> requirements) : System(requirements) {};

protected:
	virtual void start(Entity* entity) = 0;
	virtual void update(Entity* entity) = 0;
	virtual void fixedUpdate(Entity* entity) = 0;
};

// TODO: Make it possible to just put a component name
class VoxelRenderSystem : public SustainedSystem {
public:
	VoxelRenderSystem() : SustainedSystem({ typeid(TransformComp).name(), typeid(VoxelRendererComp).name() }) {};

protected:
	void SustainedSystem::start(Entity* entity) {}
	void SustainedSystem::update(Entity* entity);
	void SustainedSystem::fixedUpdate(Entity* entity) {}
};

class BoundingBoxRendererSystem : public SustainedSystem {
public:
	BoundingBoxRendererSystem() : SustainedSystem({ typeid(TransformComp).name(), typeid(BoundingBoxRendererComp).name(), typeid(KeyInputComp).name() }) {};

protected:
	void SustainedSystem::start(Entity* entity) {}
	void SustainedSystem::update(Entity* entity);
	void SustainedSystem::fixedUpdate(Entity* entity) {}
};

class CameraMoveSystem : public SustainedSystem {
public:
	// TODO: Make it so you only have to put class name as argument
	CameraMoveSystem() : SustainedSystem({ typeid(CameraComp).name(), typeid(KeyInputComp).name() }) {};

protected:
	void SustainedSystem::start(Entity* entity) {}
	void SustainedSystem::update(Entity* entity);
	void SustainedSystem::fixedUpdate(Entity* entity) {}
};

class ResetKeyInputSystem : public SustainedSystem {
public:
	ResetKeyInputSystem() : SustainedSystem({ typeid(KeyInputComp).name() }) {};

protected:
	void SustainedSystem::start(Entity* entity) {}
	void SustainedSystem::update(Entity* entity);
	void SustainedSystem::fixedUpdate(Entity* entity) {}
};


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
			fixedUpdate(entity);
	};

protected:
	SustainedSystem(std::vector<std::string> requirements) : System(requirements) {};

protected:
	virtual void start(Entity* entity) {}
	virtual void update(Entity* entity) {}
	virtual void fixedUpdate(Entity* entity) {}
};

// TODO: Make it possible to just put a component name
class VoxelRenderSystem : public SustainedSystem {
public:
	VoxelRenderSystem() : SustainedSystem({ gcn(TransformComp()),  gcn(VoxelRendererComp()) }) {};

protected:
	void start(Entity* entity) {}
	void update(Entity* entity);
	void fixedUpdate(Entity* entity) {}
};

class BoundingBoxRendererSystem : public SustainedSystem {
public:
	BoundingBoxRendererSystem() : SustainedSystem({ gcn(TransformComp()), gcn(BoundingBoxRendererComp()), gcn(KeyInputComp()) }) {};

protected:
	void start(Entity* entity) {}
	void update(Entity* entity);
	void fixedUpdate(Entity* entity) {}
};

class CameraMoveSystem : public SustainedSystem {
public:
	// TODO: Make it so you only have to put class name as argument
	CameraMoveSystem() : SustainedSystem({ gcn(CameraComp()), gcn(KeyInputComp()), gcn(TransformComp()) }) {};

protected:
	void start(Entity* entity);
	void update(Entity* entity);
};

class ResetKeyInputSystem : public SustainedSystem {
public:
	ResetKeyInputSystem() : SustainedSystem({ gcn(KeyInputComp()) }) {};

protected:
	void update(Entity* entity);
};


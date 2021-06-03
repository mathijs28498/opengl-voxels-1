#pragma once

#include "Component.h"

#include <vector>

// TODO: Make systems that are only called on events
class System {
public:
	void doStart(Entity* entity) {
		if (hasComponents(entity))
			start(entity);
	};
	
	void doUpdate(Entity* entity) {
		if (hasComponents(entity))
			update(entity);
	};

protected:
	System(std::vector<std::string> requirements) : requirements(requirements) {};
	std::vector<std::string> requirements;

	virtual void start(Entity* entity) = 0;
	virtual void update(Entity* entity) = 0;

private:
	bool hasComponents(Entity* entity) {
		for (std::string requirement: requirements) {
			if (!entity->hasRequirement(requirement))
				return false;
		}
		return true;
	}
};

class VoxelRenderSystem : public System {
public:
	VoxelRenderSystem() : System({ typeid(Transform).name(), typeid(VoxelRenderer).name() }) {};

protected:
	void System::start(Entity* entity);
	void System::update(Entity* entity);
};

class MainCameraSystem : public System {
public:
	MainCameraSystem() : System({ typeid(MainCamera).name() }) {};

protected:
	void System::start(Entity* entity);
	void System::update(Entity* entity);
};
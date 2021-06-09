#pragma once
#include "Component.h"

#include <vector>

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

	void doFixedUpdate(Entity* entity) {
		if (hasComponents(entity))
			update(entity);
	};

protected:
	System(std::vector<std::string> requirements) : requirements(requirements) {};
	std::vector<std::string> requirements;

	virtual void start(Entity* entity) = 0;
	virtual void update(Entity* entity) = 0;
	virtual void fixedUpdate(Entity* entity) = 0;

private:
	bool hasComponents(Entity* entity) {
		for (std::string requirement : requirements) {
			if (!entity->hasRequirement(requirement))
				return false;
		}
		return true;
	}
};
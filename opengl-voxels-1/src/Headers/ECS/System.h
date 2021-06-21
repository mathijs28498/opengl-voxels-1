#pragma once
#include "Component.h"

#include <vector>

class System {
protected:
	System(std::vector<std::string> requirements) : requirements(requirements) {};
	std::vector<std::string> requirements;

	bool hasComponents(Entity* entity) {
		for (std::string requirement : requirements) {
			if (!entity->hasRequirement(requirement))
				return false;
		}
		return true;
	}
};
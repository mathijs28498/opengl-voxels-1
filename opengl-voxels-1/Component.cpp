#include "Component.h"

#include <stdexcept>

void Entity::insertComponent(Component* component) {
	if (components.count(component->type))
		throw std::runtime_error("Entity already contains " + component->type + " component");

	components.insert({ component->type, component });
};

bool Entity::hasRequirement(std::string requirement) {
	return components.count(requirement);
};

Component* Entity::getComponent(std::string requirement) {
	return components[requirement];
};
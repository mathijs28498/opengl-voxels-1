#include "Component.h"

#include <stdexcept>

void Entity::insertComponent(Component* component) {
	if (components.count(typeid(*component).name()))
		throw std::runtime_error("Entity already contains component");

	components.insert({ typeid(*component).name(), component });
};

bool Entity::hasRequirement(std::string requirement) {
	return components.count(requirement);
};

Component* Entity::getComponent(std::string requirement) {
	return components[requirement];
};

void MainCamera::setCamera(CameraComp* camera) {
	if (components.count(typeid(CameraComp).name()))
		components.insert({ typeid(CameraComp).name(), camera });
	else
		components[typeid(CameraComp).name()] = camera;
}

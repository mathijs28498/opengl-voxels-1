#pragma once

#include "Component.h"
#include "System.h"

#include <GLFW/glfw3.h>
#include <vector>

class EventSystem : public System {
public:
	EventSystem(std::vector<std::string> requirements) : System(requirements) {}
};

class KeyboardSystem : public EventSystem {
public:
	KeyboardSystem() : EventSystem({ typeid(KeyInput).name() }) {}

	void doEvent(Entity* entity, GLFWwindow* window, int key, int action) {
		if (hasComponents(entity))
			event(entity, window, key, action);
	};

private:
	void event(Entity* entity, GLFWwindow* window, int key, int action);
};


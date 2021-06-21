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
	KeyboardSystem() : EventSystem({ gcn(KeyInputComp()) }) {}

	void doEvent(Entity* entity, int key, int action) {
		if (hasComponents(entity))
			event(entity, key, action);
	};

private:
	void event(Entity* entity, int key, int action);
};

class CameraMouseCursorSystem : public EventSystem {
public:
	CameraMouseCursorSystem() : EventSystem({ gcn(CameraComp()), gcn(MouseCursorInputComp()) }) {}

	void doEvent(Entity* entity, double xpos, double ypos) {
		if (hasComponents(entity))
			event(entity, xpos, ypos);
	};

private:
	void event(Entity* entity, double xpos, double ypos);
};


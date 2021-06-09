#include "EventSystem.h"

void KeyboardSystem::event(Entity* entity, GLFWwindow* window, int key, int action) {
	KeyInput* keyInput = getComponentFromEntity<KeyInput>(entity);

	if (action == GLFW_PRESS) {
		keyInput->keyPress[key] = true;
		keyInput->keyRepeat[key] = true;
	} else if (action == GLFW_RELEASE) {
		keyInput->keyRelease[key] = true;
		keyInput->keyRepeat[key] = false;
	}
}
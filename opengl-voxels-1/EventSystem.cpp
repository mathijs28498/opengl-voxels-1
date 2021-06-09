#include "EventSystem.h"

void KeyboardSystem::event(Entity* entity, int key, int action) {
	KeyInput* keyInput = getComponentFromEntity<KeyInput>(entity);

	if (action == GLFW_PRESS) {
		keyInput->keyPress[key] = true;
		keyInput->keyRepeat[key] = true;
	} else if (action == GLFW_RELEASE) {
		keyInput->keyRelease[key] = true;
		keyInput->keyRepeat[key] = false;
	}
}

void CameraMouseCursorSystem::event(Entity* entity, double xpos, double ypos) {
	CameraComp* cameraComp = getComponentFromEntity<CameraComp>(entity);
	MouseCursorInput* mouseInput = getComponentFromEntity<MouseCursorInput>(entity);
	Camera* cam = cameraComp->camera;

	// Make sure the mouseInput position is properly set the first movement
	glm::vec2 newPos(xpos, ypos);
	if (mouseInput->pos == glm::vec2(-1, -1)) {
		mouseInput->pos = newPos;
		return;
	}

	glm::vec2 diff = mouseInput->pos - newPos;
	mouseInput->pos = newPos;

	cam->yaw -= diff.x * cam->mouseSensitivity;
	cam->pitch += diff.y * cam->mouseSensitivity;

	if (cam->pitch > 89.0f)
		cam->pitch = 89.0f;
	if (cam->pitch < -89.0f)
		cam->pitch = -89.0f;

	cam->cameraDir.x = cos(glm::radians(cam->yaw)) * cos(glm::radians(cam->pitch));
	cam->cameraDir.y = sin(glm::radians(cam->pitch));
	cam->cameraDir.z = sin(glm::radians(cam->yaw)) * cos(glm::radians(cam->pitch));
	cam->setViewMatrix();

	mouseInput->pos = newPos;
	/*mouseInput->xpos = xpos;
	mouseInput->ypos = ypos;*/
}
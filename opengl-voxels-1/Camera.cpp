#include "Camera.h"

Camera::Camera(GLFWwindow* window, glm::vec3 startPosition, float walkSpeed, float speedMultiplier, float mouseSensitivity, const int WIDTH, const int HEIGHT) {
	cameraPos = startPosition;
	normalWalkSpeed = walkSpeed;
	actualWalkSpeed = walkSpeed;
	this->speedMultiplier = speedMultiplier;
	this->mouseSensitivity = mouseSensitivity;
	this->width = WIDTH;
	this->height = HEIGHT;
	glfwSetWindowUserPointer(window, this);

	glfwSetCursorPosCallback(window, this->cursorPositionCallback);
	glfwSetScrollCallback(window, scrollCallback);

	setViewMatrix();
	setProjMatrix();
}

void Camera::setProjMatrix() {
	proj = glm::perspective(glm::radians(fov), (width + 0.0f) / height, 0.1f, 1000.0f);
}

void Camera::setViewMatrix() {
	glm::vec3 temp = cameraPos + cameraDir;
	view = glm::lookAt(cameraPos, cameraPos + cameraDir, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::setUniforms(const Shader* shader) {
	shader->setMat4("view", view);
	shader->setMat4("proj", proj);
	shader->setVec3("viewPos", cameraPos);
}

// TODO: Exchange glfwgettime for a deltatime thing
void Camera::moveCamera(GLFWwindow* window) {
	glm::vec3 addPos(0, 0, 0);
	bool camPosChanged = false;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		addPos += getForwardVec3();
		camPosChanged = true;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		addPos += getRightVec3();
		camPosChanged = true;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		addPos -= getForwardVec3();
		camPosChanged = true;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		addPos -= getRightVec3();
		camPosChanged = true;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		cameraPos.y -= actualWalkSpeed * static_cast<float>(glfwGetTime());
		camPosChanged = true;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		cameraPos.y += actualWalkSpeed * static_cast<float>(glfwGetTime());
		camPosChanged = true;
	}

	//// TODO: Make this a callback
	//if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
	//}

	if (camPosChanged) {
		if (glm::length(addPos) > 0.001f) cameraPos += glm::normalize(addPos) * static_cast<float>(actualWalkSpeed) * static_cast<float>(glfwGetTime());
		setViewMatrix();
	}
}

void Camera::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
	Camera* cam = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
	glm::vec2 newPos(xpos, ypos);
	if (cam->oldMousePos == glm::vec2(-1, -1)) {
		cam->oldMousePos = newPos;
		return;
	}

	glm::vec2 diff = cam->oldMousePos - newPos;
	cam->oldMousePos = newPos;

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
}

void Camera::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	Camera* cam = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
	cam->fov -= (float)yoffset;
	if (cam->fov < 1.0f)
		cam->fov = 1.0f;
	if (cam->fov > 45.0f)
		cam->fov = 45.0f;
	cam->setProjMatrix();
}
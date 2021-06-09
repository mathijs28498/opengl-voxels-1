#include "Camera.h"

Camera::Camera(GLFWwindow* window, glm::vec3 startPosition, float walkSpeed, float speedMultiplier, float mouseSensitivity, const int WIDTH, const int HEIGHT) {
	cameraPos = startPosition;
	normalWalkSpeed = walkSpeed;
	actualWalkSpeed = walkSpeed;
	this->speedMultiplier = speedMultiplier;
	this->mouseSensitivity = mouseSensitivity;
	this->width = WIDTH;
	this->height = HEIGHT;

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
	shader->setVec3("viewDir", cameraDir);
}
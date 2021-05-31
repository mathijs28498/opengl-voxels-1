#pragma once

#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

class Camera {
public:
	glm::vec3 cameraPos;
	glm::mat4 view;
	glm::mat4 proj;
	float fov = 45.0f;
	int width;
	int height;

	Camera() {};
	Camera(GLFWwindow* window, glm::vec3 startPosition, float walkSpeed, float mouseSensitivity, float speedMultiplier, const int WIDTH, const int HEIGHT);
	void setProjMatrix();
	void setViewMatrix();

	void setUniforms(const Shader* shader);
	void moveCamera(GLFWwindow* window);

	static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	void setWidthHeight(int width, int height) {
		this->width = width;
		this->height = height;
	}

private:
	float normalWalkSpeed;
	float speedMultiplier;
	float actualWalkSpeed;
	float mouseSensitivity;

	float yaw = -90.0f;
	float pitch = 0.0f;
	glm::vec2 oldMousePos = glm::vec2(-1.0f, -1.0f);
	glm::vec3 cameraDir = glm::vec3(0.0f, 0.0f, -1.0f);

	glm::vec3 getForwardVec3() {
		return glm::normalize(cameraDir * glm::vec3(1, 0, 1));
	}

	glm::vec3 getRightVec3() {
		return glm::cross(glm::vec3(0, 1, 0), getForwardVec3());
	}
};
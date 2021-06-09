#include "SustainedSystem.h"
#include "VertexData.h"

#include <glm/gtx/rotate_vector.hpp>

void VoxelRenderSystem::update(Entity* entity) {
    Transform* transform = getComponentFromEntity<Transform>(entity);
    VoxelRenderer* renderer = getComponentFromEntity<VoxelRenderer>(entity);

    renderer->shader->use();
    renderer->camera->setUniforms(renderer->shader);
    renderer->shader->setMat4("model", glm::translate(glm::mat4(1), transform->position));

    float mult = 0.5f;
    static float angle = -40;
    angle += 0.05f * mult;
    if (angle > 360 - 40)
        angle -= 360;
    float realAngle = angle;

    float lightIntensity = (angle + 30) / 120;
    if (lightIntensity > 1) lightIntensity = 2 - lightIntensity;
    if (lightIntensity < 0) {
        realAngle = 270;
        lightIntensity = 0;
        angle += 0.3f * mult;
    }

    renderer->shader->setVec3("lightDir", glm::rotate(glm::vec3(-1, 0, 0), glm::radians(90.0f), glm::vec3(-1, 0, 1)));
    renderer->shader->setFloat("lightIntensity", 1);

    renderer->shader->setFloat("voxSize", VOX_SIZE);

    glBindVertexArray(renderer->VAO);
    glDrawArrays(GL_POINTS, 0, renderer->voxelAmount);
    glBindVertexArray(0);
}

void CameraMoveSystem::update(Entity* entity) {
    CameraComp* mainCamera = getComponentFromEntity<CameraComp>(entity);
    KeyInput* keyInput = getComponentFromEntity<KeyInput>(entity);
	Camera* cam = mainCamera->camera;

	glm::vec3 addPos(0, 0, 0);
	bool camPosChanged = false;
	keyInput->keyPress[GLFW_KEY_W];
	if (keyInput->keyRepeat[GLFW_KEY_W]) {
		addPos += cam->getForwardVec3();
		camPosChanged = true;
	}
	if (keyInput->keyRepeat[GLFW_KEY_A]) {
		addPos += cam->getRightVec3();
		camPosChanged = true;
	}
	if (keyInput->keyRepeat[GLFW_KEY_S]) {
		addPos -= cam->getForwardVec3();
		camPosChanged = true;
	}
	if (keyInput->keyRepeat[GLFW_KEY_D]) {
		addPos -= cam->getRightVec3();
		camPosChanged = true;
	}

	if (keyInput->keyRepeat[GLFW_KEY_LEFT_SHIFT]) {
		cam->cameraPos.y -= cam->actualWalkSpeed * static_cast<float>(glfwGetTime());
		camPosChanged = true;
	}
	if (keyInput->keyRepeat[GLFW_KEY_SPACE]) {
		cam->cameraPos.y += cam->actualWalkSpeed * static_cast<float>(glfwGetTime());
		camPosChanged = true;
	}

	if (camPosChanged) {
		if (glm::length(addPos) > 0.001f) cam->cameraPos += glm::normalize(addPos) * static_cast<float>(cam->actualWalkSpeed) * static_cast<float>(glfwGetTime());
		cam->setViewMatrix();
	}
}

void ResetKeyInputSystem::update(Entity* entity) {
    KeyInput* keyInput = getComponentFromEntity<KeyInput>(entity);

    std::fill_n(keyInput->keyPress, GLFW_KEY_LAST, false);
    std::fill_n(keyInput->keyRelease, GLFW_KEY_LAST, false);
}
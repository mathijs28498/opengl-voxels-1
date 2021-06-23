#include "../Headers/ECS/SustainedSystem.h"
#include "../Headers/Global/VertexData.h"

#include <glm/gtx/rotate_vector.hpp>

void VoxelRenderSystem::update(Entity* entity) {
    TransformComp* transform = getComponentFromEntity<TransformComp>(entity);
    VoxelRendererComp* renderer = getComponentFromEntity<VoxelRendererComp>(entity);

    renderer->shader->use();
    renderer->camera->setUniforms(renderer->shader);
    renderer->shader->setMat4("model", glm::translate(glm::mat4(1), transform->position));

    float multiplier = 0.2f;
    static float angle = -40;
    angle += 0.05f * multiplier;
    if (angle > 360 - 40)
        angle -= 360;
    float realAngle = angle;

    float lightIntensity = (angle + 30) / 120;
    if (lightIntensity > 1) lightIntensity = 2 - lightIntensity;
    if (lightIntensity < 0) {
        realAngle = 270;
        lightIntensity = 0;
        angle += 0.3f * multiplier;
    }

    renderer->shader->setVec3("lightDir", glm::rotate(glm::vec3(-1, 0, 0), glm::radians(angle), glm::vec3(-1, 0, 1)));
    renderer->shader->setFloat("lightIntensity", 1);

    renderer->shader->setFloat("voxSize", VOX_SIZE);

    glBindVertexArray(renderer->VAO);
    glDrawArrays(GL_POINTS, 0, renderer->voxelAmount);
    glBindVertexArray(0);
}

void BoundingBoxRendererSystem::update(Entity* entity) {
	TransformComp* transform = getComponentFromEntity<TransformComp>(entity);
	BoundingBoxRendererComp* renderer = getComponentFromEntity<BoundingBoxRendererComp>(entity);
	KeyInputComp* keyInput = getComponentFromEntity<KeyInputComp>(entity);


	if (keyInput->keyPress[GLFW_KEY_T]) {
		renderer->show = !renderer->show;
	}

	if (renderer->show) {
		renderer->shader->use();
		renderer->camera->setUniforms(renderer->shader);

		renderer->shader->setMat4("model", glm::translate(glm::mat4(1), transform->position));
		renderer->shader->setFloat("voxSize", VOX_SIZE);
		renderer->shader->setVec3("color", 1, 1, 0);

		glBindVertexArray(renderer->VAO);
		glDrawArrays(GL_POINTS, 0, renderer->boundingBoxAmount);
		glBindVertexArray(0);
	}
}

void CameraMoveSystem::start(Entity* entity) {
	CameraComp* mainCamera = getComponentFromEntity<CameraComp>(entity);
	Camera* cam = mainCamera->camera;
	TransformComp* transform = getComponentFromEntity<TransformComp>(entity);

	cam->setViewMatrix(transform->position);
}

void CameraMoveSystem::update(Entity* entity) {
    CameraComp* mainCamera = getComponentFromEntity<CameraComp>(entity);
    KeyInputComp* keyInput = getComponentFromEntity<KeyInputComp>(entity);
	Camera* cam = mainCamera->camera;
	TransformComp* transform = getComponentFromEntity<TransformComp>(entity);

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
		transform->position.y -= cam->actualWalkSpeed * static_cast<float>(glfwGetTime());
		camPosChanged = true;
	}
	if (keyInput->keyRepeat[GLFW_KEY_SPACE]) {
		transform->position.y += cam->actualWalkSpeed * static_cast<float>(glfwGetTime());
		camPosChanged = true;
	}

	if (camPosChanged) {
		if (glm::length(addPos) > 0.001f) transform->position += glm::normalize(addPos) * static_cast<float>(cam->actualWalkSpeed) * static_cast<float>(glfwGetTime());
		cam->setViewMatrix(transform->position);
	}
}

void ResetKeyInputSystem::update(Entity* entity) {
    KeyInputComp* keyInput = getComponentFromEntity<KeyInputComp>(entity);

    std::fill_n(keyInput->keyPress, GLFW_KEY_LAST, false);
    std::fill_n(keyInput->keyRelease, GLFW_KEY_LAST, false);
}
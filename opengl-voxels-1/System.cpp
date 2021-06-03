#include "System.h"
#include "VertexData.h"

#include <glm/gtx/rotate_vector.hpp>

void VoxelRenderSystem::start(Entity* entity) {
    Transform* transform = getComponentFromEntity<Transform>(entity);
    VoxelRenderer* renderer = getComponentFromEntity<VoxelRenderer>(entity);
}

void VoxelRenderSystem::update(Entity* entity) {
    Transform* transform = getComponentFromEntity<Transform>(entity);
    VoxelRenderer* renderer = getComponentFromEntity<VoxelRenderer>(entity);

    renderer->shader->use();
    renderer->camera->setUniforms(renderer->shader);
    renderer->shader->setMat4("model", glm::translate(glm::mat4(1), transform->position));

    float mult = 0.1f;
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

void MainCameraSystem::start(Entity* entity) {
    Transform* transform = getComponentFromEntity<Transform>(entity);
    VoxelRenderer* renderer = getComponentFromEntity<VoxelRenderer>(entity);
}

void MainCameraSystem::update(Entity* entity) {
    Transform* transform = getComponentFromEntity<Transform>(entity);
    VoxelRenderer* renderer = getComponentFromEntity<VoxelRenderer>(entity);
}

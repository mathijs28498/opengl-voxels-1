#pragma once

#include "Shader.h"
#include "Mesh.h"

#include <iostream>

class Model {
public:
	Model(glm::vec3 scaling, uint32_t xAmount, uint32_t yAmount, uint32_t zAmount);
	void draw(Shader* shader);

private:
	glm::vec3 scale;
	uint32_t xAmount, yAmount, zAmount;
	uint32_t cubeVAO;

	void loadModel(std::string path);
};
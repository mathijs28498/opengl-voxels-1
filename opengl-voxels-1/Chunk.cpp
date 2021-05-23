#include "Chunk.h"
#include "GLHelperFunctions.h"

#define FNL_IMPL
#include "FastNoiseLite.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include <set>
#include <iostream>

Chunk::Chunk(uint32_t xAmount, uint32_t zAmount, glm::vec2 chunkCenter, const float voxSize) {
	this->chunkCenter = chunkCenter;
	this->voxSize = voxSize;

	fnl_state noise = fnlCreateState();
	noise.noise_type = FNL_NOISE_PERLIN;

	voxels.resize(xAmount * zAmount * 2);

	for (uint32_t zi = 0; zi < zAmount; zi++) {
		for (uint32_t xi = 0; xi < xAmount; xi++) {
			int32_t x = xi + chunkCenter.x -  xAmount / 2.0f;
			int32_t z = zi + chunkCenter.y - zAmount / 2.0f;
			float data = fnlGetNoise2D(&noise, x, z) * 50;
			data += fnlGetNoise2D(&noise, x * 10, z * 10) * 3;
			data += fnlGetNoise2D(&noise, x * 100, z * 100) * 0.1f;

			float y = std::round(data);

			VoxelData topLayer = {
				glm::vec3(x, y, z),
				glm::vec3((y + 30) / 30, 1 - (y + 20) / 30, (y) / 30),
			};
			VoxelData bottomLayer = {
				topLayer.location - glm::vec3(0, 1, 0),
				glm::vec3((y - 1 + 30) / 30, 1 - (y - 1 + 20) / 30, (y - 1) / 30),
			};

			uint32_t pos = (zi * xAmount + xi) * 2;
			voxels[pos] = topLayer;
			voxels[pos + 1] = bottomLayer;
		}
	}

	VAO;
	glGenVertexArrays(1, &VAO);

	fillVAO();
}

void Chunk::fillVAO() {
	uint32_t VBO;
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, voxels.size() * sizeof(VoxelData), voxels.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VoxelData), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VoxelData), (void*) offsetof(VoxelData, color));

	glBindVertexArray(0);
}

void Chunk::draw(Shader* shader) {

	shader->setMat4("model", glm::mat4(1.0f));
	shader->setVec3("gColor", glm::vec3(0, 1, 0));
	shader->setFloat("voxSize", voxSize);

	size_t temp = voxels.size();
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, voxels.size());
	glBindVertexArray(0);
}


void Chunk::loadModel(std::string path) {}
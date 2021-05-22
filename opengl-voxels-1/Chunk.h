#pragma once

#include "Shader.h"
#include "Mesh.h"

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

struct VoxelData {
	glm::vec3 location;
	glm::vec3 color;
	uint8_t enabledFaces;
};

class Chunk {
public:
	Chunk(std::string path, uint32_t xAmount, uint32_t zAmount, glm::vec2 chunkCenter, const float voxSize);
	void draw(Shader* shader);

private:
	glm::vec2 chunkCenter;
	float voxSize;
	uint32_t VAO;
	glm::vec3 scale;
	uint32_t xAmount, yAmount, zAmount;
	uint32_t cubeVAO;
	std::vector<VoxelData> voxels;

	void loadModel(std::string path);
	void fillVAO();
};
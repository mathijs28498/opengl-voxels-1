#pragma once

#include "Shader.h"
#include "Camera.h"
#include "VertexData.h"
#include "Component.h"

#include <vector>

struct QuadUint8Struct {
	uint8_t a, b, c, d;

	static QuadUint8Struct getQuadUint8Struct(std::vector<char>* bytes, size_t* bytePointer) {
		return {
			(uint8_t)(*bytes)[(*bytePointer)++],
			(uint8_t)(*bytes)[(*bytePointer)++],
			(uint8_t)(*bytes)[(*bytePointer)++],
			(uint8_t)(*bytes)[(*bytePointer)++]
		};
	}
};

class Model {
public:
	Model(const char* filePath);

	void draw(Shader* shader, Camera* cam);
	VoxelRenderer getVoxelRenderer(Shader* shader, Camera* camera);
private:
	uint32_t VAO;
	uint32_t amountOfVoxels;
	glm::vec3 position;

	void loadModel(const char* filePath);
	void calculateVAO(std::vector<Voxel>* voxels);
};
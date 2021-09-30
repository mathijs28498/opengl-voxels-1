#pragma once
#include <glm/glm.hpp>

// TODO: Decrease this to 0.125f
const float VOX_SIZE = 0.25f;

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

struct Light {
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct Voxel {
	float position[4];
	float color[3];
	unsigned int sideBitMask;

	static Voxel getVoxelCopy(Voxel originalVoxel) {
		Voxel voxel;
		for (size_t i = 0; i < 4; i++) 
			voxel.position[i] = originalVoxel.position[i];
		for (size_t i = 0; i < 3; i++) 
			voxel.color[i] = originalVoxel.color[i];
		voxel.sideBitMask = originalVoxel.sideBitMask;

		return voxel;
	}
};

extern float cubeVertices[];

extern float vertices[];

extern glm::vec3 cubePositions[];

extern Material materials[];

extern glm::vec3 pointLightPositions[];

extern glm::vec3 pointLightColors[];
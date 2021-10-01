#pragma once
#include <glm/glm.hpp>
#include <iostream>

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
	uint32_t positionInt;
	// TODO: Put enabled faces in colorInt extra byte (or perhaps make 2 uint16_t)
	uint32_t colorAndEnabledInt;

	static Voxel getVoxelCopy(Voxel originalVoxel) {
		Voxel voxel{ originalVoxel.positionInt, originalVoxel.colorAndEnabledInt };

		return voxel;
	}
};

extern float cubeVertices[];

extern float vertices[];

extern glm::vec3 cubePositions[];

extern Material materials[];

extern glm::vec3 pointLightPositions[];

extern glm::vec3 pointLightColors[];
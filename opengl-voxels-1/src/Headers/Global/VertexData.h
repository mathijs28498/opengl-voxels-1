#pragma once
#include "../Observer pattern/SceneObserver.h"
#include "../Global/GeneralHelperFunctions.h"

#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>


class Entity;

extern Event<Entity*> addEntityEvent;
extern Event2<std::vector<Entity*>*, std::string> getEntitiesWithComponentEvent;


// TODO: Decrease this to 0.125f
#define VOX_SIZE 0.25f
#define HALF_VOX_SIZE (VOX_SIZE / 2)

#define OCTREE_SIZE 256
#define REAL_OCTREE_SIZE (VOX_SIZE * OCTREE_SIZE)
#define REAL_HALF_OCTREE_SIZE (REAL_OCTREE_SIZE / 2)

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

struct Ray {
	glm::vec3 origin;
	glm::vec3 direction;
	float length;
};

struct RayCollision {
	glm::vec3 point;
	float distance = -10;
}; 

struct Voxel {
	uint32_t positionInt;
	uint32_t colorAndEnabledInt;

	glm::vec3 getPosition(glm::vec3& posOctree);
	glm::vec3 getModelPosition(const glm::vec3& modelPosition) const;
	bool rayCastCollision(Ray& ray, glm::vec3& posOctree, RayCollision* rayCollision);
	static Voxel getVoxelCopy(Voxel originalVoxel);
};

struct VoxelCollision {
	Voxel* voxel;
	glm::vec3 intersecPoint;
	float dist;
};

struct VoxelAABB {
	glm::vec3 min, max;

	VoxelAABB(const Voxel& voxel, const glm::vec3& modelPosition);
	VoxelAABB(const glm::vec3& pos, float size);
	float rayCollision(const Ray& r, float& tmin);
	float rayCollision(const Ray& r);
};


extern float cubeVertices[];

extern float vertices[];

extern glm::vec3 cubePositions[];

extern Material materials[];

extern glm::vec3 pointLightPositions[];

extern glm::vec3 pointLightColors[];
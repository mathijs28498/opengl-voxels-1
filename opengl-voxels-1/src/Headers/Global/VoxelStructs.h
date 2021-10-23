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

//#define OCTREE_SIZE 256
#define OCTREE_SIZE 256
#define REAL_OCTREE_SIZE (VOX_SIZE * OCTREE_SIZE)
#define REAL_HALF_OCTREE_SIZE (REAL_OCTREE_SIZE / 2)

//#define LEAF_SIZE 16
#define LEAF_SIZE 16
#define LEAF_MAX_INDEX (LEAF_SIZE - 1)
#define Q_LEAF_SIZE (LEAF_SIZE * LEAF_SIZE)
#define Q3_LEAF_SIZE (LEAF_SIZE * LEAF_SIZE * LEAF_SIZE)
#define Q3_LEAF_SIZE_OVERFLOW (Q_LEAF_SIZE + 2)

struct Ray {
	glm::vec3 origin;
	glm::vec3 direction;
	float length;
};



//struct Voxel {
//	union {
//		uint8_t positionArray[4];
//		uint32_t positionInt;
//	};
//	//uint32_t positionInt;
//	union {
//		uint8_t colorAndEnabledArray[4];
//		uint32_t colorAndEnabledInt;
//	};
//
//	glm::vec3 getModelPosition(const glm::vec3& modelPosition) const;
//	static Voxel getVoxelCopy(Voxel originalVoxel);
//};
struct Voxel {
	uint32_t positionInt;
	uint32_t colorAndEnabledInt;

	glm::vec3 getModelPosition(const glm::vec3& modelPosition) const;
	bool hasSamePosition(const std::vector<uint8_t>& otherPos) const;
	static Voxel getVoxelCopy(Voxel originalVoxel);
};

struct VoxelCollision {
	//uint32_t voxelIndex;
	Voxel* voxel;
	glm::vec3 intersecPoint;
	float dist;
	//uint32_t voxelIndex;
};

struct VoxelAABB {
	glm::vec3 min, max;

	VoxelAABB(const Voxel& voxel, const glm::vec3& modelPosition);
	VoxelAABB(const glm::vec3& pos, float size);
	float rayCollision(const Ray& r, float& tmin);
	float rayCollision(const Ray& r);
};
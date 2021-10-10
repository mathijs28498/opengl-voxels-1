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

	glm::vec3 getPosition(glm::vec3& posOctree) {
		uint8_t* posBytes = positionFromInt(positionInt);
		glm::vec3 pos = (glm::vec3(posBytes[0], posBytes[1], posBytes[2]) + posOctree) * VOX_SIZE;

		pos += posOctree;
		pos *= VOX_SIZE;
		return pos;
	}

	bool rayCastCollision(Ray& ray, glm::vec3& posOctree, RayCollision* rayCollision) {
		glm::vec3 pos = getPosition(posOctree);
		glm::vec3 min = pos - VOX_SIZE / 2;
		glm::vec3 max = pos + VOX_SIZE / 2;

		float tMinX = (min.x - ray.origin.x) / ray.direction.x;
		float tMinY = (min.y - ray.origin.y) / ray.direction.y;
		float tMinZ = (min.z - ray.origin.z) / ray.direction.z;

		float tMaxX = (max.x - ray.origin.x) / ray.direction.x;
		float tMaxY = (max.y - ray.origin.y) / ray.direction.y;
		float tMaxZ = (max.z - ray.origin.z) / ray.direction.z;

		float tMin = std::max(std::max(std::min(tMinX, tMaxX), std::min(tMinY, tMaxY)), std::min(tMinZ, tMaxZ));
		float tMax = std::min(std::min(std::max(tMinX, tMaxX), std::max(tMinY, tMaxY)), std::max(tMinZ, tMaxZ));

		if (tMax < 0 || tMin > tMax) {
			return false;
		} 
		
		rayCollision->distance = tMin > 0 ? tMin : tMax;
		rayCollision->point = ray.origin + ray.direction * rayCollision->distance;

		return true;
	}

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
#pragma once
#pragma once

#include "../Shader.h"
#include "../Camera.h"
#include "../Global/VoxelStructs.h"
#include "../ECS/Component.h"

#include <glm/glm.hpp>

#include <vector>
#include <thread>

int* getColorFromInt(int colorInt);
int getIntFromColor(const int* colorArray); 
glm::vec3 getRealOctreePos(glm::vec3& octreePos);

struct BoundingBoxPoint {
	float position[3];
	float color[3];
	float size;
};

class OctreeNode {
public:
	std::vector<int> pos;

	OctreeNode(std::vector<int> pos, int size, OctreeNode* parent = nullptr);
	OctreeNode();
	~OctreeNode();

	void insert(Voxel* voxel);
	void subdivide();
	void insertIntoChildren(Voxel* voxel);
	bool containsPoint(Voxel* voxel) const;
	bool containsPoint(const std::vector<uint8_t>& position) const;
	bool containsPoint(int16_t x, int16_t y, int16_t z) const;

	void calculateVAO(std::vector<Voxel>* voxelCloud, uint32_t lod);
	void calculateBoundingBoxVAO(std::vector<BoundingBoxPoint>* pointCloud, uint8_t depth);
	void calculateEnabledFaces();

	bool rayCastCollision(Ray& ray, glm::vec3& octreePos, VoxelCollision& collisionOut);
	void removeVoxel(const std::vector<uint8_t>& position);
	void calculateVoxelsToRemove(const std::vector<uint8_t>& position, uint16_t power, std::vector<std::vector<uint8_t>>& toRemoveOut);
	void calculateSurroundedFaces(const std::vector<uint8_t>& voxelPos);

private:
	Voxel* findSiblingVoxel(int16_t x, int16_t y, int16_t z) const;
	VoxelAABB getVoxelAABB(glm::vec3& octreePos);
	int getVoxelIndex(uint8_t x, uint8_t y, uint8_t z) const;

	void addSurroundingVoxelsToRemove(const std::vector<uint8_t>& voxelPos, uint8_t power, std::vector<std::vector<uint8_t>>& toRemoveOut);
	void addSurroundingVoxelToRemove(const std::vector<int16_t>& voxelPos, uint8_t power, std::vector<std::vector<uint8_t>>& toRemoveOut);
	uint8_t calculateEnabledFace(const std::vector<uint8_t>& voxelPos);

	std::vector<Voxel*> voxels;
	uint16_t size;
	bool hasChildren = false;
	OctreeNode* parent;
	OctreeNode* children[8];
};

class Octree {
public:
	uint16_t size;

	Octree() {};
	Octree(const std::vector<int> pos, uint32_t size);

	void insert(Voxel* voxel);
	void calculateBoundingBoxVAO();
	void calculateVoxelVAO(uint32_t lod);
	void makeNoiseTerrain(std::vector<int32_t> pos); 

	static Voxel* createVoxel(uint8_t x, uint32_t i, uint8_t z, std::array<uint8_t, 3> color);
	static uint8_t createEnabledFacesBitMask(uint32_t i, float y, float yf, float yb, float yl, float yr);
	void calculateEnabledFaces();

	void fillVoxelRenderer(VoxelRendererComp* renderer, uint32_t lod, bool reFill = false);
	void fillBoundingBoxRenderer(BoundingBoxRendererComp* renderer);

	bool rayCastCollision(Ray& ray, glm::vec3& pos, VoxelCollision& collisionOut);
	void removeVoxels(const std::vector<uint8_t>& position, uint16_t power);

private:
	OctreeNode root;
	std::map<uint32_t, uint32_t> voxelVAOs;
	std::map<uint32_t, uint32_t> voxelVBOs;
	std::map<uint32_t, uint32_t> voxelAmounts;
	uint32_t boundingBoxVAO;
	size_t amountOfBoundingboxes;
};
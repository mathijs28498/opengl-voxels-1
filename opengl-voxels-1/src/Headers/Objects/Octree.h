#pragma once
#pragma once

#include "../Shader.h"
#include "../Camera.h"
#include "../Global/VoxelStructs.h"
#include "../ECS/Component.h"

#include <glm/glm.hpp>

#include <vector>
#include <thread>
#include <array>

class Octree;
class OctreeNode;

int* getColorFromInt(int colorInt);
int getIntFromColor(const int* colorArray); 
glm::vec3 getRealOctreePos(glm::vec3& octreePos);


namespace DIR {
	enum Side {
		LEFT,
		RIGHT,
		DOWN,
		UP,
		BACK,
		FRONT,
	};
	
	Side getOpposite(int otherSide);
}

struct BoundingBoxPoint {
	float position[3];
	float color[3];
	float size;
};



struct FoundVoxel {
	OctreeNode* node;
	Voxel* voxel;
};

class OctreeNode {
public:
	std::array<int, 3> pos;

	OctreeNode(std::array<int, 3> pos, int size, OctreeNode* parent);
	OctreeNode(std::array<int, 3> pos, int size, Octree* parent);
	OctreeNode();
	~OctreeNode();

	void insert(Voxel* voxel);
	void subdivide();
	void insertIntoChildren(Voxel* voxel);
	bool containsPoint(Voxel* voxel) const;
	bool containsPoint(const std::array<uint8_t, 3>& position) const;
	bool containsPoint(int16_t x, int16_t y, int16_t z) const;

	void calculateVAO(std::vector<Voxel>* voxelCloud, uint32_t lod);
	void calculateBoundingBoxVAO(std::vector<BoundingBoxPoint>* pointCloud, uint8_t depth);
	void calculateEnabledFaces();

	bool rayCastCollision(Ray& ray, glm::vec3& octreePos, VoxelCollision& collisionOut);
	void removeVoxel(Voxel* voxel);
	void removeVoxel(const std::array<uint8_t, 3>& position);
	void calculateVoxelsToRemove(const std::array<uint8_t, 3>& position, float power, std::vector<FoundVoxel>& toRemoveOut);
	void calculateVoxelsToRemove(const FoundVoxel& fv, float power, std::vector<FoundVoxel>& toRemoveOut);
	void calculateSurroundedFaces(const std::array<uint8_t, 3>& voxelPos);
	void calculateEnabledFace(const FoundVoxel& curFv, DIR::Side side);
	FoundVoxel findSiblingVoxel(int16_t x, int16_t y, int16_t z);
	Octree* getParentOctree();

private:
	std::vector<Voxel> voxels;
	uint16_t size;
	bool hasChildren = false;

	enum { NODE, OCTREE } parentType;
	union {
		OctreeNode* parent;
		Octree* parentOctree;
	};
	OctreeNode* children[8];

	FoundVoxel findSiblingVoxel(const std::array<int16_t, 3>& voxPos);
	FoundVoxel findSiblingVoxel(const std::array<uint8_t, 3>& voxPos);
	uint8_t calculateEnabledFace(const std::array<uint8_t, 3>& voxelPos);

	VoxelAABB getVoxelAABB(glm::vec3& octreePos);
	int getVoxelIndex(uint8_t x, uint8_t y, uint8_t z) const;

	void addSurroundingVoxelsToRemove(const std::array<uint8_t, 3>& voxelPos, float power, std::vector<FoundVoxel>& toRemoveOut);
	void addSurroundingVoxelToRemove(const std::array<int16_t, 3>& voxelPos, float power, std::vector<FoundVoxel>& toRemoveOut);

	

};

class Octree {
public:
	uint16_t size;
	bool reFillRenderer = false;

	Octree() {};
	Octree(const std::array<int, 3> pos, uint32_t size);

	void insert(Voxel* voxel);
	void calculateBoundingBoxVAO();
	void calculateVoxelVAO(uint32_t lod);
	void makeNoiseTerrain(std::array<int32_t, 3> pos); 

	static Voxel* createVoxel(uint8_t x, uint32_t i, uint8_t z, uint8_t matID);
	static uint8_t createEnabledFacesBitMask(uint32_t i, float y, float yf, float yb, float yl, float yr);
	void calculateEnabledFaces();

	void fillVoxelRenderer(VoxelRendererComp* renderer, uint32_t lod, bool reFill = false);
	void fillBoundingBoxRenderer(BoundingBoxRendererComp* renderer);

	bool rayCastCollision(Ray& ray, glm::vec3& pos, VoxelCollision& collisionOut);
	std::vector<FoundVoxel> removeVoxels(const std::array<uint8_t, 3>& position, uint16_t power);

	void setSibling(Octree* octree, int side);
	void setSiblings(const std::vector<Octree*>& octrees);
	FoundVoxel findSiblingVoxel(int16_t x, int16_t y, int16_t z);

private:
	FoundVoxel getSideFoundVoxel(int16_t i, int16_t j, int side);

	std::array<int, 3> pos;
	OctreeNode root;
	Octree* siblings[6];
	std::map<uint32_t, uint32_t> voxelVAOs;
	std::map<uint32_t, uint32_t> voxelVBOs;
	std::map<uint32_t, uint32_t> voxelAmounts;
	uint32_t boundingBoxVAO;
	size_t amountOfBoundingboxes;
};
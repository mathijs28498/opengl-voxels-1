#pragma once

#include "Shader.h"

#include <glm/glm.hpp>

#include <vector>

int* getColorFromInt(int colorInt);
int getIntFromColor(const int* colorArray);

struct BoundingBoxPoint {
	float position[3];
	float size;
};

struct Voxel {
	float position[3];
	float color[3];
};

class OctreeNode {
public:
	OctreeNode(std::vector<int> pos, int size, OctreeNode* parent = nullptr);
	OctreeNode();
	~OctreeNode();

	void insert(Voxel voxel);
	void subdivide();
	void insertIntoChildren(Voxel voxel);
	bool containsPoint(Voxel voxel) const;

	void calculateVAO(std::vector<Voxel>* voxelCloud);
	void calculateBoundingBoxVAO(std::vector<BoundingBoxPoint>* pointCloud);

	void drawVoxels(Shader* shader) const;
private:
	std::vector<int> pos;
	std::vector<Voxel> voxels;
	int size;
	OctreeNode* parent;
	bool hasChildren;
	OctreeNode* children[8];
};

class Octree {
public:
	Octree(const std::vector<int> pos, int size);

	void insert(Voxel voxel);
	void calculateBoundingBoxVAO();
	void calculateVoxelVAO();

	void drawVoxels(Shader* shader);
	void drawBoundingBoxes(Shader* shader) const;
private:
	OctreeNode root;
	// TODO: Make this a vector/hashmap with multiple LOD
	uint32_t voxelVAO;
	size_t amountOfVoxels;
	uint32_t boundingBoxVAO;
	size_t amountOfBoundingboxes;
};
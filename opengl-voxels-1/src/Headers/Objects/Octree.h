#pragma once
#pragma once

#include "../Shader.h"
#include "../Camera.h"
#include "../Global/VertexData.h"
#include "../ECS/Component.h"

#include <glm/glm.hpp>

#include <vector>
#include <thread>

int* getColorFromInt(int colorInt);
int getIntFromColor(const int* colorArray);

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

	void insert(Voxel* voxel, int32_t voxelPosInt[3]);
	void subdivide();
	void insertIntoChildren(Voxel* voxel, int32_t voxelPosInt[3]);
	bool containsPoint(Voxel* voxel) const;

	void calculateVAO(std::vector<Voxel>* voxelCloud, uint32_t lod);
	void calculateBoundingBoxVAO(std::vector<BoundingBoxPoint>* pointCloud, uint8_t depth);

	void drawVoxels(Shader* shader) const;
private:
	Voxel getAverageVoxelChildren();

	std::vector<Voxel*> voxels;
	int size;
	OctreeNode* parent;
	bool hasChildren = false;
	OctreeNode* children[8];
};

class Octree {
public:
	int32_t size;

	Octree() {};
	Octree(const std::vector<int> pos, uint32_t size);

	void insert(Voxel* voxel, int32_t voxelPosInt[3]);
	void calculateBoundingBoxVAO();
	void calculateVoxelVAO(uint32_t lod);
	void makeNoiseTerrain(std::vector<int32_t> pos);

	void fillVoxelRenderer(VoxelRendererComp* renderer, uint32_t lod);
	void fillBoundingBoxRenderer(BoundingBoxRendererComp* renderer);

private:
	OctreeNode root;
	std::map<uint32_t, uint32_t> voxelVAOs;
	std::map<uint32_t, uint32_t> voxelAmounts;
	uint32_t boundingBoxVAO;
	size_t amountOfBoundingboxes;
};
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

	void insert(Voxel* voxel);
	void subdivide();
	void insertIntoChildren(Voxel* voxel);
	bool containsPoint(Voxel* voxel) const;

	void calculateVAO(std::vector<Voxel>* voxelCloud, uint32_t lod);
	void calculateBoundingBoxVAO(std::vector<BoundingBoxPoint>* pointCloud, uint8_t depth);

	bool rayCastCollision(Ray& ray, glm::vec3& octreePos, RayCollision* collision);

private:
	Voxel getAverageVoxelChildren();

	std::vector<Voxel*> voxels;
	uint16_t size;
	OctreeNode* parent;
	bool hasChildren = false;
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

	static Voxel* createVoxel(uint8_t x, uint32_t i, uint8_t z, std::array<uint8_t, 3> color, float y, float yf, float yb, float yl, float yr);
	static uint8_t createEnabledFacesBitMask(uint32_t i, float y, float yf, float yb, float yl, float yr);

	void fillVoxelRenderer(VoxelRendererComp* renderer, uint32_t lod);
	void fillBoundingBoxRenderer(BoundingBoxRendererComp* renderer);

	bool rayCastCollision(Ray& ray, glm::vec3& pos, RayCollision* collision);

private:
	OctreeNode root;
	std::map<uint32_t, uint32_t> voxelVAOs;
	std::map<uint32_t, uint32_t> voxelAmounts;
	uint32_t boundingBoxVAO;
	size_t amountOfBoundingboxes;
};
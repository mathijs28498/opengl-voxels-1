#pragma once

#include "glm/glm.hpp"

#include <vector>

namespace oct {
	int* getColorFromInt(int colorInt);
	int getIntFromColor(const int* colorArray);

	struct BoundingBox {
		glm::vec3 start;
		float size;

		bool contains(glm::vec3 pos);
	};

	class OctreeNode {
	public:
		OctreeNode(BoundingBox boundary, OctreeNode* parent);
		void insert(glm::vec3 pos, int color);

	private:
		BoundingBox boundary;
		OctreeNode* parent;
		bool hasChildren;
		OctreeNode* children[8];
		int32_t color;
		uint32_t lod;

		void subdivide();
		void insertIntoChildren(glm::vec3 pos, int color);
	};

	class Octree {
	public:
		Octree(BoundingBox boundary);
		~Octree();

		void insert(glm::vec3 pos, int color);
		void fillVAO(uint32_t lod);
	private:
		OctreeNode* rootNode;
		std::vector<uint32_t> VAOs;
	};
}
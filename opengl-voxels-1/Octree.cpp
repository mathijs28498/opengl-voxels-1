#include "Octree.h"
#include "VertexData.h"

#include <glm/gtx/norm.hpp>
#include <stdexcept>

using namespace oct;

/// Begin OctreeNode ///

OctreeNode::OctreeNode(BoundingBox boundary, OctreeNode* parent = nullptr) {
	this->boundary = boundary;
	this->parent = parent;
}

void OctreeNode::insert(glm::vec3 pos, int color) {
	if (boundary.size - VOX_SIZE < 0.001f) {
		this->color = color;
		return;
	}
	insertIntoChildren(pos, color);
}

void OctreeNode::insertIntoChildren(glm::vec3 pos, int color) {
	subdivide();
	for (OctreeNode* node : children) 	{
		if (node->boundary.contains(pos)) {
			node->insert(pos, color);
			return;
		}
	}

	throw std::runtime_error("Failed to insert OctreeNode child, given voxels was not within boundary of children!");
}

void OctreeNode::subdivide() {
	float newSize = boundary.size / 2;
	OctreeNode child = OctreeNode({ boundary.start, newSize }, this);
	children[0] = &child;
	child = OctreeNode({ boundary.start + glm::vec3(0, 0, 1) * newSize, newSize }, this);
	children[1] = &child;
	child = OctreeNode({ boundary.start + glm::vec3(0, 1, 0) * newSize, newSize }, this);
	children[2] = &child;
	child = OctreeNode({ boundary.start + glm::vec3(0, 1, 1) * newSize, newSize }, this);
	children[3] = &child;
	child = OctreeNode({ boundary.start + glm::vec3(1, 0, 0) * newSize, newSize }, this);
	children[4] = &child;
	child = OctreeNode({ boundary.start + glm::vec3(1, 0, 1) * newSize, newSize }, this);
	children[5] = &child;
	child = OctreeNode({ boundary.start + glm::vec3(1, 1, 0) * newSize, newSize }, this);
	children[6] = &child;
	child = OctreeNode({ boundary.start + glm::vec3(1, 1, 1) * newSize, newSize }, this);
	children[7] = &child;
}

/// End OctreeNode ///

/// Begin Octree //

Octree::Octree(BoundingBox boundary) {
	OctreeNode node = OctreeNode(boundary);
	rootNode = &node;
}

void Octree::insert(glm::vec3 pos, int color) {
	rootNode->insert(pos, color);
}

/// End Octree //

/// Begin BoundingBox ///

bool BoundingBox::contains(glm::vec3 pos) {
	glm::vec3 diff = (pos - start);
	float diffLengthSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
	if (diffLengthSq < 0.001f * 0.001f) return true;

	return pos.x >= start.x && pos.x <= start.x + size 
		&& pos.y >= start.y && pos.y <= start.y + size 
		&& pos.z >= start.z && pos.z <= start.z + size;
}

/// End BoundingBox ///

int oct::getIntFromColor(const int* colorArray) {
	int red = (colorArray[0] << 16) & 0x00FF0000;
	int green = (colorArray[1] << 8) & 0x0000FF00;
	int blue = colorArray[2] & 0x000000FF;

	return 0xFF000000 | red | green | blue;
}

int* oct::getColorFromInt(int colorInt) {
	int colorArray[3];
	colorArray[0] = (colorInt & 0x00FF0000) >> 16;
	colorArray[1] = (colorInt & 0x0000FF00) >> 8;
	colorArray[2] = colorInt & 0x000000FF;
	return colorArray;
}

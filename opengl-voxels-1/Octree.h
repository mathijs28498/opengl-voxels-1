#pragma once

#include "Shader.h"

#include <glm/glm.hpp>

#include <vector>

int* getColorFromInt(int colorInt);
int getIntFromColor(const int* colorArray);


class OctreeNode {
public:
	OctreeNode(const int* pos, int size, OctreeNode* parent = nullptr);

	void draw(Shader* shader) const;
	void drawBoundingBox(Shader* shader) const;
private:
	const int* pos;
	int size;
	OctreeNode* parent;
	OctreeNode* children[8];
	bool hasChildren;

};

class Octree {
public:
	Octree(const int* pos, int size);

	void draw(Shader* shader) const;
	void drawBoundingBoxes(Shader* shader) const;
private:
	OctreeNode* root;
};
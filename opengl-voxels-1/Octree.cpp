#include "Octree.h"
#include "VertexData.h"

#include <stdexcept>

OctreeNode::OctreeNode(const int* pos, int size, OctreeNode* parent) {
	this->pos = pos;
	this->size = size;
	this->parent = parent;
}

Octree::Octree(const int* pos, int size) {
	OctreeNode node = OctreeNode(pos, size);
	root = &node;
}

void Octree::drawBoundingBoxes(Shader* shader) const {
	shader->setMat4("Model", glm::mat4(1.0f));

	uint32_t VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	float data[] = { 0.0f, 0.0f, 0.0f };

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float), data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(data), (void*)0);

	glBindVertexArray(0);

	shader->setMat4("model", glm::mat4(1.0f));
	shader->setFloat("voxSize", VOX_SIZE);
	shader->setFloat("size", 100);
	shader->setVec3("color", 1, 1, 0);

	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, 3);
	glBindVertexArray(0);
}

int getIntFromColor(const int* colorArray) {
	int red = (colorArray[0] << 16) & 0x00FF0000;
	int green = (colorArray[1] << 8) & 0x0000FF00;
	int blue = colorArray[2] & 0x000000FF;

	return 0xFF000000 | red | green | blue;
}

int* getColorFromInt(int colorInt) {
	int colorArray[3];
	colorArray[0] = (colorInt & 0x00FF0000) >> 16;
	colorArray[1] = (colorInt & 0x0000FF00) >> 8;
	colorArray[2] = colorInt & 0x000000FF;
	return colorArray;
}

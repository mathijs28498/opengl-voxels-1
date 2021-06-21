#include "../Headers/Objects/Octree.h"

#define FNL_IMPL
#include "../Vendors/Noise/FastNoiseLite.h"

#include <stdexcept>
#include <chrono>
#include <glm/gtx/rotate_vector.hpp>

/// BEGIN OCTREE_NODE ///

OctreeNode::OctreeNode(std::vector<int> pos, int size, OctreeNode* parent) {
	this->pos.resize(3);
	this->pos[0] = pos[0];
	this->pos[1] = pos[1];
	this->pos[2] = pos[2];

	this->size = size;
	this->parent = parent;
}

OctreeNode::OctreeNode() {}

OctreeNode::~OctreeNode() {
	for (OctreeNode* child : children) {
		delete child;
	}
}

void OctreeNode::insert(Voxel voxel) {
	if (size <= 16) {
		voxels.push_back(voxel);
		return;
	}
	insertIntoChildren(voxel);

}

void OctreeNode::insertIntoChildren(Voxel voxel) {
	if (!hasChildren) {
		subdivide();
	}

	for (OctreeNode* node : children) {
		if (node->containsPoint(voxel)) {
			node->insert(voxel);
			return;
		}
	}
}

bool OctreeNode::containsPoint(Voxel voxel) const {
	return voxel.position[0] >= pos[0] && voxel.position[0] <= pos[0] + size &&
		voxel.position[1] >= pos[1] && voxel.position[1] <= pos[1] + size &&
		voxel.position[2] >= pos[2] && voxel.position[2] <= pos[2] + size;
}

void OctreeNode::subdivide() {
	int newSize = size / 2;

	children[0] = new OctreeNode(pos, newSize, this);
	children[1] = new OctreeNode({ pos[0], pos[1], pos[2] + newSize }, newSize, this);
	children[2] = new OctreeNode({ pos[0], pos[1] + newSize, pos[2] }, newSize, this);
	children[3] = new OctreeNode({ pos[0], pos[1] + newSize, pos[2] + newSize }, newSize, this);
	children[4] = new OctreeNode({ pos[0] + newSize, pos[1], pos[2] }, newSize, this);
	children[5] = new OctreeNode({ pos[0] + newSize, pos[1], pos[2] + newSize }, newSize, this);
	children[6] = new OctreeNode({ pos[0] + newSize, pos[1] + newSize, pos[2] }, newSize, this);
	children[7] = new OctreeNode({ pos[0] + newSize, pos[1] + newSize, pos[2] + newSize }, newSize, this);

	hasChildren = true;
}

void OctreeNode::drawVoxels(Shader* shader) const {

}

// TODO: Make different lod
void OctreeNode::calculateVAO(std::vector<Voxel>* voxelCloud) {
	if (!hasChildren) {
		for (size_t i = 0; i < voxels.size(); i++) {
			voxelCloud->push_back(voxels[i]);
		}
	} else {
		for (OctreeNode* child : children) {
			child->calculateVAO(voxelCloud);
		}
	}
}
// TODO: Make different colour for each layer
void OctreeNode::calculateBoundingBoxVAO(std::vector<BoundingBoxPoint>* pointCloud, uint8_t depth) {
	if (!hasChildren) {
		pointCloud->push_back({
			{static_cast<float>(pos[0]), static_cast<float>(pos[1]), static_cast<float>(pos[2])},
			{(sin(static_cast<float>(depth)) + 1) / 2, (cos(static_cast<float>(depth)) + 1) / 2, 1 - (sin(static_cast<float>(depth)) + 1) / 2},
			static_cast<float>(size) });
	} else {
		for (OctreeNode* child : children) {
			child->calculateBoundingBoxVAO(pointCloud, depth + 1);
		}
	}
}

/// END OCTREE_NODE ///

/// BEGIN OCTREE ///

Octree::Octree(const std::vector<int> pos, int size) {
	OctreeNode node = OctreeNode(pos, size);
	root = node;
	this->pos = pos;
	this->size = size;
	glGenVertexArrays(1, &voxelVAO);
	glGenVertexArrays(1, &boundingBoxVAO);
}

void Octree::insert(Voxel voxel) {
	root.insert(voxel);
}

void Octree::calculateBoundingBoxVAO() {
	std::vector<BoundingBoxPoint> pointCloud;
	root.calculateBoundingBoxVAO(&pointCloud, 0);
	amountOfBoundingboxes = pointCloud.size();

	uint32_t VBO;
	glGenBuffers(1, &VBO);
	glBindVertexArray(boundingBoxVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, pointCloud.size() * sizeof(BoundingBoxPoint), pointCloud.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BoundingBoxPoint), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(BoundingBoxPoint), (void*)offsetof(BoundingBoxPoint, color));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(BoundingBoxPoint), (void*)offsetof(BoundingBoxPoint, size));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Octree::calculateVoxelVAO() {
	using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	using std::chrono::duration;
	using std::chrono::milliseconds;

	auto t1 = high_resolution_clock::now();

	std::vector<Voxel> voxelCloud;
	root.calculateVAO(&voxelCloud);
	amountOfVoxels = voxelCloud.size();


	uint32_t VBO;
	glGenBuffers(1, &VBO);
	glBindVertexArray(voxelVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, voxelCloud.size() * sizeof(Voxel), voxelCloud.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Voxel), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Voxel), (void*)offsetof(Voxel, color));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	duration<double, std::milli> ms = high_resolution_clock::now() - t1;
	std::cout << ms.count() << '\n';

}

void Octree::makeNoiseTerrain() {
	int32_t chunkPos[] = {pos[0], pos[2] };
	fnl_state noise = fnlCreateState();
	noise.noise_type = FNL_NOISE_PERLIN;

	for (int32_t zi = 0; zi < size; zi++) {
		for (int32_t xi = 0; xi < size; xi++) {
			float x = static_cast<float>(xi + chunkPos[0]);
			float z = static_cast<float>(zi + chunkPos[1]);
			float data = (fnlGetNoise2D(&noise, x, z) + 1) * 50;
			data += fnlGetNoise2D(&noise, x * 10, z * 10) * 3;
			data += fnlGetNoise2D(&noise, x * 100, z * 100) * 0.1f;

			float y = std::round(data);

			insert({ {x, y, z}, {0, 0, 0} });
			insert({ {x, y - 1, z}, {0, 0, 0} });
		}
	}

}

VoxelRenderer* Octree::getVoxelRenderer(Shader* shader, Camera* camera) {
	return new VoxelRenderer{ shader, camera, voxelVAO, static_cast<uint32_t>(amountOfVoxels) };
}

BoundingBoxRenderer* Octree::getBoundingBoxRenderer(Shader* shader, Camera* camera, bool show) {
	return new BoundingBoxRenderer{ shader, camera, boundingBoxVAO, static_cast<uint32_t>(amountOfVoxels), show };
}

/// END OCTREE ///

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

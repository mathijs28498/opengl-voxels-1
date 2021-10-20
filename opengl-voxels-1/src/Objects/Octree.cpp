#include "../Headers/Objects/Octree.h"
#include "../Headers/Global/GeneralHelperFunctions.h"

#define FNL_IMPL
#include "../Vendors/Noise/FastNoiseLite.h"

#include <stdexcept>
#include <chrono>
#include <glm/gtx/rotate_vector.hpp>
#include <array>

constexpr uint32_t LEAF_SIZE = 16;

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
	// TODO: Fix this
	/*for (OctreeNode* child : children) {
		delete child;
	}*/
}

void OctreeNode::insert(Voxel* voxel) {
	if (size <= LEAF_SIZE) {
		voxels.push_back(voxel);
		return;
	}
	insertIntoChildren(voxel);

}

void OctreeNode::insertIntoChildren(Voxel* voxel) {
	if (!hasChildren) {
		subdivide();
	}

	int32_t childSize = children[0]->size;
	uint8_t* voxelPos = positionFromInt(voxel->positionInt);

	int32_t childIndex =
		(voxelPos[0] - pos[0]) / childSize * 4 +
		(voxelPos[1] - pos[1]) / childSize * 2 +
		(voxelPos[2] - pos[2]) / childSize;

	children[childIndex]->insert(voxel);
}

bool OctreeNode::containsPoint(Voxel* voxel) const {
	std::vector<uint8_t> position = intToBytes(voxel->positionInt);
	return position[0] >= pos[0] && position[0] <= pos[0] + size &&
		position[1] >= pos[1] && position[1] <= pos[1] + size &&
		position[2] >= pos[2] && position[2] <= pos[2] + size;
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

// TODO: Make different lod
void OctreeNode::calculateVAO(std::vector<Voxel>* voxelCloud, uint32_t lod) {
	static uint32_t sum = 0;
	if (!hasChildren) {
		if (lod == 1) {
			for (size_t i = 0; i < voxels.size(); i++) {
				/*if (++sum % 1000 == 0)
					std::cout << sum << '\n';*/

				if ((voxels[i]->colorAndEnabledInt & 0xFF000000) == 0x00)
					continue;
				voxelCloud->push_back(*voxels[i]);
			}
		} else {

			// TODO: Change enabled faces for higher lod (or add flag for lod)
			// TODO: Fix newSize property
			uint32_t newSize = pow(2, lod - 1);
			for (size_t i = 0; i < voxels.size(); i++) {
				Voxel voxel = Voxel::getVoxelCopy(*voxels[i]);
				voxel.colorAndEnabledInt = voxel.colorAndEnabledInt & 0xFFFFFF + 0xFF000000;
				std::vector<uint8_t> position = intToBytes(voxel.positionInt);
				if ((static_cast<uint32_t>(position[0]) % newSize) == 0 &&
					(static_cast<uint32_t>(position[1]) % newSize) == 0 &&
					(static_cast<uint32_t>(position[2]) % newSize) == 0) {
					voxel.positionInt = voxel.positionInt & 0xFFFFFF + (lod - 1) << 24;
					voxelCloud->push_back(voxel);
				}
			}
		}
	} else {
	for (OctreeNode* child : children) {
		child->calculateVAO(voxelCloud, lod);
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

glm::vec3 getRealOctreePos(glm::vec3& octreePos) {
	return octreePos * REAL_OCTREE_SIZE - REAL_HALF_OCTREE_SIZE;
}

VoxelAABB OctreeNode::getVoxelAABB(glm::vec3& octreePos) {
	glm::vec3 realLocalPos{
		pos[0] * VOX_SIZE,
		pos[1] * VOX_SIZE,
		pos[2] * VOX_SIZE,
	};
	glm::vec3 realGlobalPos = realLocalPos + octreePos * REAL_OCTREE_SIZE - REAL_HALF_OCTREE_SIZE;
	return { realGlobalPos, size * VOX_SIZE };
}

bool OctreeNode::rayCastCollision(Ray& ray, glm::vec3& octreePos, VoxelCollision& collisionOut) {
	if (voxels.size() > 0) {
		float intersecDist = -1;
		Voxel* voxelIntersected = nullptr;
		glm::vec3 realOctreePos = getRealOctreePos(octreePos);
		for (size_t i = 0; i < voxels.size(); i++) {
			VoxelAABB aabb(*voxels[i], realOctreePos);
			float d = aabb.rayCollision(ray);
			if (d > 0 && (intersecDist < 0 || d < intersecDist)) {
				voxelIntersected = voxels[i];
				intersecDist = d;
			}
		}
		if (intersecDist < 0)
			return false;

		collisionOut.voxel = voxelIntersected;
		collisionOut.dist = intersecDist;
		return true;
	} else if (!hasChildren) {
		return false;
	} else {
		VoxelAABB aabb = getVoxelAABB(octreePos);
		float tmin;
		float collisionDist = aabb.rayCollision(ray, tmin);

		// No hit if:
		// - there is no collision
		// - the distance is further than the ray length
		// - there is a closer collision found
		if (collisionDist < 0 || tmin > ray.length || (collisionOut.dist > 0 && collisionOut.dist < collisionDist)) 
			return false;

		VoxelCollision collisionChild;
		float collisionChildDist = -1;
		for (OctreeNode* child : children) {
			if (child->rayCastCollision(ray, octreePos, collisionChild) && (collisionChildDist < 0 || collisionChild.dist < collisionChildDist)) {
				collisionChildDist = collisionChild.dist;
				collisionOut = collisionChild;
			}
		}
		
		return collisionChildDist > 0;
	}
}

/// END OCTREE_NODE ///

/// BEGIN OCTREE ///

Octree::Octree(const std::vector<int> pos, uint32_t size) {
	root = OctreeNode({ 0, 0, 0 }, size);

	this->size = size;
	glGenVertexArrays(1, &boundingBoxVAO);
}

void Octree::insert(Voxel* voxel) {
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

void Octree::calculateVoxelVAO(uint32_t lod) {
	using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	using std::chrono::duration;

	auto t1 = high_resolution_clock::now();

	std::vector<Voxel> voxelCloud;
	root.calculateVAO(&voxelCloud, lod);

	if (!voxelVAOs.count(lod)) {
		uint32_t VAO;
		glGenVertexArrays(1, &VAO);
		voxelVAOs.insert({ lod, VAO });
		voxelAmounts.insert({ lod, voxelCloud.size() });
	}

	std::cout << voxelAmounts[lod] << " voxels in drawn\n";

	uint32_t VBO;
	glGenBuffers(1, &VBO);
	glBindVertexArray(voxelVAOs[lod]);

	// TODO: Change to size of Voxel

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, voxelCloud.size() * sizeof(Voxel), voxelCloud.data(), GL_DYNAMIC_DRAW);

	glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(Voxel), (void*) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, sizeof(Voxel), (void*)offsetof(Voxel, colorAndEnabledInt));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	duration<double, std::milli> ms = high_resolution_clock::now() - t1;
	std::cout << ms.count() << "ms to build voxel chunk VAO\n";
}

float getNoiseHeight(fnl_state* noise, float xn, float zn) {
	float data = (fnlGetNoise2D(noise, xn, zn) + 1) * 50;
	data += fnlGetNoise2D(noise, xn * 10, zn * 10) * 3;
	data += fnlGetNoise2D(noise, xn * 100, zn * 100) * 0.1f;
	return std::round(data);
}

uint8_t Octree::createEnabledFacesBitMask(uint32_t i, float y, float yf, float yb, float yl, float yr) {
	uint8_t enabledFaces = 0x00;
	if (i == 0)
		enabledFaces += 0x20;
	if (i == y - 1)
		enabledFaces += 0x10;
	if (i >= yf)
		enabledFaces += 0x01;
	if (i >= yb)
		enabledFaces += 0x02;
	if (i >= yl)
		enabledFaces += 0x04;
	if (i >= yr)
		enabledFaces += 0x08;
	return enabledFaces;
}

Voxel* Octree::createVoxel(uint8_t x, uint32_t i, uint8_t z, std::array<uint8_t, 3> color, float y, float yf, float yb, float yl, float yr) {
	uint8_t enabledFaces = createEnabledFacesBitMask(i, y, yf, yb, yl, yr);
	uint32_t voxelPosInt = bytesToInt(x, i, z, 1);
	return new Voxel{ voxelPosInt, bytesToInt(color[0], color[1], color[2], enabledFaces) };
}


void Octree::makeNoiseTerrain(std::vector<int32_t> pos) {

	using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	using std::chrono::duration;
	using std::chrono::milliseconds;

	auto t1 = high_resolution_clock::now();

	int32_t chunkPos[] = { root.pos[0], root.pos[2] };
	fnl_state noise = fnlCreateState();
	noise.noise_type = FNL_NOISE_PERLIN;

	float xOffset = pos[0] * size;
	float zOffset = pos[2] * size;
	for (uint32_t z = 0; z < size; z++) {
		for (uint32_t x = 0; x < size; x++) {
			float xn = x + xOffset;
			float zn = z + zOffset;

			float y = getNoiseHeight(&noise, xn, zn);

			float yf = getNoiseHeight(&noise, xn, zn + 1);
			float yb = getNoiseHeight(&noise, xn, zn - 1);
			float yl = getNoiseHeight(&noise, xn - 1, zn);
			float yr = getNoiseHeight(&noise, xn + 1, zn);

			// TODO: REMOVE THIS!!!!

			int32_t curY = 0;
			uint8_t voxelPosBytes[] = { x, 0, z };
			for (int32_t i = curY; i < y && i < 25; i++, curY++) {
				insert(createVoxel(x, i, z, { 52, 58, 235 }, y, yf, yb, yl, yr));
			}
			for (size_t i = curY; i < y && i < 30; i++, curY++) {
				insert(createVoxel(x, i, z, { 52, 143, 235 }, y, yf, yb, yl, yr));
			}
			for (size_t i = curY; i < y && i < 35; i++, curY++) {
				insert(createVoxel(x, i, z, { 207, 191, 70 }, y, yf, yb, yl, yr));
			}
			for (size_t i = curY; i < y && i < 45; i++, curY++) {
				insert(createVoxel(x, i, z, { 75, 207, 70 }, y, yf, yb, yl, yr));
			}
			for (size_t i = curY; i < y && i < 55; i++, curY++) {
				insert(createVoxel(x, i, z, { 113, 122, 119 }, y, yf, yb, yl, yr));
			}
			for (size_t i = curY; i < y && i < 65; i++, curY++) {
				insert(createVoxel(x, i, z, { 59, 66, 64 }, y, yf, yb, yl, yr));
			}
			for (size_t i = curY; i < y; i++, curY++) {
				insert(createVoxel(x, i, z, { 230, 230, 230 }, y, yf, yb, yl, yr));
			}
		}
	}
	
	duration<double, std::milli> ms = high_resolution_clock::now() - t1;
	std::cout << ms.count() << "ms to create voxel terrain chunk\n";
}

void Octree::fillVoxelRenderer(VoxelRendererComp* renderer, uint32_t lod) {
	if (!voxelVAOs.count(lod))
		calculateVoxelVAO(lod);

	renderer->VAO = voxelVAOs[lod];
	renderer->voxelAmount = static_cast<uint32_t>(voxelAmounts[lod]);
}

void Octree::fillBoundingBoxRenderer(BoundingBoxRendererComp* renderer) {
	renderer->VAO = boundingBoxVAO;
	renderer->boundingBoxAmount = (uint32_t)amountOfBoundingboxes;
}

bool Octree::rayCastCollision(Ray& ray, glm::vec3& octreePos, VoxelCollision& collisionOut) {
	bool isCollided = root.rayCastCollision(ray, octreePos, collisionOut);
	if (!isCollided) return false;

	collisionOut.intersecPoint = ray.origin + ray.direction * collisionOut.dist;
	return true;
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

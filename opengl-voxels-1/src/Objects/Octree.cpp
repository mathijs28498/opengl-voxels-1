#include "../Headers/Objects/Octree.h"
#include "../Headers/Global/GeneralHelperFunctions.h"

#define FNL_IMPL
#include "../Vendors/Noise/FastNoiseLite.h"

#include <stdexcept>
#include <chrono>
#include <glm/gtx/rotate_vector.hpp>
#include <array>
#include <algorithm>

template<class T_I, class T_O>
std::array<T_O, 3>* getSurroundingPositions(const std::array<T_I, 3>& voxelPos) {
	std::array<T_O, 3> positions[6] = {
			{ (T_O)voxelPos[0] - 1, (T_O)voxelPos[1], (T_O)voxelPos[2] },
			{ (T_O)voxelPos[0] + 1, (T_O)voxelPos[1], (T_O)voxelPos[2] },
			{ (T_O)voxelPos[0], (T_O)voxelPos[1] - 1, (T_O)voxelPos[2] },
			{ (T_O)voxelPos[0], (T_O)voxelPos[1] + 1, (T_O)voxelPos[2] },
			{ (T_O)voxelPos[0], (T_O)voxelPos[1], (T_O)voxelPos[2] - 1 },
			{ (T_O)voxelPos[0], (T_O)voxelPos[1], (T_O)voxelPos[2] + 1 },
	};

	return positions;
}

template<class T>
bool isSamePosition(const std::array<T, 3>& pos0, const std::array<T, 3>& pos1) {
	return pos0[0] == pos1[0] && pos0[1] == pos1[1] && pos0[2] == pos1[2];
} 

uint32_t faceFlags[6] = {
	0x04,
	0x08,
	0x20,
	0x10,
	0x02,
	0x01,
};

uint32_t faceFlagsInv[6] = {
	0x08,
	0x04,
	0x10,
	0x20,
	0x01,
	0x02,
};

bool shouldInsertFace(FoundVoxel cur, FoundVoxel other, bool inv = false) {
	if (other.node == nullptr) {
		return false;
	}

	if (other.voxel == nullptr) 
		return inv ? false : true;

	if (!inv && cur.voxel->getMaterial()->isTransparent != other.voxel->getMaterial()->isTransparent)
		return true;

	return inv ? true : false;
}

/// BEGIN OCTREE_NODE ///

OctreeNode::OctreeNode(std::array<int, 3> pos, int size, OctreeNode* parent) {
	this->pos = pos;

	this->size = size;
	this->parentType = NODE;
	this->parent = parent;
}

OctreeNode::OctreeNode(std::array<int, 3> pos, int size, Octree* parent) {
	this->pos = pos;

	this->size = size;
	this->parentType = OCTREE;
	this->parentOctree = parent;
}

OctreeNode::OctreeNode() {}

OctreeNode::~OctreeNode() {
	// TODO: Fix this
	/*for (OctreeNode* child : children) {
		delete child;
	}*/
}

int OctreeNode::getVoxelIndex(uint8_t x, uint8_t y, uint8_t z) const {
	return (z - pos[2]) * Q_LEAF_SIZE + (y - pos[1]) * LEAF_SIZE + (x - pos[0]);
}

void OctreeNode::insert(Voxel* voxel) {
	if (size <= LEAF_SIZE) {
		std::array<uint8_t, 3> voxPos = intToBytes3(voxel->positionInt);
		int index = getVoxelIndex(voxPos[0], voxPos[1], voxPos[2]);
		if (voxels.size() < index + 1) {
			voxels.resize(std::min(Q3_LEAF_SIZE, index + 100));
			//PRINT(voxels.size());
		}
		voxels[index] = voxel;
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
	return containsPoint(intToBytes3(voxel->positionInt));
}

bool OctreeNode::containsPoint(const std::array<uint8_t, 3>& position) const {
	return position[0] >= pos[0] && position[0] < pos[0] + size &&
		position[1] >= pos[1] && position[1] < pos[1] + size &&
		position[2] >= pos[2] && position[2] < pos[2] + size;
}

bool OctreeNode::containsPoint(int16_t x, int16_t y, int16_t z) const {
	return x >= pos[0] && x < pos[0] + size &&
		y >= pos[1] && y < pos[1] + size &&
		z >= pos[2] && z < pos[2] + size;
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
				if (voxels[i] == nullptr)
					continue;
				if ((voxels[i]->materialAndEnabledInt & 0xFF000000) == 0x00)
					continue;
				voxelCloud->push_back(*voxels[i]);
			}
		} else {

			// TODO: Change enabled faces for higher lod (or add flag for lod)
			// TODO: Fix newSize property
			uint32_t newSize = pow(2, lod - 1);
			for (size_t i = 0; i < voxels.size(); i++) {
				Voxel voxel = Voxel::getVoxelCopy(*voxels[i]);
				voxel.materialAndEnabledInt = voxel.materialAndEnabledInt & 0xFFFFFF + 0xFF000000;
				std::array<uint8_t, 3> position = intToBytes3(voxel.positionInt);
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

FoundVoxel OctreeNode::findSiblingVoxel(const std::array<int16_t, 3>& voxPos) {
	return findSiblingVoxel(voxPos[0], voxPos[1], voxPos[2]);
}

FoundVoxel OctreeNode::findSiblingVoxel(const std::array<uint8_t, 3>& voxPos) {
	return findSiblingVoxel(voxPos[0], voxPos[1], voxPos[2]);
}

FoundVoxel OctreeNode::findSiblingVoxel(int16_t x, int16_t y, int16_t z) {
	if (containsPoint(x, y, z)) {
		if (voxels.size() > 0) {
			int index = getVoxelIndex(x, y, z);
			if (voxels.size() < index + 1) 
				return { this, nullptr };

			return { this, voxels[index] };
		}

		if (!hasChildren) 
			return { this, nullptr };

		for (OctreeNode* child : children) {
			if (child->containsPoint(x, y, z)) {
				return child->findSiblingVoxel(x, y, z);
			}
		}

		return { this, nullptr };
	}

	if (parentType == OCTREE) {
		return parentOctree->findSiblingVoxel(x, y, z);
	}

	return parent->findSiblingVoxel(x, y, z);
}

Octree* OctreeNode::getParentOctree() {
	if (parentType == NODE) {
		return parent->getParentOctree();
	}

	return parentOctree;
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
			if (voxels[i] == nullptr)
				continue;
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

void OctreeNode::removeVoxel(Voxel* voxel) {
	removeVoxel(intToBytes3(voxel->positionInt));
}

void OctreeNode::removeVoxel(const std::array<uint8_t, 3>& position) {
	if (voxels.size() > 0) {
		int index = getVoxelIndex(position[0], position[1], position[2]);
		if (index < 0 || index > voxels.size() - 1)
			return;
		Voxel* vox = voxels[index];
		voxels[index] = nullptr;
		delete vox;
		return;
	}
	if (!hasChildren) {
		return;
	}
	for (OctreeNode* child : children) {
		if (child->containsPoint(position)) {
			child->removeVoxel(position);
			return;
		}
	}
}




void OctreeNode::calculateVoxelsToRemove(const std::array<uint8_t, 3>& position, float power, std::vector<FoundVoxel>& toRemoveOut) {
	calculateVoxelsToRemove(findSiblingVoxel(position), power, toRemoveOut);
}

void OctreeNode::calculateVoxelsToRemove(const FoundVoxel& fv, float power, std::vector<FoundVoxel>& toRemoveOut) {
	if (fv.voxel != nullptr) {
		float newPower = power - fv.voxel->getMaterial()->strength;
		//float newPower = power - 4;
		if (newPower <= 0.0001f)
			return;
		toRemoveOut.push_back(fv);
		fv.node->addSurroundingVoxelsToRemove(intToBytes3(fv.voxel->positionInt), newPower, toRemoveOut);
	}
}

void OctreeNode::addSurroundingVoxelToRemove(const std::array<int16_t, 3>& voxelPos, float power, std::vector<FoundVoxel>& toRemoveOut) {
	FoundVoxel fv = findSiblingVoxel(voxelPos);

	if (fv.voxel != nullptr) {
		bool alreadyRemoved = false;
		for (FoundVoxel voxRem : toRemoveOut) {
			if (fv.voxel->hasSamePosition(voxRem.voxel)) {
				alreadyRemoved = true;
				break;
			}
		}

		if (!alreadyRemoved)
			fv.node->calculateVoxelsToRemove(fv, power, toRemoveOut);
	}
}

void OctreeNode::addSurroundingVoxelsToRemove(const std::array<uint8_t, 3>& voxelPos, float power, std::vector<FoundVoxel>& toRemoveOut) {
	Voxel* voxel;
	std::array<int16_t, 3>* positions = getSurroundingPositions<uint8_t, int16_t>(voxelPos);
	for (size_t i = 0; i < 6; i++) {
		addSurroundingVoxelToRemove(positions[i], power, toRemoveOut);
	}
}

void OctreeNode::calculateSurroundedFaces(Voxel* voxel) {
	calculateSurroundedFaces(intToBytes3(voxel->positionInt));
}

void OctreeNode::calculateSurroundedFaces(const std::array<uint8_t, 3>& voxelPos) {
	FoundVoxel curVox = findSiblingVoxel(voxelPos);
	Voxel* voxel;
	std::array<int16_t, 3>* positions = getSurroundingPositions<uint8_t, int16_t>(voxelPos);

	for (size_t i = 0; i < 6; i++) {
		FoundVoxel voxel = findSiblingVoxel(positions[i]);
		if (shouldInsertFace(curVox, voxel, true))
			voxel.voxel->materialAndEnabledInt = voxel.voxel->materialAndEnabledInt | faceFlagsInv[i] << 24;
	}
}

uint8_t OctreeNode::calculateEnabledFace(const std::array<uint8_t, 3>& voxelPos) {
	uint8_t en = 0x00;
	FoundVoxel curVox = findSiblingVoxel(voxelPos);
	std::array<int16_t, 3>* positions = getSurroundingPositions<uint8_t, int16_t>(voxelPos);

	for (size_t i = 0; i < 6; i++) {
		if (shouldInsertFace(curVox, findSiblingVoxel(positions[i]))) en |= faceFlags[i];
	}

	return en;
}

void OctreeNode::calculateEnabledFaces() {
	if (hasChildren) {
		for (OctreeNode* child : children) {
			child->calculateEnabledFaces();
		}
	} else if (voxels.size() > 0) {
		for (Voxel* voxel : voxels) {
			if (voxel == nullptr)
				continue;
			//uint8_t* voxelPos = voxel->getPositionBytes();
			std::array<uint8_t, 3> cae = intToBytes3(voxel->materialAndEnabledInt);
			std::array<uint8_t, 3> voxelPos = intToBytes3(voxel->positionInt);
			uint8_t en = calculateEnabledFace(voxelPos);
			voxel->materialAndEnabledInt = bytesToInt(cae[0], cae[1], cae[2], en);
		}
	}
}

/// END OCTREE_NODE ///

/// BEGIN OCTREE ///

Octree::Octree(const std::array<int, 3> pos, uint32_t size) {
	root = OctreeNode({ 0, 0, 0 }, size, this);

	this->pos = pos;
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

	bool first_time_lod = false;

	std::vector<Voxel> voxelCloud;
	root.calculateVAO(&voxelCloud, lod);
	if (!voxelVAOs.count(lod)) {
		first_time_lod = true;
		uint32_t VAO;
		glGenVertexArrays(1, &VAO);
		voxelVAOs.insert({ lod, VAO });
		uint32_t VBO;
		glGenBuffers(1, &VBO);
		voxelVBOs.insert({ lod, VBO });
		voxelAmounts.insert({ lod, voxelCloud.size() });
		std::cout << voxelAmounts[lod] << " voxels drawn\n";
	} else {
		voxelAmounts[lod] = voxelCloud.size();
	}


	
	glBindVertexArray(voxelVAOs[lod]);

	glBindBuffer(GL_ARRAY_BUFFER, voxelVBOs[lod]);
	glBufferData(GL_ARRAY_BUFFER, voxelCloud.size() * sizeof(Voxel), voxelCloud.data(), GL_DYNAMIC_DRAW);

	glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(Voxel), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, sizeof(Voxel), (void*)offsetof(Voxel, materialAndEnabledInt));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	if (first_time_lod) {
		duration<double, std::milli> ms = high_resolution_clock::now() - t1;
		std::cout << ms.count() << "ms to build voxel chunk VAO\n";
	}
}

float getNoiseHeight(fnl_state* noise, float xn, float zn) {
	float data = (fnlGetNoise2D(noise, xn, zn) + 1) * 50;
	data += fnlGetNoise2D(noise, xn * 10, zn * 10) * 3;
	data += fnlGetNoise2D(noise, xn * 100, zn * 100) * 0.1f;
	return std::round(data);
}

Voxel* Octree::createVoxel(uint8_t x, uint32_t i, uint8_t z, uint8_t matID) {
	uint8_t enabledFaces = 0x00;
	uint32_t voxelPosInt = bytesToInt(x, i, z, 1);
	return new Voxel{ voxelPosInt, bytesToInt(matID, 0x00, 0x00, enabledFaces) };
}

FoundVoxel Octree::findSiblingVoxel(int16_t x, int16_t y, int16_t z) {
	//PRINT("Outside of tree");
	if (x < 0) {
		x += 256;
		return siblings[0] == nullptr ? FoundVoxel{ nullptr, nullptr } : siblings[0]->root.findSiblingVoxel(x, y, z);
	} else if (x > 255) {
		x -= 256;
		return siblings[1] == nullptr ? FoundVoxel{ nullptr, nullptr } : siblings[1]->root.findSiblingVoxel(x, y, z);
	} else if (y < 0) {
		y += 256;
		return siblings[2] == nullptr ? FoundVoxel{ nullptr, nullptr } : siblings[2]->root.findSiblingVoxel(x, y, z);
	} else if (y > 255) {
		y -= 256;
		return siblings[3] == nullptr ? FoundVoxel{ nullptr, nullptr } : siblings[3]->root.findSiblingVoxel(x, y, z);
	} else if (z < 0) {
		z += 256;
		return siblings[4] == nullptr ? FoundVoxel{ nullptr, nullptr } : siblings[4]->root.findSiblingVoxel(x, y, z);
	} else {
		z -= 256;
		return siblings[5] == nullptr ? FoundVoxel{ nullptr, nullptr } : siblings[5]->root.findSiblingVoxel(x, y, z);
	}
}


void Octree::makeNoiseTerrain(std::array<int32_t, 3> pos) {

	using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	using std::chrono::duration;
	using std::chrono::milliseconds;

	//auto t1 = high_resolution_clock::now();

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

			// TODO: improve this!!!!

			y = std::min((float)OCTREE_SIZE, y);
			int32_t curY = 0;
			/*for (int32_t i = curY; i < 30; i++, curY++) {
				if (i >= y) {
					insert(createVoxel(x, i, z, mat::water));
				} else if (i < 25) {
					insert(createVoxel(x, i, z, mat::gabbro));
				} else {
					insert(createVoxel(x, i, z, mat::basalt));
				}
			}*/

			for (size_t i = curY; i < y && i < 25; i++, curY++) {
				insert(createVoxel(x, i, z, mat::gabbro));
			}
			for (size_t i = curY; i < y && i < 30; i++, curY++) {
				insert(createVoxel(x, i, z, mat::basalt));
			}
			for (size_t i = curY; i < y && i < 35; i++, curY++) {
				insert(createVoxel(x, i, z, mat::sand));
			}
			for (size_t i = curY; i < y && i < 45; i++, curY++) {
				insert(createVoxel(x, i, z, mat::soil));
			}
			for (size_t i = curY; i < y && i < 55; i++, curY++) {
				insert(createVoxel(x, i, z, mat::shale));
			}
			for (size_t i = curY; i < y && i < 65; i++, curY++) {
				insert(createVoxel(x, i, z, mat::limestone));
			}
			for (size_t i = curY; i < y; i++, curY++) {
				insert(createVoxel(x, i, z, mat::snow));
			}

		}
	}




	auto t1 = high_resolution_clock::now();
	calculateEnabledFaces();

	duration<double, std::milli> ms = high_resolution_clock::now() - t1;
	std::cout << ms.count() << "ms to calculate enabled faces\n";
}

void Octree::calculateEnabledFaces() {
	root.calculateEnabledFaces();
}

void Octree::fillVoxelRenderer(VoxelRendererComp* renderer, uint32_t lod, bool reFill) {
	if (reFill || !voxelVAOs.count(lod))
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

std::vector<FoundVoxel> Octree::removeVoxels(const std::array<uint8_t, 3>& position, uint16_t power) {
	std::vector<FoundVoxel> toRemove;
	root.calculateVoxelsToRemove(position, power, toRemove);

	std::vector<Octree*> affectedOctrees;

	

	for (FoundVoxel fv : toRemove) {
		fv.node->removeVoxel(fv.voxel);
	}
	for (FoundVoxel fv : toRemove) {
		fv.node->calculateSurroundedFaces(fv.voxel);
	}

	return toRemove;
}


void Octree::setSibling(Octree* octree, int side) {
	// TODO: Recalculate the faces on this side edge
	siblings[side] = octree;
}

void Octree::setSiblings(const std::vector<Octree*>& octrees) {
	if (octrees.size() == 0) return;

	std::array<int, 3>* surrPositions = getSurroundingPositions<int, int>(pos);

	for (Octree* octree : octrees) {
		// TODO OPTIMIZATION: Calculate if the distance from current is more than 1, then continue
		for (size_t i = 0; i < 6; i++) {
			if (isSamePosition<int>(octree->pos, surrPositions[i])) {
				siblings[i] = octree;
				octree->setSibling(this, DIR::getOpposite(i));
				break;
			}
		}
	}
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

DIR::Side DIR::getOpposite(int otherSide){
	switch (otherSide) {
	case LEFT:
		return RIGHT;
	case RIGHT:
		return LEFT;
	case DOWN:
		return UP;
	case UP:
		return DOWN;
	case BACK:
		return FRONT;
	case FRONT:
		return BACK;
	}

	return LEFT;
}
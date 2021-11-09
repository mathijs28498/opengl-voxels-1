#include "../Headers/Global/VoxelStructs.h"

Material materials[mat::empty] = {
	{ mat::gabbro, false, true, { 0.20, 0.26, 0.18, 1 }, 3 },
	{ mat::basalt, false, true, { 0.33, 0.31, 0.30, 1 }, 2 },
	{ mat::sand, false, true, { 0.97, 0.94, 0.64, 1 }, 1 },
	{ mat::soil, false, true, { 0.49, 0.78, 0.31, 1 }, 1 },
	{ mat::shale, false, true, { 0.50, 0.52, 0.53, 1 }, 5 },
	{ mat::limestone, false, true, { 0.75, 0.74, 0.56, 1 }, 3 },
	{ mat::snow, false, true, { 0.9, 0.9, 0.9, 1 }, 1 },
	{ mat::water, true, false, { 0.14, 0.36, 0.99, 0.5 }, 1 },
};

std::string getMaterialShaderString() {
	static std::string materialString = "";
	if (materialString != "") return materialString;

	materialString += "[" + std::to_string(mat::empty) + "] = Material[" + std::to_string(mat::empty) + "] (";

	for (size_t i = 0; i < mat::empty; i++) {
		Material m = materials[i];
		materialString += "Material(vec4(" + std::to_string(m.color[0]) + "," + std::to_string(m.color[1]) + "," 
			+ std::to_string(m.color[2]) + "," + std::to_string(m.color[3]) + "))";
		if (i != mat::empty - 1)
			materialString += ",";
	}
	materialString += ")";

	return materialString;
}


Event<Entity*> addEntityEvent;
Event2<std::vector<Entity*>*, std::string> getEntitiesWithComponentEvent;

glm::vec3 Voxel::getModelPosition(const glm::vec3& modelPosition) const {
	glm::vec3 position = {
		positionInt & 0xFF,
		(positionInt >> 8) & 0xFF,
		(positionInt >> 16) & 0xFF,
	};

	return position * VOX_SIZE + modelPosition;
}

bool Voxel::hasSamePosition(Voxel* otherVoxel) const {
	return hasSamePosition(intToBytes3(otherVoxel->positionInt));
}

bool Voxel::hasSamePosition(const std::array<uint8_t, 3>& otherPos) const {
	std::array<uint8_t, 3> voxPos = intToBytes3(positionInt);
	return voxPos[0] == otherPos[0] && voxPos[1] == otherPos[1] && voxPos[2] == otherPos[2];
}

bool Voxel::isNull() const {
	return positionInt == 0x00 && materialAndEnabledInt == 0x00;
}

Material* Voxel::getMaterial() const {
	return &materials[intToBytes3(materialAndEnabledInt)[0]];
}

Voxel Voxel::getVoxelCopy(const Voxel& originalVoxel) {
	Voxel voxel{ originalVoxel.positionInt, originalVoxel.materialAndEnabledInt };
	return voxel;
}

VoxelAABB::VoxelAABB(const Voxel& voxel, const glm::vec3& modelPosition) {
	glm::vec3 pos = voxel.getModelPosition(modelPosition);
	float sizeMult = std::pow(2, (voxel.positionInt >> 24) - 1);

	min = pos - HALF_VOX_SIZE;
	max = pos + VOX_SIZE * sizeMult - HALF_VOX_SIZE;
}

VoxelAABB::VoxelAABB(const glm::vec3& pos, float size) {
	min = pos - HALF_VOX_SIZE;
	max = pos + size - HALF_VOX_SIZE;
}

float VoxelAABB::rayCollision(const Ray& r) {
	float ignored;
	return rayCollision(r, ignored);
}

float VoxelAABB::rayCollision(const Ray& r, float& tmin) {
	float t0 = (min.x - r.origin.x) / r.direction.x;
	float t1 = (max.x - r.origin.x) / r.direction.x;

	float t2 = (min.y - r.origin.y) / r.direction.y;
	float t3 = (max.y - r.origin.y) / r.direction.y;

	float t4 = (min.z - r.origin.z) / r.direction.z;
	float t5 = (max.z - r.origin.z) / r.direction.z;

	tmin = std::max(std::max(std::min(t0, t1), std::min(t2, t3)), std::min(t4, t5));
	float tmax = std::min(std::min(std::max(t0, t1), std::max(t2, t3)), std::max(t4, t5));

	if (tmax < 0 || tmin > tmax)
		return -1;

	if (tmin < 0)
		return tmax;

	return tmin;
}
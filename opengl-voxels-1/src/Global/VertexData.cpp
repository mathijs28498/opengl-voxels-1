#include "../Headers/Global/VertexData.h"


Event<Entity*> addEntityEvent;
Event2<std::vector<Entity*>*, std::string> getEntitiesWithComponentEvent;

float cubeVertices[]{
	0.0f, 0.0f, 0.0f
};

float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
};

//int indices[] = {
//
//};

////
//float vertices[] = {
//	0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
//	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
//	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//	-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f
//};
//float vertices[] = {
//	0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
//	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
//	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
//};

//float texCoords[] = {
//	0.0f, 0.0f,
//	1.0f, 0.0f,
//	0.5f, 1.0f,
////};
//
//uint32_t indices[]{
//	0, 1, 3,
//	1, 2, 3
//};

//uint32_t indices[]{
//	0, 1, 2
//};
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f, 2.0f, -2.5f),
	glm::vec3(1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f)
};

Material materials[]{
	{glm::vec3(0.0215f, 0.1745f, 0.0215f), glm::vec3(0.07568, 0.61424, 0.07568), glm::vec3(0.633, 0.727811, 0.633), 64 },
	{glm::vec3(0.135, 0.2225, 0.1575), glm::vec3(0.54, 0.89, 0.63), glm::vec3(0.316228, 0.316228, 0.316228), 16 },
	{glm::vec3(0.05375, 0.05, 0.06625), glm::vec3(0.18275, 0.17, 0.22525), glm::vec3(0.332741, 0.328634, 0.346435), 32 },
	{glm::vec3(0.25, 0.20725, 0.0215f), glm::vec3(1, 0.829, 0.829), glm::vec3(0.296648, 0.296648, 0.296648), 16 },
	{glm::vec3(0.1745, 0.01175, 0.20725), glm::vec3(0.61424, 0.04136, 0.04136), glm::vec3(0.727811, 0.626959, 0.626959), 64 },
	{glm::vec3(0.1, 0.18725, 0.01175), glm::vec3(0.396, 0.74151, 0.69102), glm::vec3(0.297254, 0.30829, 0.306678), 16 },
	{glm::vec3(0.329412, 0.223529, 0.1745), glm::vec3(0.780392, 0.113725, 0.07568), glm::vec3(0.992157, 0.941176, 0.807843), 32 },
	{glm::vec3(0.2125, 0.1275, 0.027451), glm::vec3(0.714, 0.4284, 0.18144), glm::vec3(0.393548, 0.271906, 0.166721), 32 },
	{glm::vec3(0.25, 0.25, 0.25), glm::vec3(0.4, 0.4, 0.4), glm::vec3(0.774597, 0.774597, 0.774597), 64 },
	{glm::vec3(0.19125, 0.0735, 0.0225), glm::vec3(0.7038, 0.27048, 0.0828), glm::vec3(0.256777, 0.137622, 0.086014), 16 },
};

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

glm::vec3 pointLightColors[] = {
	glm::vec3(0.654, 0.511, 0.82),
	glm::vec3(0.14, 0.5, 0.9),
	glm::vec3(0.26, 0.19, 0.78),
	glm::vec3(0.53, 0.13, 0.12),
};
/*glm::vec3 pointLightColors[] = {
	glm::vec3(1, 1, 1),
	glm::vec3(1, 1, 1),
	glm::vec3(1, 1, 1),
	glm::vec3(1, 1, 1),
};*/


glm::vec3 Voxel::getPosition(glm::vec3& posOctree) {
	uint8_t* posBytes = positionFromInt(positionInt);
	glm::vec3 pos = (glm::vec3(posBytes[0], posBytes[1], posBytes[2]) + posOctree) * VOX_SIZE;

	pos += posOctree;
	pos *= VOX_SIZE;
	return pos;
}

glm::vec3 Voxel::getModelPosition(const glm::vec3& modelPosition) const {
	glm::vec3 position = {
		positionInt & 0xFF,
		(positionInt >> 8) & 0xFF,
		(positionInt >> 16) & 0xFF,
	};

	//PRINT_VEC3(modelPosition);

	return position * VOX_SIZE + modelPosition;
}

bool Voxel::rayCastCollision(Ray& ray, glm::vec3& posOctree, RayCollision* rayCollision) {
	glm::vec3 pos = getPosition(posOctree);
	glm::vec3 min = pos - VOX_SIZE / 2;
	glm::vec3 max = pos + VOX_SIZE / 2;

	float tMinX = (min.x - ray.origin.x) / ray.direction.x;
	float tMinY = (min.y - ray.origin.y) / ray.direction.y;
	float tMinZ = (min.z - ray.origin.z) / ray.direction.z;

	float tMaxX = (max.x - ray.origin.x) / ray.direction.x;
	float tMaxY = (max.y - ray.origin.y) / ray.direction.y;
	float tMaxZ = (max.z - ray.origin.z) / ray.direction.z;

	float tMin = std::max(std::max(std::min(tMinX, tMaxX), std::min(tMinY, tMaxY)), std::min(tMinZ, tMaxZ));
	float tMax = std::min(std::min(std::max(tMinX, tMaxX), std::max(tMinY, tMaxY)), std::max(tMinZ, tMaxZ));

	if (tMax < 0 || tMin > tMax) {
		return false;
	}

	rayCollision->distance = tMin > 0 ? tMin : tMax;
	rayCollision->point = ray.origin + ray.direction * rayCollision->distance;

	return true;
}

Voxel Voxel::getVoxelCopy(Voxel originalVoxel) {
	Voxel voxel{ originalVoxel.positionInt, originalVoxel.colorAndEnabledInt };
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
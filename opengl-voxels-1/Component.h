#pragma once

#include "Shader.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include<map>

// TODO: Make these not auto anymore
constexpr auto ComponentCount = 32;
constexpr auto SystemCount = 8;

struct Component {
	virtual void ignored() {};
	const std::string type;
	Component(const std::string type) : type(type) {};
};

struct Transform : Component {
	Transform(glm::vec3 pos) : Component("Transform"), position(pos) {};
	glm::vec3 position;
};

struct Velocity : Component {
	Velocity(glm::vec3 vel) : Component("Velocity"), vel(vel) {};
	glm::vec3 vel;
};

struct VoxelRenderer : Component {
	VoxelRenderer(Shader* shader, Camera* camera, uint32_t VAO, uint32_t voxelAmount) : 
		Component("VoxelRenderer"), shader(shader), camera(camera), VAO(VAO), voxelAmount(voxelAmount) {};
	Shader* shader;
	Camera* camera;
	uint32_t VAO;
	uint32_t voxelAmount;
};

// TODO: Deconstruct camera and use it in voxelrenderer (have a main camera)
struct CameraComp : Component {
	CameraComp(Camera camera) : Component("CameraComp"), camera(camera) {};
	Camera camera;
};

class Entity {
public:
	~Entity() {
		// TODO: Components are created on the heap, probably need to be deleted here but gives an error
		/*for (auto i = components.rbegin(); i != components.rend(); ++i) {
			delete i->second;
		}*/
	}

	void insertComponent(Component* component);
	bool hasRequirement(std::string requirement);
	Component* getComponent(std::string key);

private:
	std::map<std::string, Component*> components;
};
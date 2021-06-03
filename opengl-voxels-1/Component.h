#pragma once

#include "Shader.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include<map>

struct Component {
	virtual void ignored() {};
};

struct Transform : Component {
	Transform(glm::vec3 pos) : position(pos) {};
	glm::vec3 position;
};

struct Velocity : Component {
	Velocity(glm::vec3 vel) : vel(vel) {};
	glm::vec3 vel;
};

struct VoxelRenderer : Component {
	VoxelRenderer(Shader* shader, Camera* camera, uint32_t VAO, uint32_t voxelAmount) : 
		shader(shader), camera(camera), VAO(VAO), voxelAmount(voxelAmount) {};
	Shader* shader;
	Camera* camera;
	uint32_t VAO;
	uint32_t voxelAmount;
};

// TODO: Untangle camera and use it in voxelrenderer (have a main camera)
struct CameraComp : Component {
	CameraComp(Camera camera) : camera(camera) {};
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

protected:
	std::map<std::string, Component*> components;
};

class MainCamera : Entity {
	MainCamera();
};

template <typename T>
T* getComponentFromEntity(Entity* entity) {
	return dynamic_cast<T*>(entity->getComponent(typeid(T).name()));
}
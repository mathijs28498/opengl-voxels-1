#pragma once

#include "Shader.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <map>

struct Component {
	// Virtual member necessary to cast derived classes to this base class
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
	CameraComp(Camera* camera) : camera(camera) {};
	Camera* camera;
};

struct KeyInput : Component {
	KeyInput() {
		std::fill_n(keyPress, GLFW_KEY_LAST, false);
		std::fill_n(keyRepeat, GLFW_KEY_LAST, false);
		std::fill_n(keyRelease, GLFW_KEY_LAST, false);
	}
	bool keyPress[GLFW_KEY_LAST];
	bool keyRepeat[GLFW_KEY_LAST];
	bool keyRelease[GLFW_KEY_LAST];
};

class Entity {
public:
	Entity() {};
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
	Entity(std::map<std::string, Component*> components) : components(components) {};
	std::map<std::string, Component*> components;
};

class MainCamera : Entity {
public:
	void insertComponent(Component* component) = delete;
	void setCamera(CameraComp* camera);
};

template <typename T>
T* getComponentFromEntity(Entity* entity) {
	return dynamic_cast<T*>(entity->getComponent(typeid(T).name()));
}
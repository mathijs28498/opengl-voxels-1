#pragma once

#include "../Shader.h"
#include "../Camera.h"

#include <glm/glm.hpp>
#include <map>


template <typename T>
// Stands for getClassName
std::string gcn(T c) { return typeid(c).name(); };

struct Component {
	// Virtual member necessary to cast derived classes to this base class
	virtual void ignored() {};
};

struct TransformComp : Component {
	TransformComp() {};
	TransformComp(glm::vec3 pos) : position(pos) {};
	glm::vec3 position;
};

struct VelocityComp : Component {
	VelocityComp() {};
	VelocityComp(glm::vec3 vel) : vel(vel) {};
	glm::vec3 vel;
};

struct VoxelRendererComp : Component {
	VoxelRendererComp() {};
	VoxelRendererComp(Shader* shader, Camera* camera, uint32_t VAO, uint32_t voxelAmount) :
		shader(shader), camera(camera), VAO(VAO), voxelAmount(voxelAmount) {};
	Shader* shader;
	Camera* camera;
	uint32_t VAO;
	uint32_t voxelAmount;
};

// TODO: Untangle camera and use it in voxelrenderer (have a main camera)
struct CameraComp : Component {
	CameraComp() {};
	CameraComp(Camera* camera) : camera(camera) {};
	Camera* camera;
};

struct KeyInputComp : Component {
	KeyInputComp() {
		std::fill_n(keyPress, GLFW_KEY_LAST, false);
		std::fill_n(keyRepeat, GLFW_KEY_LAST, false);
		std::fill_n(keyRelease, GLFW_KEY_LAST, false);
	}
	bool keyPress[GLFW_KEY_LAST];
	bool keyRepeat[GLFW_KEY_LAST];
	bool keyRelease[GLFW_KEY_LAST];
};

struct MouseCursorInputComp : Component {
	MouseCursorInputComp() : pos(glm::vec2(-1)) {}
	glm::vec2 pos;
};

struct BoundingBoxRendererComp : Component {
	BoundingBoxRendererComp() {};
	BoundingBoxRendererComp(Shader* shader, Camera* camera, uint32_t VAO, uint32_t boundingBoxAmount, bool show) :
		shader(shader), camera(camera), VAO(VAO), boundingBoxAmount(boundingBoxAmount), show(show) {};
	Shader* shader;
	Camera* camera;
	uint32_t VAO;
	uint32_t boundingBoxAmount;
	bool show;
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
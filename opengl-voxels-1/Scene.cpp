#include "Scene.h"
#include "SustainedSystem.h"
#include "EventSystem.h"

std::vector<SustainedSystem*> systems{
	new VoxelRenderSystem(),
	new CameraMoveSystem(),
	new ResetKeyInputSystem(),
};

KeyboardSystem keyboardSystem;

void Scene::addEntity(Entity* entity) {
	entities.push_back(entity);
}

void Scene::keyCallBack(GLFWwindow* window, int key, int action) {
	for (SustainedSystem* system : systems) {
		for (auto i = entities.rbegin(); i != entities.rend(); ++i) {
			keyboardSystem.doEvent(*i, window, key, action);
		}
	}
};

void Scene::start() {
	for (SustainedSystem* system : systems) {
		for (auto i = entities.rbegin(); i != entities.rend(); ++i) {
			system->doStart(*i);
		}
	}
};

void Scene::update() {

	for (SustainedSystem* system : systems) {
		for (auto i = entities.rbegin(); i != entities.rend(); ++i) {
			system->doUpdate(*i);
		}
	}
};
#include "Scene.h"
#include "SustainedSystem.h"
#include "EventSystem.h"

std::vector<SustainedSystem*> systems{
	new CameraMoveSystem(),
	new VoxelRenderSystem(),
	new BoundingBoxRendererSystem(),

	new ResetKeyInputSystem(),
};

KeyboardSystem keyboardSystem;
CameraMouseCursorSystem cameraMouseCursorSystem;

void Scene::addEntity(Entity* entity) {
	entities.push_back(entity);
}

void Scene::keyCallback(int key, int action) {
	for (SustainedSystem* system : systems) {
		for (auto i = entities.rbegin(); i != entities.rend(); ++i) {
			keyboardSystem.doEvent(*i, key, action);
		}
	}
};

void Scene::mouseCursorCallback(double xpos, double ypos) {
	for (SustainedSystem* system : systems) {
		for (auto i = entities.rbegin(); i != entities.rend(); ++i) {
			cameraMouseCursorSystem.doEvent(*i, xpos, ypos);
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
}

void Scene::fixedUpdate() {
	for (SustainedSystem* system : systems) {
		for (auto i = entities.rbegin(); i != entities.rend(); ++i) {
			system->doFixedUpdate(*i);
		}
	}
};
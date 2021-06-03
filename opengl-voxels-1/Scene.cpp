#include "Scene.h"
#include "System.h"

std::vector<System*> systems{
	new VoxelRenderSystem(),
};

void Scene::addEntity(Entity* entity) {
	entities.push_back(entity);
}

void Scene::start() {
	for (System* system : systems) {
		for (auto i = entities.rbegin(); i != entities.rend(); ++i) {
			system->doStart(*i);
		}
	}
};

void Scene::update() {
	for (System* system : systems) {
		for (auto i = entities.rbegin(); i != entities.rend(); ++i) {
			system->doUpdate(*i);
		}
	}
};
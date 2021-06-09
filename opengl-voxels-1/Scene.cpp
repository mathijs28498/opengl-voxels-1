#include "Scene.h"
#include "SustainedSystem.h"

std::vector<SustainedSystem*> systems{
	new VoxelRenderSystem(),
};

void Scene::addEntity(Entity* entity) {
	entities.push_back(entity);
}

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
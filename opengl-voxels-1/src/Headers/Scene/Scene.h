#pragma once

#include "../ECS/Component.h"
#include "../Objects/Octree.h"
#include "../Observer pattern/SceneObserver.h"

#include <vector>
#include <queue>



class Scene {
public:
	Scene();
	// TODO: Probably need to do garbage collection but this gives an error (entities are on heap)
	/*~Scene() {
	* delete mainCamera;
		for (size_t i = 0; i < entities.size(); i++) {
			delete entities[i];
		}
	};*/


	void keyCallback(int key, int action);
	void mouseCursorCallback(double xpos, double ypos);

	void update();
	void fixedUpdate();

private:
	std::vector<Entity*> entities{};
	void addEntity(Entity* entity);
	bool isStarted = false;
	std::queue<Entity*> toAdd;

	class SceneObserver : public Observer<Entity*> {
	public:
		SceneObserver(Scene* scene) : scene(scene) {}

		void update(Entity* entity) override {
			scene->addEntity((Entity*)entity);
		}
	private:
		Scene* scene;
	};

	class GetEntitiesObserver : public Observer2<std::vector<Entity*>*, std::string> {
	public:
		GetEntitiesObserver(Scene* scene) : scene(scene) {};

		void update(std::vector<Entity*>* entities, std::string compName) override {
			// TODO: 
			//		 - Look through entities to find with compName
			//		 - Put in std::vector
			for (Entity* entity : scene->entities) {
				if (entity->hasRequirement(compName)) {
					entities->push_back(entity);
				}
			}
		}

	private:
		Scene* scene;
	};
};


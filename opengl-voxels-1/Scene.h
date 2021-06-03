#pragma once

#include "Component.h"
#include "Octree.h"

#include <vector>



class Scene {
public:
	Scene() {};
	// TODO: Probably need to do garbage collection but this gives an error (entities are on heap)
	/*~Scene() {
		for (size_t i = 0; i < entities.size(); i++) {
			delete entities[i];
		}
	};*/

	void addEntity(Entity* entity);

	void start();
	void update();
	//void fixedUpdate();

private:
	//Octree terrain;
	std::vector<Entity*> entities{};
};
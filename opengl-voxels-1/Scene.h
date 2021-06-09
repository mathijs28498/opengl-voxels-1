#pragma once

#include "Component.h"
#include "Octree.h"

#include <vector>



class Scene {
public:
	// TODO: Probably need to do garbage collection but this gives an error (entities are on heap)
	/*~Scene() {
	* delete mainCamera;
		for (size_t i = 0; i < entities.size(); i++) {
			delete entities[i];
		}
	};*/

	void addEntity(Entity* entity);

	void keyCallback(int key, int action);
	void mouseCursorCallback(double xpos, double ypos);

	void start();
	void update();
	void fixedUpdate();

private:
	std::vector<Entity*> entities{};
};
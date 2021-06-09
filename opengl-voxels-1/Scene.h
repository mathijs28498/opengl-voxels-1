#pragma once

#include "Component.h"
#include "Octree.h"

#include <vector>



class Scene {
public:
	Scene() : mainCamera(new MainCamera()) {};
	// TODO: Probably need to do garbage collection but this gives an error (entities are on heap)
	/*~Scene() {
	* delete mainCamera;
		for (size_t i = 0; i < entities.size(); i++) {
			delete entities[i];
		}
	};*/
	void setMainCameraComponent(CameraComp* cameraComp) { mainCamera->setCamera(cameraComp); };
	MainCamera* getMainCamera() { return mainCamera; };

	void addEntity(Entity* entity);

	void keyCallback(int key, int action);
	void mouseCursorCallback(double xpos, double ypos);

	void start();
	void update();
	void fixedUpdate();

private:
	MainCamera* mainCamera;
	//Octree terrain;
	std::vector<Entity*> entities{};
};
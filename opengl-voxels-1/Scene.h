#pragma once

#include "Octree.h"

class Scene {
public:
	void draw();

private:
	Octree terrain;

};
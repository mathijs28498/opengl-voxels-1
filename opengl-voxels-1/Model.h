#pragma once

#include "Shader.h"
#include "Mesh.h"

#include <iostream>

class Model {
public:
	Model(const char* path) {
		loadModel(path);
	}
	void draw(Shader& shader);

private:
	std::vector<Mesh> meshes;
	std::string directory;

	void loadModel(std::string path);
};
#include "Model.h"
#include "GLHelperFunctions.h"


void Model::draw(Shader& shader) {
	for (uint32_t i = 0; i < meshes.size(); i++) {
		meshes[i].draw(shader);
	}
}


void Model::loadModel(std::string path) {	}
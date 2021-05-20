#include "Model.h"
#include "GLHelperFunctions.h"


Model::Model(glm::vec3 scale, uint32_t xAmount, uint32_t yAmount, uint32_t zAmount) {
	this->scale = scale;
	this->xAmount = xAmount;
	this->yAmount = yAmount;
	this->zAmount = zAmount;

	cubeVAO = getCubeVAO(scale);
}

void Model::draw(Shader* shader) {
	glBindVertexArray(cubeVAO);
	shader->setFloat("glfwTime", glfwGetTime());

	for (uint32_t x = 0; x < xAmount; x++) {
		for (uint32_t y = 0; y < yAmount; y++) {
			for (uint32_t z = 0; z < zAmount; z++) {
				shader->setVec3("pos", glm::vec3(x, y, z));

				shader->setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(scale.x * x, scale.y * y, scale.z * z)));
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			}
		}
	}

	glBindVertexArray(0);
}


void Model::loadModel(std::string path) { }
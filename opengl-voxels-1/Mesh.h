#pragma once

#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <iostream>
#include <vector>


struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture {
	uint32_t id;
	std::string type;
	const char* path;
};

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures);
	void draw(Shader& shader);

private:
	uint32_t VAO, VBO, EBO;
	void setupMesh();
};
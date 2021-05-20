#include "GLHelperFunctions.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


uint32_t getVAO() {
	uint32_t VAO;
	glGenVertexArrays(1, &VAO);
	uint32_t VBO;
	glGenBuffers(1, &VBO);
	uint32_t EBO;
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	/*
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);*/

	glBindVertexArray(0);
	return VAO;
}

uint32_t getCubeVAO(glm::vec3 scale) {
	uint32_t VAO;
	uint32_t VBO;
	uint32_t EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	float vertices[] = {
		// Front
		 1.0f * scale.x, 1.0f * scale.y, 1.0f * scale.z,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f, // top right
		 1.0f * scale.x, 0.0f,			 1.0f * scale.z,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, // bottom right
		 0.0f,			 0.0f,			 1.0f * scale.z,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f, // bottom left
		 0.0f,			 1.0f * scale.y, 1.0f * scale.z,		0.0f, 0.0f, 1.0f,		0.0f, 1.0f, // top left

		// Back
		 1.0f * scale.x, 1.0f * scale.y, 0.0f,				0.0f, 0.0f, 1.0f,		1.0f, 1.0f, // top right
		 1.0f * scale.x, 0.0f,			 0.0f,				0.0f, 0.0f, 1.0f,		1.0f, 0.0f, // bottom right
		 0.0f,			 0.0f,			 0.0f,				0.0f, 0.0f, 1.0f,		0.0f, 0.0f, // bottom left
		 0.0f,			 1.0f * scale.y, 0.0f,				0.0f, 0.0f, 1.0f,		0.0f, 0.0f, // top left
	};

	uint32_t indices[]{
		0, 1, 2,	// Front
		3, 0, 2,

		6, 5, 4,	// Back
		6, 4, 7,

		7, 3, 2,	// Left
		7, 2, 6,

		4, 1, 0,	// Right
		4, 5, 1,

		3, 4, 0,	// Up
		7, 4, 3,

		2, 1, 6,	// Down
		1, 5, 6,
	};

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//// TODO ERROR: Maybve change this back to GL_FALSE (maybe bad for performance?)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 8, (void*)3);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8, (void*)6);
	glEnableVertexAttribArray(2);


	glBindVertexArray(0);
	return VAO;
}

uint32_t loadTexture(const char* imagePath, GLenum format, bool shouldFlip) {
	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(shouldFlip);
	uint8_t* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		throw std::runtime_error("failed to load texture\n");
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
	return texture;
}

uint32_t textureFromFile(const char* path, const std::string& directory) {
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)     {
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}     else     {
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
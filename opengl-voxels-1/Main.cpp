#include "Window.h"

#include <iostream>
//
//#include <glad/glad.h>
//#include<GLFW/glfw3.h>

const int WIDTH = 800;
const int HEIGHT = 600;
const char* TITLE = "Voxels everywhere";

void processInput(GLFWwindow* window) {
	Camera* cam = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	cam->moveCamera(window);
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

int main() {
	try {
		Window window(WIDTH, HEIGHT, TITLE, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		Camera cam(window.window, glm::vec3(0, 1.5f, -2), 0.06f, 0.1f, WIDTH, HEIGHT);

		uint32_t cubeVAO = getCubeVAO(glm::vec3(0.1f, 0.1f, 2));
		Shader simpleShader("shaders/simple.vert", "shaders/simple.frag");

		while (!window.shouldClose()) {
			processInput(window.window);
			window.beginLoop();

			glUseProgram(simpleShader.ID);
			glBindVertexArray(cubeVAO);

			glm::mat4 view = glm::lookAt(glm::vec3(0, 0, -1), glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));


			simpleShader.setMat4("model", glm::mat4(1.0f));
			cam.setUniforms(&simpleShader);

			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			//glDrawArrays(GL_TRIANGLES, 0, 3);

			/*simpleShader.use();
			simpleShader.setVec3("color", glm::vec3(1, 0, 0));
			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 3);*/

			glBindVertexArray(0);

			window.endLoop();
		}
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}
#include "Window.h"
#include "GLHelperFunctions.h"
#include "Model.h"

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

int main() {
	try {
		Window window(WIDTH, HEIGHT, TITLE, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		Camera cam(window.window, glm::vec3(0, 1.5f, 2), 0.06f, 0.1f, WIDTH, HEIGHT);

		Model plane(glm::vec3(0.1f), 20, 20, 20);
		Shader simpleShaderGeom("shaders/testGeom.vert", "shaders/testGeom.geom", "shaders/testGeom.frag");
		Shader simpleShader("shaders/simple.vert", "shaders/simple.frag");

		//float points[] = {
		//	0, 0, 0,
		//	0.25f, 0, 0,
		//	0, 0.25f, 0,
		//	0.25f, 0.25f, 0,
		//	0, 0, -0.25f,
		//	0.25f, 0, -0.25f,
		//	0, 0.25f, -0.25f,
		//	0.25f, 0.25f, -0.25f,
		//};

		uint32_t points[]{
			0, 0, 0,
			1, 0, 0,
			2, 0, 0,
			4, 0, 0,
		};

		uint32_t VAO;
		uint32_t VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_UNSIGNED_INT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);


		while (!window.shouldClose()) {
			processInput(window.window);
			window.beginLoop();

			simpleShaderGeom.use();
			cam.setUniforms(&simpleShaderGeom);
			simpleShaderGeom.setMat4("model", glm::mat4(1.0f));
			glBindVertexArray(VAO);
			glDrawArrays(GL_POINTS, 0, 8);

			/*simpleShader.use();
			cam.setUniforms(&simpleShader);
			plane.draw(&simpleShader);*/

			window.endLoop();
		}
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}
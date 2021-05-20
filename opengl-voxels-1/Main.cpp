#include "Window.h"
#include "GLHelperFunctions.h"
#include "Model.h"

#define FNL_IMPL
#include "FastNoiseLite.h"

#include <iostream>
//
//#include <glad/glad.h>
//#include<GLFW/glfw3.h>

const int WIDTH = 800;
const int HEIGHT = 600;
const char* TITLE = "Voxels everywhere";
const int xAmount = 200;
const int zAmount = 200;


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
		Camera cam(window.window, glm::vec3(0, 1.5f, 2), 0.1f * 5, 5, 0.1f, WIDTH, HEIGHT);

		

		Model plane(glm::vec3(0.1f), 20, 20, 20);
		Shader simpleShaderGeom("shaders/testGeom.vert", "shaders/testGeom.geom", "shaders/testGeom.frag");
		Shader simpleShader("shaders/simple.vert", "shaders/simple.frag");

		fnl_state noise = fnlCreateState();
		noise.noise_type = FNL_NOISE_PERLIN;

		int32_t points[xAmount * zAmount * 3 * 2] = { 0 };
		for (uint32_t z = 0; z < zAmount; z++) {
			for (uint32_t x = 0; x < xAmount; x++) {
				float data = fnlGetNoise2D(&noise, x, z) * 50;
				data += fnlGetNoise2D(&noise, x * 10, z * 10) * 3;
				data += fnlGetNoise2D(&noise, x * 100, z * 100) * 0.1f;
				size_t pos = (z * xAmount + x) * 6;
				points[pos] = x - xAmount / 2;
				points[pos + 1] = std::round(data);
				points[pos + 2] = z - zAmount / 2;
				points[pos + 3] = x - xAmount / 2;
				points[pos + 4] = std::round(data) - 1;
				points[pos + 5] = z - zAmount / 2;
			}
		}

		uint32_t VAO;
		uint32_t VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_INT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);


		while (!window.shouldClose()) {
			processInput(window.window);
			window.beginLoop();

			simpleShaderGeom.use();
			cam.setUniforms(&simpleShaderGeom);
			simpleShaderGeom.setMat4("model", glm::mat4(1.0f));
			simpleShaderGeom.setVec3("gColor", glm::vec3(0, 1, 0));
			simpleShaderGeom.setFloat("voxSize", 0.25f);
			glBindVertexArray(VAO);
			glDrawArrays(GL_POINTS, 0, xAmount * zAmount * 2);

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
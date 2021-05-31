#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	uint32_t ID;

	Shader(const char* vertexPath, const char* fragmentPath);
	Shader(const char* vertexPath, const char* geometryPath, const char* fragmentPath);
	void use();


	void setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setVec2(const std::string& name, float v0, float v1) const {
		glUniform2f(glGetUniformLocation(ID, name.c_str()), v0, v1);
	}
	void setVec2(const std::string& name, glm::vec2 value) const {
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
	}
	void setVec3(const std::string& name, float v0, float v1, float v2) const {
		glUniform3f(glGetUniformLocation(ID, name.c_str()), v0, v1, v2);
	}
	void setVec3(const std::string& name, glm::vec3 value) const {
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
	}
	void setVec4(const std::string& name, float v0, float v1, float v2, float v3) const {
		glUniform4f(glGetUniformLocation(ID, name.c_str()), v0, v1, v2, v3);
	}
	void setVec4(const std::string& name, glm::vec4 value) const {
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
	}

	void setMat4(const std::string& name, glm::mat4 trans) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(trans));
	}

	/*void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec3(const std::string& name, float v0, float v1, float v2) const;
	void setVec4(const std::string& name, float v0, float v1, float v2, float v3) const;*/
private:
	void getShaderCode(const char* shaderPath, std::string* vectorCode);
	uint32_t createShader(const char* shaderPath, GLuint type);
};
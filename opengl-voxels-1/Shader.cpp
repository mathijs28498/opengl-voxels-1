#include "Shader.h"

// TODO: Make private function to get single shader
Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	std::string vShaderCodeStr;
	std::string fShaderCodeStr;
	getShaderCode(vertexPath, &vShaderCodeStr);
	getShaderCode(fragmentPath, &fShaderCodeStr);
	const char* vShaderCode = vShaderCodeStr.c_str();
	const char* fShaderCode = fShaderCodeStr.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << infoLog << '\n';
		std::cout << vShaderCode << '\n';
		throw std::runtime_error("failed to compile vertex shader!");
	}
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << infoLog << '\n';
		throw std::runtime_error("failed to compile fragment shader!");
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		throw std::runtime_error("failed to link shaders!");
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::getShaderCode(const char* shaderPath, std::string* shaderCode) {
	std::ifstream shaderFile;

	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		shaderFile.open(shaderPath);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		*shaderCode = shaderStream.str();
	} catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ " << shaderPath << '\n';
		std::cout << e.what() << '\n';
	}
}

void Shader::use() {
	glUseProgram(ID);
}



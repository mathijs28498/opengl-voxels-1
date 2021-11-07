#include "Headers/Shader.h"
#include "Headers/Global/VoxelStructs.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	uint32_t vertex = createShader(vertexPath, GL_VERTEX_SHADER);
	uint32_t fragment = createShader(fragmentPath, GL_FRAGMENT_SHADER);

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	int success;
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		throw std::runtime_error("failed to link shaders!");
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::Shader(const char* vertexPath, const char* geometryPath, const char* fragmentPath) {
	uint32_t vertex = createShader(vertexPath, GL_VERTEX_SHADER);
	uint32_t geometry = createShader(geometryPath, GL_GEOMETRY_SHADER);
	uint32_t fragment = createShader(fragmentPath, GL_FRAGMENT_SHADER);

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, geometry);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	int success;
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		throw std::runtime_error("failed to link shaders!");
	}

	glDeleteShader(vertex);
	glDeleteShader(geometry);
	glDeleteShader(fragment);
}

uint32_t Shader::createShader(const char* shaderPath, GLuint type) {
	static std::string materialStr = "Material materials";
	static std::string insertStr = getMaterialShaderString();

	uint32_t shaderID;
	int success;
	char infoLog[512];

	std::string shaderCodeStr;
	getShaderCode(shaderPath, &shaderCodeStr);
	size_t materialIndex = shaderCodeStr.find(materialStr);
	if (materialIndex != std::string::npos) {

		shaderCodeStr.insert(materialIndex + materialStr.size(), insertStr);
	}

	const char* shaderCode = shaderCodeStr.c_str();

	shaderID = glCreateShader(type);
	glShaderSource(shaderID, 1, &shaderCode, NULL);
	glCompileShader(shaderID);

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		std::cout << infoLog << '\n';
		std::cout << shaderCode << '\n';
		throw std::runtime_error("failed to compile shader!");
	}
	return shaderID;
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



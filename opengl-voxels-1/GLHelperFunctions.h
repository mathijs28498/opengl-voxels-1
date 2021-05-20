#pragma once
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

#include "VertexData.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>


uint32_t getVAO();

uint32_t loadTexture(const char* imagePath, GLenum format, bool shouldFlip);

uint32_t textureFromFile(const char* path, const std::string& directory);
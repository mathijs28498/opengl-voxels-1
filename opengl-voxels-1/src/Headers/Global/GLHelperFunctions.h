#pragma once
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

#include "VoxelStructs.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

uint32_t createVoxelVAO(Voxel* voxelPointer, size_t voxelSize);
void editVoxelVAO(Voxel* voxelPointer, size_t voxelSize, uint32_t VAO);

uint32_t getVAO();

uint32_t loadTexture(const char* imagePath, GLenum format, bool shouldFlip);

uint32_t textureFromFile(const char* path, const std::string& directory);

uint32_t getCubeVAO(glm::vec3 scale);
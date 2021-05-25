#pragma once

#include "Shader.h"

#include <iostream>


class TerrainVoxel {
public:
	TerrainVoxel(int32_t x, int32_t y, int color);
	void draw(Shader* shader) const;
private:
	int32_t x, y;
	int color;
};
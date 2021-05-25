#include "TerrainVoxel.h"

TerrainVoxel::TerrainVoxel(int32_t x, int32_t y, int color) {
    this->x = x;
    this->y = y;
    this->color = color;
}

void TerrainVoxel::draw(Shader* shader) const {

}

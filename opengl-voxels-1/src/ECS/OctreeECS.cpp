#include "../Headers/ECS/OctreeECS.h"

void OctreeSystem::start(Entity * entity) {
    OctreeComp* octreeComp = getComponentFromEntity<OctreeComp>(entity);
    TransformComp* transform = getComponentFromEntity<TransformComp>(entity);
    VoxelRendererComp* renderer = getComponentFromEntity<VoxelRendererComp>(entity);

    transform->position = {
        octreeComp->pos[0] * octreeComp->octreeSize,
        octreeComp->pos[1] * octreeComp->octreeSize,
        octreeComp->pos[2] * octreeComp->octreeSize
    };

    Octree octree(octreeComp->pos, octreeComp->octreeSize);
    octree.makeNoiseTerrain();
    octree.calculateVoxelVAO();

    octree.fillVoxelRenderer(renderer);
}

void OctreeSystem::update(Entity* entity) {

}
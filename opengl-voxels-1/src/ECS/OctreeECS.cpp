#include "../Headers/ECS/OctreeECS.h"
#include <glm/gtx/norm.hpp>

constexpr uint32_t LOD1 = 1;
constexpr uint32_t LOD2 = 2;
constexpr uint32_t LOD3 = 3;
constexpr uint32_t LOD4 = 4;

void OctreeSystem::start(Entity * entity) {
    OctreeComp* octree = getComponentFromEntity<OctreeComp>(entity);
    TransformComp* transform = getComponentFromEntity<TransformComp>(entity);
    VoxelRendererComp* renderer = getComponentFromEntity<VoxelRendererComp>(entity);
    //octree->currentLOD = 1;

    transform->position = {
        octree->octree.pos[0] * octree->octree.size,
        octree->octree.pos[1] * octree->octree.size,
        octree->octree.pos[2] * octree->octree.size
    };

    octree->octree.makeNoiseTerrain();


    //octree->octree.fillVoxelRenderer(renderer, octree->currentLOD);
}

void OctreeSystem::update(Entity* entity) {
    OctreeComp* octree = getComponentFromEntity<OctreeComp>(entity);
    TransformComp* transform = getComponentFromEntity<TransformComp>(entity);
    VoxelRendererComp* renderer = getComponentFromEntity<VoxelRendererComp>(entity);


    float distThreshold0 = octree->octree.size * VOX_SIZE / 2;
    float distThreshold1 = distThreshold0 * 3;
    float distThreshold2 = distThreshold1 * 2;
    float distSq = glm::distance2(transform->position, octree->cameraTransform->position);

    if (distSq < distThreshold0 * distThreshold0) {
        if (octree->currentLOD != LOD1) {
            octree->currentLOD = LOD1;
            octree->octree.fillVoxelRenderer(renderer, octree->currentLOD);
        }
    } else if (distSq < distThreshold1 * distThreshold1) {
        if (octree->currentLOD != LOD2) {
            octree->currentLOD = LOD2;
            octree->octree.fillVoxelRenderer(renderer, octree->currentLOD);
        }
    } else if (distSq < distThreshold2 * distThreshold2) {
        if (octree->currentLOD != LOD3) {
            octree->currentLOD = LOD3;
            octree->octree.fillVoxelRenderer(renderer, octree->currentLOD);
        }
    } else {
        if (octree->currentLOD != LOD4) {
            octree->currentLOD = LOD4;
            octree->octree.fillVoxelRenderer(renderer, octree->currentLOD);
        }

    }
}
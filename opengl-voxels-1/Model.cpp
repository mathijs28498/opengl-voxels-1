#include "Model.h"
#include "GeneralHelperFunctions.h"

#include <fstream>
#include <fstream>
#include <iterator>

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <intrin.h>

Model::Model(const char* filePath) {
    glGenVertexArrays(1, &VAO);
	loadModel(filePath);
}

void Model::draw(Shader* shader, Camera* cam, glm::mat4 model) {
    shader->use();
    cam->setUniforms(shader);
    shader->setMat4("model", model);

    float mult = 0.1f;
    static float angle = -40;
    angle += 0.05f * mult;
    if (angle > 360 - 40)
        angle -= 360;
    float realAngle = angle;

    float lightIntensity = (angle + 30) / 120;
    if (lightIntensity > 1) lightIntensity = 2 - lightIntensity;
    if (lightIntensity < 0) {
        realAngle = 270;
        lightIntensity = 0;
        angle += 0.3f * mult;
    }

    shader->setVec3("lightDir", glm::rotate(glm::vec3(-1, 0, 0), glm::radians(realAngle), glm::vec3(-1, 0, 1)));
    shader->setFloat("lightIntensity", lightIntensity);

    shader->setFloat("voxSize", VOX_SIZE); 

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, amountOfVoxels);
    glBindVertexArray(0);
}

void Model::calculateVAO(std::vector<Voxel>* voxels) {
    uint32_t VBO;
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, voxels->size() * sizeof(Voxel), voxels->data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Voxel), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Voxel), (void*)offsetof(Voxel, color));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

// TODO: Fix for multiple chunks
void Model::loadModel(const char* filePath) {
    std::ifstream input(filePath, std::ios::binary);

    std::vector<char> bytes((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));

    input.close();

    size_t bytePointer = 4;
    std::cout << "Version: "<< bytesToUint32(&bytes, &bytePointer) << '\n';
    
    if (!findWordInBytes(&bytes, &bytePointer, "SIZE", 4)) {
        throw std::runtime_error("Couldn't find 'SIZE' in vox file");
    }
    bytePointer += 4 * 2;
    uint32_t xChunkSize = bytesToUint32(&bytes, &bytePointer);
    uint32_t yChunkSize = bytesToUint32(&bytes, &bytePointer);
    uint32_t zChunkSize = bytesToUint32(&bytes, &bytePointer);

    if (!findWordInBytes(&bytes, &bytePointer, "XYZI", 4)) {
        throw std::runtime_error("couldn't find 'XYZI' in vox file");
    }

    bytePointer += 4 * 2;
    const uint32_t voxAmount = bytesToUint32(&bytes, &bytePointer);
    amountOfVoxels = voxAmount;
    std::vector<QuadUint8Struct> voxXYZIs(voxAmount);
    for (int i = 0; i < voxAmount; i++) {
        voxXYZIs[i] = QuadUint8Struct::getQuadUint8Struct(&bytes, &bytePointer);
    }

    if (!findWordInBytes(&bytes, &bytePointer, "RGBA", 4)) {
        // TODO: Add default palette if RGBA not found (also make bytePointer not go to end)
        throw std::runtime_error("Couldn't find 'RGBA' in vox file");
    }

    bytePointer += 4 * 2;
    constexpr size_t rgbaAmount = 255;
    std::vector<QuadUint8Struct> voxRGBAs(rgbaAmount);
    for (int i = 0; i < rgbaAmount; i++) {
        voxRGBAs[i] = QuadUint8Struct::getQuadUint8Struct(&bytes, &bytePointer);
    }

    std::vector<Voxel> voxels(voxAmount);
    for (size_t i = 0; i < voxAmount; i++) {
        QuadUint8Struct voxXYZI = voxXYZIs[i];
        QuadUint8Struct voxRGBA = voxRGBAs[voxXYZIs[i].d - 1];

        voxels[i] = {
            (float)voxXYZI.a, (float)voxXYZI.c, (float)voxXYZI.b,
            (float)voxRGBA.a / 255.0f, (float)voxRGBA.b / 255.0f, (float)voxRGBA.c / 255.0f,
        };
    }

    calculateVAO(&voxels);
}
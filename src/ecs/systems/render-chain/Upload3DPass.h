//
// Created by XL0002 on 2026/10/15.
//

#ifndef GAMEENGINEQ_UPLOAD3DPASS_H
#define GAMEENGINEQ_UPLOAD3DPASS_H
#include "DrawableGPUMesh.h"
#include "SDL3/SDL_gpu.h"
#include "../../Scene.h"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

struct ColorVertex3D {
    glm::vec3 pos;
    glm::vec4 color;
};
using Indice=glm::ivec3;
class Upload3DPass {
private:
    SDL_GPUDevice* device;
public:
    Upload3DPass(SDL_GPUDevice *device):device(device) {};
    void uploadData(ecs::Scene *scene, std::unordered_map<Entity, DrawableGPUMesh> &drawableMeshes);
};


#endif //GAMEENGINEQ_UPLOAD3DPASS_H

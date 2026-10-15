//
// Created by XL0002 on 2026/9/29.
//

#ifndef GAMEENGINEQ_RENDER3DPASS_H
#define GAMEENGINEQ_RENDER3DPASS_H
#include "SdlGpuRenderSystem.h"
#include "glm/glm.hpp"
#include "SDL3/SDL_gpu.h"


namespace ecs {
    class Scene;
}
struct MVP {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};
class Render3DPass {
private:
    SDL_GPUDevice* device;
    SDL_Window* window;
    SDL_GPUGraphicsPipeline* pipeline3d=nullptr;
public:
    Render3DPass(SDL_GPUDevice *device, SDL_Window *window);
    ~Render3DPass()= default;


    void draw(ecs::Scene *scene, SDL_GPUTexture *targetTexture, SDL_GPUCommandBuffer *commandBuffer, const std::unordered_map<Entity,
              DrawableGPUMesh>
              &drawableMeshes, Entity camera);
};


#endif //GAMEENGINEQ_RENDER3DPASS_H

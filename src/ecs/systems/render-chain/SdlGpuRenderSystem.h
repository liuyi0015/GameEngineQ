//
// Created by XL0002 on 2026/9/15.
//

#ifndef GAMEENGINEQ_SDLGPU_RENDERSYSTEM_H
#define GAMEENGINEQ_SDLGPU_RENDERSYSTEM_H
#include <string>
#include <unordered_map>

#include "../../System.h"
#include "../../Entity.h"
#include "SDL3/SDL_gpu.h"

class Render3DPass;
//物体在GPU的资源位置
struct DrawableGPUMesh {
    SDL_GPUBuffer* vertexBuffer;
    size_t vertexCount;
    Uint32 vertexOffset;

    SDL_GPUBuffer* indexBuffer;
    size_t indexCount;
    Uint32 indexOffset;
};
class SdlGpuRenderSystem: public ecs::System {
private:
    SDL_GPUDevice* device;
    SDL_Window *window;
    Render3DPass * render3dPass=nullptr;
    //保存场景的顶点数据布局
    std::unordered_map<Entity, DrawableGPUMesh> drawableMeshes;

    std::unordered_map<std::string, SDL_GPUTexture*> targets;
public:
    explicit SdlGpuRenderSystem();
    void start() override;
    void update(double deltaTime) override{};
    void fixed_update(double deltaTime) override{};
    void draw() override;
};

#endif //GAMEENGINEQ_SDLGPU_RENDERSYSTEM_H

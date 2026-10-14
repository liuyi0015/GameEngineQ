//
// Created by XL0002 on 2026/9/15.
//
#ifndef GAMEENGINEQ_SOFT_RENDERSYSTEM_H
#define GAMEENGINEQ_SOFT_RENDERSYSTEM_H
#include "Render2DProcess.h"
#include "Render3DProcess.h"
#include "RenderCompositorProcess.h"
#include "../../System.h"
#include "../../../renderer/soft-render/GpuSimulator.h"

class SoftRenderSystem: public ecs::System {
private:
    SoftGPU* gpu;
    RenderContext * renderContext=nullptr;
    Render2DProcess * render2dProcess=nullptr;
    RenderCompositorProcess * renderCompositorProcess=nullptr;
    Render3DProcess * render3dProcess=nullptr;
    std::unordered_map<std::string,ColorBuffer*> targets;
public:
    explicit SoftRenderSystem();
    void start() override;
    void update(double deltaTime) override{};
    void fixed_update(double deltaTime) override{};
    void draw() override;
};

#endif //GAMEENGINEQ_SOFT_RENDERSYSTEM_H

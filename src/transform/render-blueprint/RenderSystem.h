//
// Created by XL0002 on 2026/9/15.
//

#ifndef GAMEENGINEQ_RENDERSYSTEM_H
#define GAMEENGINEQ_RENDERSYSTEM_H
#include "Render2DProcess.h"
#include "RenderCompositorProcess.h"
#include "UploadProcess.h"
#include "../../core/ecs/System.h"
#include "../../core/Context.hpp"
#include "../../soft-render/GpuSimulator.h"

class RenderSystem: public ecs::System {
private:
    SoftGPU* gpu;
    UploadProcess * uploadProcess=nullptr;
    RenderContext * renderContext=nullptr;
    Render2DProcess * render2dProcess=nullptr;
    RenderCompositorProcess * renderCompositorProcess=nullptr;

public:
    explicit RenderSystem(ecs::Scene* scene);
    void start() override;
    void update(double deltaTime) override{};
    void fixed_update(double deltaTime) override{};
    void draw() override;
};

#endif //GAMEENGINEQ_RENDERSYSTEM_H

//
// Created by abc17 on 2026/8/12.
//

#ifndef GAMEENGINE_RENDERCOMPOSITOR_H
#define GAMEENGINE_RENDERCOMPOSITOR_H
#include <vector>

#include "../../core/ecs/System.h"
#include "../../soft-render/GpuSimulator.h"
#include "../../core/Context.hpp"
#include "RenderContext.h"

class RenderCompositorProcess{
private:
    ecs::Scene* scene;
    SoftGPU* gpu;
    RenderContext* renderContext;
    ColorBuffer* target;
public:
    explicit RenderCompositorProcess(ecs::Scene* scene,RenderContext* renderContext)
    : scene(scene),  renderContext(renderContext){
        gpu = ApplicationContext::getInstance().get<SoftGPU *>("mygpu");
        target=gpu->swapchain_texture;
    }

    std::vector<ColorBuffer*> srcs;

    void start() ;
    void draw();
};


#endif //GAMEENGINE_RENDERCOMPOSITOR_H

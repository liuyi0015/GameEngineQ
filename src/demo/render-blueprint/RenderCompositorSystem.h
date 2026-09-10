//
// Created by abc17 on 2026/8/12.
//

#ifndef GAMEENGINE_RENDERCOMPOSITOR_H
#define GAMEENGINE_RENDERCOMPOSITOR_H
#include <vector>

#include "../../core/ecs/System.h"
#include "../../soft-render/SoftRenderer2D.h"
#include "../../core/Context.hpp"
#include "../../soft-render/RenderContext.h"

class RenderCompositorSystem:public ecs::System{
private:
    SoftGPU* gpu;
    RenderContext* renderContext;
    ColorBuffer* target;
public:
    explicit RenderCompositorSystem(ecs::Scene* scene,RenderContext* renderContext)
    : System(scene),  renderContext(renderContext){
        gpu = ApplicationContext::getInstance().get<SoftGPU *>("mygpu");
        target=gpu->swapchain_texture;
    }

    std::vector<ColorBuffer*> srcs;

    void start() override;
    void update(double deltaTime) override{};
    void fixed_update(double deltaTime) override{};
    void draw() override;
};


#endif //GAMEENGINE_RENDERCOMPOSITOR_H

//
// Created by XL0002 on 2026/7/22.
//

#ifndef GAMEENGINE_RENDERSYSTEM_H
#define GAMEENGINE_RENDERSYSTEM_H
#include <utility>

#include "../../soft-render/GpuSimulator.h"
#include "../../core/Context.hpp"
#include "../../core/ecs/BaseComponents.h"
#include "../../core/ecs/System.h"
#include "RenderContext.h"


class Render2DProcess{
private:
    ecs::Scene* scene;
    RenderContext* renderContext;
    SoftGPU *gpu;
public:
    explicit Render2DProcess(ecs::Scene* scene,RenderContext* renderContext)
        : scene(scene), renderContext(renderContext){
        this->gpu=ApplicationContext::getInstance().get<SoftGPU*>("mygpu");
    }
    void draw() ;
};


#endif //GAMEENGINE_RENDERSYSTEM_H

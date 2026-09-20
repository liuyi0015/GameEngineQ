//
// Created by XL0002 on 2026/9/16.
//

#ifndef GAMEENGINEQ_RENDER3DPROCESS_H
#define GAMEENGINEQ_RENDER3DPROCESS_H

#include "../../../soft-render/GpuSimulator.h"
#include "../../../core/Context.hpp"
#include "../../../ecs/BaseComponents.h"
#include "../../../ecs/System.h"
#include "RenderContext.h"


class Render3DProcess {
private:
    ecs::Scene* scene;
    SoftGPU* gpu;
    RenderContext* renderContext;
    ColorBuffer* target;
    std::size_t vert_buffer_index;
    std::size_t index_buffer_index;
    std::size_t uniform_buffer_index;

public:
    explicit Render3DProcess(ecs::Scene* scene,RenderContext* renderContext,ColorBuffer* target)
    : scene(scene),  renderContext(renderContext),target(target){
        gpu = ApplicationContext::getInstance().get<SoftGPU *>("mygpu");
    }

    std::vector<ColorBuffer*> srcs;

    void registerPipelines() ;
    void initBuffers();
    void uploadData();
    void draw();
    void endFrame();
};


#endif //GAMEENGINEQ_RENDER3DPROCESS_H

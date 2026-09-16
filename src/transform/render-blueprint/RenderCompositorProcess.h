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
    std::size_t vert_buffer_index;
    std::size_t index_buffer_index;
    std::size_t uniform_buffer_index;

public:
    explicit RenderCompositorProcess(ecs::Scene* scene,RenderContext* renderContext,ColorBuffer* target)
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


#endif //GAMEENGINE_RENDERCOMPOSITOR_H

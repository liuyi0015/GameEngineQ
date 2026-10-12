//
// Created by XL0002 on 2026/7/22.
//

#ifndef GAMEENGINE_RENDERSYSTEM_H
#define GAMEENGINE_RENDERSYSTEM_H
#include <utility>

#include "../../../soft-render/GpuSimulator.h"
#include "../../../core/Context.hpp"
#include "../../../ecs/BaseComponents.h"
#include "../../../ecs/System.h"
#include "RenderContext.h"

class Render2DProcess{
private:
    RenderContext* renderContext;
    SoftGPU *gpu;
    std::size_t vert_buffer_index;
    std::size_t index_buffer_index;
    std::size_t uniform_buffer_index;

public:
    explicit Render2DProcess(RenderContext* renderContext)
        : renderContext(renderContext){
        this->gpu=ApplicationContext::getInstance().get<SoftGPU*>("mygpu");
    }
    void initBuffers();
    void registerPipelines();
    void uploadData(ecs::Scene *scene);
    void draw(ecs::Scene *scene) ;
    void endFrame();
};


#endif //GAMEENGINE_RENDERSYSTEM_H

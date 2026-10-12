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
    SoftGPU* gpu;
    RenderContext* renderContext;
    std::size_t vert_buffer_index;
    std::size_t index_buffer_index;
    std::size_t uniform_buffer_index;

public:
    explicit Render3DProcess(RenderContext* renderContext)
    : renderContext(renderContext){
        gpu = ApplicationContext::getInstance().get<SoftGPU *>("mygpu");
    }

    std::vector<ColorBuffer*> srcs;

    void registerPipelines() ;
    void initBuffers();
    void uploadData(ecs::Scene *scene);
    void draw(ecs::Scene *scene);
    void endFrame();
};


#endif //GAMEENGINEQ_RENDER3DPROCESS_H

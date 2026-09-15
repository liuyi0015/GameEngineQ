//
// Created by XL0002 on 2026/9/4.
//

#ifndef GAMEENGINEQ_CONSTRUCTANDUPLOADVERTEXSYSTEM_H
#define GAMEENGINEQ_CONSTRUCTANDUPLOADVERTEXSYSTEM_H
#include "../../core/ecs/System.h"
#include "../../soft-render/GpuSimulator.h"
#include "../../core/Context.hpp"
#include "../../core/ecs/Entity.h"
#include "RenderContext.h"

class UploadProcess{
private:
    ecs::Scene* scene;
    RenderContext* renderContext;
    SoftGPU* gpu;
public:
    explicit UploadProcess(ecs::Scene* scene,RenderContext* render_context)
        : scene(scene), renderContext(render_context){
        gpu=ApplicationContext::getInstance().get<SoftGPU*>("mygpu");
    }
    void draw();
};


#endif //GAMEENGINEQ_CONSTRUCTANDUPLOADVERTEXSYSTEM_H

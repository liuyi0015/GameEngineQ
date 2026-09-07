//
// Created by XL0002 on 2026/9/4.
//

#ifndef GAMEENGINEQ_CONSTRUCTANDUPLOADVERTEXSYSTEM_H
#define GAMEENGINEQ_CONSTRUCTANDUPLOADVERTEXSYSTEM_H
#include "../../core/ecs/System.h"
#include "../../soft-render/SoftRenderer2D.h"
#include "../../core/Context.hpp"
#include "../../core/ecs/Entity.h"
#include "../../soft-render/RenderContext.h"

class UploadSystem:public ecs::System{
private:
    RenderContext* renderContext;
    SoftGPU* gpu;
public:
    explicit UploadSystem(ecs::Scene* scene,RenderContext* render_context)
        : System(scene),renderContext(render_context){
        gpu=ApplicationContext::getInstance().get<SoftGPU*>("mygpu");
    }
    void start() override{};
    void update(double deltaTime) override{};
    void fixed_update(double deltaTime) override{};
    void draw() override;
};


#endif //GAMEENGINEQ_CONSTRUCTANDUPLOADVERTEXSYSTEM_H

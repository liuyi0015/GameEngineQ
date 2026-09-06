//
// Created by XL0002 on 2026/7/22.
//

#ifndef GAMEENGINE_RENDERSYSTEM_H
#define GAMEENGINE_RENDERSYSTEM_H
#include "../../soft-render/SoftRenderer2D.h"
#include "../../core/Context.hpp"
#include "../../core/ecs/BaseComponents.h"
#include "../../core/ecs/System.h"
#include "../../soft-render/RenderContext.h"


class Render2DSystem: public ecs::System {
private:
    SoftGPU *gpu;
    RenderContext* renderContext;
    ColorBuffer* target;
    Entity camera;
public:
    explicit Render2DSystem(ecs::Scene* scene,int w,int h,Entity camera,RenderContext* renderContext)
        : System(scene),camera(camera),renderContext(renderContext){
        this->target=new ColorBuffer( w, h);
        this->gpu=ApplicationContext::getInstance().get<SoftGPU*>("mygpu");
    }
    void start() override;
    void update(double deltaTime) override{};
    void fixed_update(double deltaTime) override{};
    void draw() override;
};


#endif //GAMEENGINE_RENDERSYSTEM_H

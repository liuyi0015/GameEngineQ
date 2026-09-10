//
// Created by XL0002 on 2026/7/22.
//

#ifndef GAMEENGINE_RENDERSYSTEM_H
#define GAMEENGINE_RENDERSYSTEM_H
#include <utility>

#include "../../soft-render/SoftRenderer2D.h"
#include "../../core/Context.hpp"
#include "../../core/ecs/BaseComponents.h"
#include "../../core/ecs/System.h"
#include "../../soft-render/RenderContext.h"


class Render2DSystem: public ecs::System {
private:
    SoftGPU *gpu;
    RenderContext* renderContext;
    Entity camera;
public:
    std::string targetName;
    ColorBuffer* target;
    explicit Render2DSystem(ecs::Scene* scene,ColorBuffer* target,Entity camera,RenderContext* renderContext,std::string  targetName)
        : System(scene),target(target),camera(camera),renderContext(renderContext),targetName(std::move(targetName)){
        this->gpu=ApplicationContext::getInstance().get<SoftGPU*>("mygpu");
    }
    void start() override;
    void update(double deltaTime) override{};
    void fixed_update(double deltaTime) override{};
    void draw() override;
};


#endif //GAMEENGINE_RENDERSYSTEM_H

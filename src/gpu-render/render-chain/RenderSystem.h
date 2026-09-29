//
// Created by XL0002 on 2026/9/15.
//

#ifndef GAMEENGINEQ_RENDERSYSTEM_H
#define GAMEENGINEQ_RENDERSYSTEM_H
#include "Render3DPass.h"
#include "../../ecs/System.h"

class RenderSystem: public ecs::System {
private:
    Render3DPass * render3dProcess=nullptr;

public:
    explicit RenderSystem();
    void start() override;
    void update(double deltaTime) override{};
    void fixed_update(double deltaTime) override{};
    void draw() override;
};

#endif //GAMEENGINEQ_RENDERSYSTEM_H

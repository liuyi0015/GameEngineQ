//
// Created by XL0002 on 2026/7/22.
//

#ifndef GAMEENGINE_RENDERSYSTEM_H
#define GAMEENGINE_RENDERSYSTEM_H
#include "../../Context.hpp"
#include "../../RenderCompositor.h"
#include "../../ecs/BaseComponents.h"
#include "../../ecs/System.h"
#include "SDL3/SDL_render.h"


class Render2dSystem: public ecs::System {
private:
    SDL_Renderer* renderer;
    Entity camera;
    SDL_Texture* target;
public:
    explicit Render2dSystem(ecs::Scene* scene,SDL_Texture* target, const Entity camera)
        : System(scene),camera(camera){
        renderer=ApplicationContext::getInstance().get<SDL_Renderer*>("renderer");
        this->target=target;
    }

    ~Render2dSystem();

    void start() override;
    void update(double deltaTime) override{};
    void fixed_update(double deltaTime) override{};
    void draw() override;

};


#endif //GAMEENGINE_RENDERSYSTEM_H

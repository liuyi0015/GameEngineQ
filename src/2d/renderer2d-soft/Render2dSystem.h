//
// Created by XL0002 on 2026/7/22.
//

#ifndef GAMEENGINE_RENDERSYSTEM_H
#define GAMEENGINE_RENDERSYSTEM_H
#include "../../ecs/BaseComponents.h"
#include "../../ecs/System.h"
#include "SDL3/SDL_render.h"


class Render2dSystem: public ecs::System {
private:
    SDL_Renderer* renderer;
    Entity camera;
public:
    explicit Render2dSystem(Scene* scene,SDL_Renderer* renderer, const Entity camera)
        : System(scene),renderer(renderer),camera(camera){
    }
    void start() override;
    void update(double deltaTime) override{};
    void fixed_update(double deltaTime) override{};
    void draw() override;

};


#endif //GAMEENGINE_RENDERSYSTEM_H

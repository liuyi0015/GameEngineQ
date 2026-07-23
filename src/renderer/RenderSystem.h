//
// Created by XL0002 on 2026/7/22.
//

#ifndef GAMEENGINE_RENDERSYSTEM_H
#define GAMEENGINE_RENDERSYSTEM_H
#include "../ecs/System.h"
#include "SDL3/SDL_render.h"


class RenderSystem: public ecs::System {
private:
    SDL_Renderer* renderer;
public:
    explicit RenderSystem(const std::shared_ptr<Scene> &scene,SDL_Renderer* renderer)
        : System(scene),renderer(renderer){
    }
    void start() override;
    void update(double deltaTime) override{};
    void fixed_update(double deltaTime) override{};
    void draw() override;

};


#endif //GAMEENGINE_RENDERSYSTEM_H

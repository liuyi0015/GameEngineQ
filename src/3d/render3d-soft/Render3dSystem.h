//
// Created by XL0002 on 2026/7/30.
//

#ifndef GAMEENGINE_RENDERSYSTEM_H
#define GAMEENGINE_RENDERSYSTEM_H
#include <memory>

#include "MyRenderer3D.h"
#include "../../ecs/Scene.h"

class Render3dSystem: public ecs::System {
private:
    SDL_Renderer* renderer;
    Entity camera;
public:
    explicit Render3dSystem(Scene* scene,SDL_Renderer* renderer, const Entity camera)
        : System(scene),renderer(renderer),camera(camera){
    }
    void start() override{};
    void update(double deltaTime) override{};
    void fixed_update(double deltaTime) override{};
    void draw() override;

};

#endif //GAMEENGINE_RENDERSYSTEM_H

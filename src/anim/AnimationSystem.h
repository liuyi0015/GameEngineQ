//
// Created by abc17 on 2026/7/23.
//

#ifndef GAMEENGINE_ANIMATOR_H
#define GAMEENGINE_ANIMATOR_H
#include "AnimationComponents.h"
#include "../ecs/MonoBehaviourSystem.h"
#include "SDL3/SDL_render.h"


class AnimationSystem:public ecs::MonoBehaviourSystem<FrameAnimatorFlag>{
protected:
    SDL_Renderer *renderer;
public:
    explicit AnimationSystem(const std::shared_ptr<Scene>& scene,SDL_Renderer* renderer)
        : ecs::MonoBehaviourSystem<FrameAnimatorFlag>(scene) , renderer(renderer){

    }
    void onStart() override;
    void onUpdate(double deltaTime) override;
    void onFixedUpdate(double deltaTime) override{};
    void onDraw() const override{};
};


#endif //GAMEENGINE_ANIMATOR_H

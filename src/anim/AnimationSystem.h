//
// Created by abc17 on 2026/7/23.
//

#ifndef GAMEENGINE_ANIMATOR_H
#define GAMEENGINE_ANIMATOR_H
#include "AnimationComponents.h"
#include "../core/ecs/MonoBehaviourSystem.h"
#include "SDL3/SDL_render.h"


class AnimationSystem:public ecs::MonoBehaviourSystem<FrameAnimatorFlag>{
protected:
public:
    explicit AnimationSystem(ecs::Scene* scene);

    ~AnimationSystem();

    void onStart() override;
    void onUpdate(double deltaTime) override;
    void onFixedUpdate(double deltaTime) override{};
    void onDraw() const override{};
};


#endif //GAMEENGINE_ANIMATOR_H

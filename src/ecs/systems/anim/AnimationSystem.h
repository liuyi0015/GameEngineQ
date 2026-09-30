//
// Created by abc17 on 2026/7/23.
//

#ifndef GAMEENGINE_ANIMATOR_H
#define GAMEENGINE_ANIMATOR_H
#include "AnimationClip.h"
#include "AnimationComponents.h"
#include "../../MonoBehaviourSystem.h"
#include "SDL3/SDL_render.h"

//这里是关键帧动画，无关键帧的可以直接写函数，不需要动画系统
class AnimationSystem:public ecs::MonoBehaviourSystem<AnimationFlag>{
protected:
public:
    void onStart() override;
    void onEnd() override;
    void onUpdate(double deltaTime) override;
    void onFixedUpdate(double deltaTime) override{};
    void onDraw() const override{};
};


#endif //GAMEENGINE_ANIMATOR_H

//
// Created by abc17 on 2026/7/23.
//

#include "AnimationSystem.h"

#include <cassert>

#include "../render-blueprint/RenderComponents.h"


void AnimationSystem::onStart() {

}

void AnimationSystem::onEnd() {
}

void AnimationSystem::onUpdate(double deltaTime) {
    auto* animationFlag=ecs::getComponent<AnimationFlag>(scene,curEntity);
    auto* drawableFlag=ecs::getComponent<Drawable2DFlag>(scene,curEntity);
    int index=-1;
    //模拟状态机
    if (true) {
        index=0;
    }else {
        index=1;
    }
    animationFlag->anims[index].update(deltaTime);
}

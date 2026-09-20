//
// Created by XL0002 on 2026/9/17.
//

#include "FrameAnimation.h"
#include "../render-blueprint/RenderComponents.h"
#include "../../../core/ResourceManager.hpp"
#include "AnimationComponents.h"

struct Drawable2DFlag;

FrameAnimationFlag FrameAnimation::update(double dt) {
    frameAnimatorFlag.timer+=dt;
    // std::cout<<frameAnimatorFlag.timer<<"+"<<deltaTime<<std::endl;
    auto animPtr = ResourceManager::getInstance().getAnimationCache().get(frameAnimatorFlag.animId);
    while (frameAnimatorFlag.timer*1000>=animPtr->delays[frameAnimatorFlag.counter]&&frameAnimatorFlag.counter<animPtr->count) {
        frameAnimatorFlag.timer-=animPtr->delays[frameAnimatorFlag.counter]*1.0/1000;
        ResourceManager::getInstance().getSurfaceCache().set(drawableFlag.material.texResourceId,animPtr->frames[frameAnimatorFlag.counter]);
        frameAnimatorFlag.counter++;
        if (frameAnimatorFlag.counter>=animPtr->count&&frameAnimatorFlag.loop) {
            frameAnimatorFlag.counter=0;
        }
    }
    return frameAnimatorFlag;
}

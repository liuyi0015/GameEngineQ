//
// Created by abc17 on 2026/7/23.
//

#include "AnimationSystem.h"

#include <cassert>

#include "../core/ResourceManager.hpp"
#include "../demo/render-blueprint/Render2DComponents.h"
#include "SDL3_image/SDL_image.h"
AnimationSystem::~AnimationSystem() {
    for (auto entity:ecs::getEntities<FrameAnimatorFlag>(scene)) {
        auto frameAnimatorFlag=ecs::getComponent<FrameAnimatorFlag>(scene,entity).value();
        auto anim=ResourceManager::getInstance().getAnimationCache().get(frameAnimatorFlag.name);
        IMG_FreeAnimation(anim);
        ResourceManager::getInstance().getAnimationCache().erase(frameAnimatorFlag.name);
    }
}
void AnimationSystem::onStart() {
    auto frameAnimatorFlag=ecs::getComponent<FrameAnimatorFlag>(scene,curEntity).value();
    auto anim=IMG_LoadAnimation(frameAnimatorFlag.path.c_str());
    ResourceManager::getInstance().getAnimationCache().set(frameAnimatorFlag.name, anim);
    ecs::setComponent<FrameAnimatorFlag>(scene,curEntity,frameAnimatorFlag);
}

void AnimationSystem::onUpdate(double deltaTime) {
    auto frameAnimatorFlag=ecs::getComponent<FrameAnimatorFlag>(scene,curEntity).value();
    assert(ecs::getComponent<Drawable2DFlag>(scene,curEntity).has_value());
    auto drawableFlag=ecs::getComponent<Drawable2DFlag>(scene,curEntity).value();
    frameAnimatorFlag.timer+=deltaTime;
    // std::cout<<frameAnimatorFlag.timer<<"+"<<deltaTime<<std::endl;
    auto animPtr = ResourceManager::getInstance().getAnimationCache().get(frameAnimatorFlag.name);
    while (frameAnimatorFlag.timer*1000>=animPtr->delays[frameAnimatorFlag.counter]&&frameAnimatorFlag.counter<animPtr->count) {
        frameAnimatorFlag.timer-=animPtr->delays[frameAnimatorFlag.counter]*1.0/1000;
        ResourceManager::getInstance().getSurfaceCache().set(drawableFlag.material.texResourceId,animPtr->frames[frameAnimatorFlag.counter]);
        frameAnimatorFlag.counter++;
        if (frameAnimatorFlag.counter>=animPtr->count&&frameAnimatorFlag.loop) {
            frameAnimatorFlag.counter=0;
        }
    }
    ecs::setComponent<FrameAnimatorFlag>(scene,curEntity,frameAnimatorFlag);
}

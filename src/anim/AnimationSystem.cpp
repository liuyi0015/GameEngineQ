//
// Created by abc17 on 2026/7/23.
//

#include "AnimationSystem.h"

#include <cassert>

#include "../ResourceManager.hpp"
#include "../2d/renderer2d-soft/RenderComponents.h"
#include "SDL3_image/SDL_image.h"

void AnimationSystem::onStart() {
    auto frameAnimatorFlag=ecs::getComponent<FrameAnimatorFlag>(scene,curEntity).value();
    auto anim=IMG_LoadAnimation(frameAnimatorFlag.path.c_str());
    ResourceManager::getInstance().getAnimationCache().set(frameAnimatorFlag.name, anim);
}

void AnimationSystem::onUpdate(double deltaTime) {
    auto frameAnimatorFlag=ecs::getComponent<FrameAnimatorFlag>(scene,curEntity).value();
    if (!ecs::getComponent<ImageRendererFlag>(scene,curEntity).has_value()) {
        return;
    }
    auto imageRenderFlag=ecs::getComponent<ImageRendererFlag>(scene,curEntity).value();
    frameAnimatorFlag.timer+=deltaTime;
    // std::cout<<frameAnimatorFlag.timer<<"+"<<deltaTime<<std::endl;
    auto animPtr = ResourceManager::getInstance().getAnimationCache().get(frameAnimatorFlag.name);
    while (frameAnimatorFlag.timer*1000>=animPtr->delays[frameAnimatorFlag.counter]&&frameAnimatorFlag.counter<animPtr->count) {
        frameAnimatorFlag.timer-=animPtr->delays[frameAnimatorFlag.counter]*1.0/1000;
        SDL_Texture* texture=SDL_CreateTextureFromSurface(renderer,animPtr->frames[frameAnimatorFlag.counter]);
        ResourceManager::getInstance().getTextureCache().set(imageRenderFlag.textureId,texture);
        frameAnimatorFlag.counter++;
        if (frameAnimatorFlag.counter>=animPtr->count&&frameAnimatorFlag.loop) {
            frameAnimatorFlag.counter=0;
        }
    }
    ecs::setComponent<FrameAnimatorFlag>(scene,curEntity,frameAnimatorFlag);
}

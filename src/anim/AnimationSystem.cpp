//
// Created by abc17 on 2026/7/23.
//

#include "AnimationSystem.h"

#include <cassert>

#include "../ResourceManager.hpp"
#include "../2d/renderer2d/RenderComponents.h"
#include "SDL3_image/SDL_image.h"
AnimationSystem::~AnimationSystem() {
    for (auto entity:ecs::getEntities<FrameAnimatorFlag>(scene)) {
        auto imageRenderFlag=ecs::getComponent<ImageRendererFlag>(scene,entity).value();
        auto texture=ResourceManager::getInstance().getTextureCache().get(imageRenderFlag.texResourceId);
        SDL_DestroyTexture(texture);

        ResourceManager::getInstance().getTextureCache().erase(imageRenderFlag.texResourceId);
        auto frameAnimatorFlag=ecs::getComponent<FrameAnimatorFlag>(scene,entity).value();
        for (auto tex:frameAnimatorFlag.textures) {
            SDL_DestroyTexture(tex);
        }
        auto anim=ResourceManager::getInstance().getAnimationCache().get(frameAnimatorFlag.name);
        IMG_FreeAnimation(anim);
        ResourceManager::getInstance().getAnimationCache().erase(frameAnimatorFlag.name);

    }
}
void AnimationSystem::onStart() {
    auto frameAnimatorFlag=ecs::getComponent<FrameAnimatorFlag>(scene,curEntity).value();
    auto anim=IMG_LoadAnimation(frameAnimatorFlag.path.c_str());
    ResourceManager::getInstance().getAnimationCache().set(frameAnimatorFlag.name, anim);
    auto imageRendererComp = ecs::getComponent<ImageRendererFlag>(scene, curEntity).value();
    std::cout<<"动画占位texture "<<imageRendererComp.texResourceId<<std::endl;
    for (int i=0;i<anim->count;i++) {
        SDL_Texture* texture=SDL_CreateTextureFromSurface(renderer,anim->frames[i]);
        frameAnimatorFlag.textures.push_back(texture);
    }
    ecs::setComponent<FrameAnimatorFlag>(scene,curEntity,frameAnimatorFlag);
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
        ResourceManager::getInstance().getTextureCache().set(imageRenderFlag.texResourceId,frameAnimatorFlag.textures[frameAnimatorFlag.counter]);
        frameAnimatorFlag.counter++;
        if (frameAnimatorFlag.counter>=animPtr->count&&frameAnimatorFlag.loop) {
            frameAnimatorFlag.counter=0;
        }
    }
    ecs::setComponent<FrameAnimatorFlag>(scene,curEntity,frameAnimatorFlag);
}

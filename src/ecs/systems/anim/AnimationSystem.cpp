//
// Created by abc17 on 2026/7/23.
//

#include "AnimationSystem.h"

#include <cassert>

#include "FrameAnimation.h"
#include "../render-blueprint/RenderComponents.h"
#include "SDL3_image/SDL_image.h"

AnimationSystem::AnimationSystem(ecs::Scene *scene)
        : ecs::MonoBehaviourSystem<FrameAnimationFlag>(scene){
    // auto frameAnimatorFlag=ecs::getComponent<FrameAnimatorFlag>(scene,curEntity).value();
    // auto anim=IMG_LoadAnimation(frameAnimatorFlag.path.c_str());
    // ResourceManager::getInstance().getAnimationCache().set(frameAnimatorFlag.name, anim);
    // ecs::setComponent<FrameAnimatorFlag>(scene,curEntity,frameAnimatorFlag);
}

AnimationSystem::~AnimationSystem() {
    // for (auto entity:ecs::getEntities<FrameAnimatorFlag>(scene)) {
    //     auto frameAnimatorFlag=ecs::getComponent<FrameAnimatorFlag>(scene,entity).value();
    //     auto anim=ResourceManager::getInstance().getAnimationCache().get(frameAnimatorFlag.name);
    //     IMG_FreeAnimation(anim);
    //     ResourceManager::getInstance().getAnimationCache().erase(frameAnimatorFlag.name);
    // }
}

void AnimationSystem::onStart() {

}

void AnimationSystem::onUpdate(double deltaTime) {
    if (ecs::getComponent<FrameAnimationFlag>(scene,curEntity).has_value()){

        auto frameAnimatorFlag=ecs::getComponent<FrameAnimationFlag>(scene,curEntity).value();
        if (ecs::getComponent<Drawable2DFlag>(scene,curEntity).has_value()) {
            auto drawableFlag=ecs::getComponent<Drawable2DFlag>(scene,curEntity).value();
            // auto newFrameAnimationFlag=FrameAnimation::update(deltaTime,frameAnimatorFlag,drawableFlag);
            // ecs::setComponent<FrameAnimationFlag>(scene,curEntity,newFrameAnimationFlag);

        }
    }
}

//
// Created by XL0002 on 2026/8/3.
//

#include "ChangeTransformSystem.h"

#include <cassert>

#include "GameComponents.h"
#include "../../transform/transform2d/Transform2DComponents.h"
#include "../../ecs/util/TransformSceneUtil.h"
void ChangeTransformSystem::onFixedUpdate(double deltaTime) {
    assert(ecs::getComponent<Transform2DComp>(scene,curEntity)!=nullptr);
    if ( ecs::getComponent<RotationFlag>(scene, curEntity)) {
        rotate(deltaTime);
    }
    if ( ecs::getComponent<ScalerFlag>(scene, curEntity)) {
        scale(deltaTime);
    }
    if ( ecs::getComponent<MoveFlag>(scene, curEntity)) {
        move(deltaTime);
    }
}
void ChangeTransformSystem::rotate(double deltaTime) {
    auto* transformComp = ecs::getComponent<Transform2DComp>(scene, curEntity);
    auto* rotationComp=ecs::getComponent<RotationFlag>(scene, curEntity);
    if (rotationComp==nullptr) {
        return;
    }
    transformComp->transform.rotation+= rotationComp->speed * static_cast<float>(deltaTime);
}

void ChangeTransformSystem::scale(double deltaTime) {
    auto* transformComp = ecs::getComponent<Transform2DComp>(scene, curEntity);
    auto* scalerComp=ecs::getComponent<ScalerFlag>(scene, curEntity);
    auto scale=transformComp->transform.scale;
    scale.x+= scalerComp->speed * static_cast<float>(deltaTime);
    scale.y+= scalerComp->speed * static_cast<float>(deltaTime);
    transformComp->transform.scale=scale;

}
void ChangeTransformSystem::move(double deltaTime) {

    auto* moveFlagComp=ecs::getComponent<MoveFlag>(scene, curEntity);
    auto* transformComp = ecs::getComponent<Transform2DComp>(scene, curEntity);
    transformComp->transform.position.x+=moveFlagComp->speed * static_cast<float>(deltaTime);
}

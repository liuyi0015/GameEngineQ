//
// Created by XL0002 on 2026/8/3.
//

#include "ChangeTransformSystem.h"

#include <cassert>

#include "GameComponents.h"
#include "../../transform/transform2d/Transform2DComponents.h"
#include "../../ecs/util/TransformSceneUtil.h"
void ChangeTransformSystem::onFixedUpdate(double deltaTime) {
    if (ecs::getComponent<Transform2DComp>(scene, curEntity).has_value() && ecs::getComponent<RotationFlag>(scene, curEntity).has_value()) {
        rotate(deltaTime);
    }
    if (ecs::getComponent<Transform2DComp>(scene, curEntity).has_value() && ecs::getComponent<ScalerFlag>(scene, curEntity).has_value()) {
        scale(deltaTime);
    }
    if (ecs::getComponent<Transform2DComp>(scene, curEntity).has_value() && ecs::getComponent<MoveFlag>(scene, curEntity).has_value()) {
        move(deltaTime);
    }
}
void ChangeTransformSystem::rotate(double deltaTime) {
    auto transform = ecs::getComponent<Transform2DComp>(scene, curEntity).value();
    auto rotation=ecs::getComponent<RotationFlag>(scene, curEntity).value();

    transform.transform.rotation+= rotation.speed * static_cast<float>(deltaTime);
    ecs::setComponent<Transform2DComp>(scene, curEntity,transform);
}

void ChangeTransformSystem::scale(double deltaTime) {
    auto transformComp = ecs::getComponent<Transform2DComp>(scene, curEntity);
    auto scalerComp=ecs::getComponent<ScalerFlag>(scene, curEntity);
    auto scale=transformComp.value().transform.scale;
    auto speed=scalerComp.value().speed;
    scale.x+= speed * static_cast<float>(deltaTime);
    scale.y+= speed * static_cast<float>(deltaTime);
    ecs::setComponent<Transform2DComp>(scene, curEntity,transformComp.value());
}
void ChangeTransformSystem::move(double deltaTime) {

    auto moveFlagComp=ecs::getComponent<MoveFlag>(scene, curEntity);
    auto transformComp = ecs::getComponent<Transform2DComp>(scene, curEntity);
    glm::vec2& pos = transformComp.value().transform.position;

    switch (moveFlagComp.value().currentEdge) {
        case 0:
            pos.x += moveFlagComp.value().speed * deltaTime;
            if (pos.x >= moveFlagComp.value().x + moveFlagComp.value().width) {
                pos.x = moveFlagComp.value().x + moveFlagComp.value().width;
                moveFlagComp.value().currentEdge = 1;
            }
            break;
        case 1:
            pos.y += moveFlagComp.value().speed * deltaTime;
            if (pos.y >= moveFlagComp.value().y + moveFlagComp.value().height) {
                pos.y = moveFlagComp.value().y + moveFlagComp.value().height;
                moveFlagComp.value().currentEdge = 2;
            }
            break;
        case 2:
            pos.x -= moveFlagComp.value().speed * deltaTime;
            if (pos.x <=moveFlagComp.value().x) {
                pos.x = moveFlagComp.value().x;
                moveFlagComp.value().currentEdge = 3;
            }
            break;
        case 3:
            pos.y -= moveFlagComp.value().speed * deltaTime;
            if (pos.y <= moveFlagComp.value().y) {
                pos.y = moveFlagComp.value().y;
                moveFlagComp.value().currentEdge = 0;
            }
            break;
        default: ;
    }
    ecs::setComponent<Transform2DComp>(scene, curEntity, transformComp.value());
    ecs::setComponent<MoveFlag>(scene, curEntity, moveFlagComp.value());
}

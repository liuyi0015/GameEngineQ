//
// Created by XL0002 on 2026/7/16.
//

#include "MoveSystem.h"

#include <cassert>
#include "../2d/transform2d/Transform2dComponents.h"
#include "GameComponents.h"

void MoveSystem::onFixedUpdate(double deltaTime) {

    auto moveFlagComp=ecs::getComponent<MoveFlag>(scene, curEntity);
    auto transformComp = ecs::getComponent<TransformComp>(scene, curEntity);
    assert(moveFlagComp.has_value() && transformComp.has_value());
    Position& pos = transformComp.value().transform.position;

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
    ecs::setComponent<TransformComp>(scene, curEntity, transformComp.value());
    ecs::setComponent<MoveFlag>(scene, curEntity, moveFlagComp.value());
}

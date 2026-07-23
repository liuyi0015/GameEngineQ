//
// Created by XL0002 on 2026/7/16.
//

#include "MoveSystem.h"

#include <cassert>

#include "GameComponents.h"

void MoveSystem::onFixedUpdate(float deltaTime) {

    auto moveFlagComp=scene->getComponent<MoveFlag>(curEntity);
    auto transformComp = scene->getComponent<ecs::Transform>(curEntity);
    assert(moveFlagComp.has_value() && transformComp.has_value());
    ecs::Position& pos = transformComp.value().position;

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
    scene->setComponent<ecs::Transform>(curEntity, transformComp.value());
    scene->setComponent<MoveFlag>(curEntity, moveFlagComp.value());
}

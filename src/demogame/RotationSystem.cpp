//
// Created by XL0002 on 2026/8/3.
//

#include "RotationSystem.h"

#include <cassert>

#include "GameComponents.h"
#include "../EventDispatcher.h"
#include "../2d/transform2d/Transform2dComponents.h"

void RotationSystem::onFixedUpdate(double deltaTime) {
    assert(ecs::getComponent<Transform>(scene, curEntity).has_value() && ecs::getComponent<RotationFlag>(scene, curEntity).has_value());
    auto transform = ecs::getComponent<Transform>(scene, curEntity).value();
    auto rotation=ecs::getComponent<RotationFlag>(scene, curEntity).value();

    transform.rotation.angle+= rotation.speed * static_cast<float>(deltaTime);
    ecs::setComponent<Transform>(scene, curEntity,{transform.position,
        transform.rotation,transform.scale,transform.parent});
}

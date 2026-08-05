//
// Created by XL0002 on 2026/8/3.
//

#include "RotationSystem.h"

#include <cassert>

#include "GameComponents.h"
#include "../EventDispatcher.h"
#include "../2d/transform2d/Transform2dComponents.h"

void RotationSystem::onFixedUpdate(double deltaTime) {
    assert(ecs::getComponent<TransformComp>(scene, curEntity).has_value() && ecs::getComponent<RotationFlag>(scene, curEntity).has_value());
    auto transform = ecs::getComponent<TransformComp>(scene, curEntity).value();
    auto rotation=ecs::getComponent<RotationFlag>(scene, curEntity).value();

    transform.transform.rotation.angle+= rotation.speed * static_cast<float>(deltaTime);
    ecs::setComponent<TransformComp>(scene, curEntity,transform);
}

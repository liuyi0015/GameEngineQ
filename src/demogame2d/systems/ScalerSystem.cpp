//
// Created by XL0002 on 2026/7/28.
//

#include "ScalerSystem.h"

#include <cassert>

#include "../GameComponents.h"
#include "../../EventDispatcher.h"
#include "../../2d/transform2d/Transform2dComponents.h"

void ScalerSystem::onFixedUpdate(double deltaTime) {
    auto transformComp = ecs::getComponent<TransformComp>(scene, curEntity);
    auto scalerComp=ecs::getComponent<ScalerFlag>(scene, curEntity);

    assert(scalerComp.has_value() && transformComp.has_value());
    auto scale=transformComp.value().transform.scale;
    auto speed=scalerComp.value().speed;
    scale.x+= speed * static_cast<float>(deltaTime);
    scale.y+= speed * static_cast<float>(deltaTime);
    ecs::setComponent<TransformComp>(scene, curEntity,transformComp.value());
}

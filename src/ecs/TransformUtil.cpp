//
// Created by XL0002 on 2026/7/21.
//

#include "TransformUtil.h"

ecs::Transform TransformUtil::computeWorldTransform(const ecs::Transform &localTransform,const std::shared_ptr<Scene>& scene) {
    ecs::Transform worldTransform;
    if (!localTransform.parent.has_value()) {
        return localTransform;
    }

    auto parentEntity = localTransform.parent.value();
    auto parentTransformOpt = scene->getComponent<ecs::Transform>(parentEntity);
    if (!parentTransformOpt.has_value()) {
        return localTransform;
    }
    // 递归计算世界坐标
    ecs::Transform parentWorldTransform = computeWorldTransform(parentTransformOpt.value(),scene);
    worldTransform.position.x = parentWorldTransform.position.x + localTransform.position.x;
    worldTransform.position.y = parentWorldTransform.position.y + localTransform.position.y;
    worldTransform.rotation.angle = parentWorldTransform.rotation.angle + localTransform.rotation.angle;
    worldTransform.scale.x = parentWorldTransform.scale.x * localTransform.scale.x;
    worldTransform.scale.y = parentWorldTransform.scale.y * localTransform.scale.y;
    return worldTransform;
}

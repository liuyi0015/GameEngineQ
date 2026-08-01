//
// Created by XL0002 on 2026/7/21.
//

#include "TransformUtil.h"
#include "Transform2dComponents.h"
#include "../../ecs/Util.h"

Transform TransformUtil::computeLocalToWorldTransform(const Transform &localTransform,const std::shared_ptr<Scene>& scene) {
    Transform worldTransform;
    if (!localTransform.parent.has_value()) {
        return localTransform;
    }

    auto parentEntity = localTransform.parent.value();
    auto parentTransformOpt = ecs::getComponent<Transform>(scene, parentEntity);
    if (!parentTransformOpt.has_value()) {
        return localTransform;
    }
    // 递归计算世界坐标
    Transform parentWorldTransform = computeLocalToWorldTransform(parentTransformOpt.value(),scene);
    worldTransform.position.x = parentWorldTransform.position.x + localTransform.position.x;
    worldTransform.position.y = parentWorldTransform.position.y + localTransform.position.y;
    worldTransform.rotation.angle = parentWorldTransform.rotation.angle + localTransform.rotation.angle;
    worldTransform.scale.x = parentWorldTransform.scale.x * localTransform.scale.x;
    worldTransform.scale.y = parentWorldTransform.scale.y * localTransform.scale.y;
    return worldTransform;
}

Transform TransformUtil::computeWorldToViewTransform(const Transform &worldTransform,const std::shared_ptr<Scene> &scene,const Entity camera) {
    Transform viewTransform;
    Transform cameraTransform=ecs::getComponent<Transform>(scene, camera).value();
    viewTransform.position=worldTransform.position-cameraTransform.position;
    viewTransform.rotation.angle=worldTransform.rotation.angle-cameraTransform.rotation.angle;
    viewTransform.scale=worldTransform.scale/cameraTransform.scale;
    return viewTransform;
}

std::vector<Entity> TransformUtil::getChildEntities(const Entity e1,
                                                    const std::shared_ptr<Scene> &scene) {
    std::vector<Entity> children;
    for (auto entity : ecs::getEntities<Transform>(scene)) {
        if (ecs::getComponent<Transform>(scene, entity).value().parent==e1) {
            children.push_back(entity);
        }
    }
    return children;
}

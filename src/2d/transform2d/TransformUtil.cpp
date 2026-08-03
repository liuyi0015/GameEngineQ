//
// Created by XL0002 on 2026/7/21.
//

#include "TransformUtil.h"
#include "Transform2dComponents.h"
#include "../../ecs/Util.h"
#include "glm/glm.hpp"

glm::mat3 TransformUtil::transformToMatrix(const Transform &transform) {
    float sx=transform.scale.x;
    float sy=transform.scale.y;
    glm::mat3 matScale=glm::mat3(
        sx,0,0,
        0,sy,0,
        0,0,1
    );
    float c=glm::cos(glm::radians(transform.rotation.angle));
    float s=glm::sin(glm::radians(transform.rotation.angle));
    glm::mat3 matRotate=glm::mat3(
        c, s, 0,
        -s, c, 0,
        0, 0, 1
    );
    float px=transform.position.x;
    float py=transform.position.y;
    glm::mat3 matTranslate=glm::mat3(
        1,0,0,
        0,1,0,
        px,py,1
        );
    return matTranslate*matRotate*matScale;
}
Transform TransformUtil::getReverseTransform(const Transform &transform) {
    Transform reverseTransform;
    reverseTransform.scale.x=1.0f/transform.scale.x;
    reverseTransform.scale.y=1.0f/transform.scale.y;
    reverseTransform.rotation.angle=-transform.rotation.angle;
    reverseTransform.position.x=-transform.position.x;
    reverseTransform.position.y=-transform.position.y;
    return reverseTransform;
}

Transform  TransformUtil::computeWorldToLocalTransform(const Transform &worldTransform,const Transform &parentWorldTransform,const std::shared_ptr<Scene>& scene) {
    // 递归计算世界坐标
    glm::mat3 parentWorldMatrix=transformToMatrix(parentWorldTransform);
    glm::mat3 worldMatrix=transformToMatrix(worldTransform);
    glm::mat3 localMatrix=parentWorldMatrix*worldMatrix;
    Transform localTransform;
    localTransform.position.x=localMatrix[2][0];
    localTransform.position.y=localMatrix[2][1];
    localTransform.rotation.angle=glm::degrees(glm::atan(localMatrix[0][1],localMatrix[0][0]));
    localTransform.scale.x=glm::length(glm::vec2(localMatrix[0][0],localMatrix[1][0]));
    localTransform.scale.y=glm::length(glm::vec2(localMatrix[0][1],localMatrix[1][1]));
    return localTransform;
}
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
    glm::mat3 localMatrix=transformToMatrix(localTransform);
    glm::mat3 parentWorldMatrix=transformToMatrix(parentWorldTransform);
    glm::mat3 worldMatrix=parentWorldMatrix*localMatrix;
    //提取矩阵数据
    worldTransform.position.x=worldMatrix[2][0];
    worldTransform.position.y=worldMatrix[2][1];
    worldTransform.rotation.angle=glm::degrees(glm::atan(worldMatrix[0][1],worldMatrix[0][0]));
    worldTransform.scale.x=glm::length(glm::vec2(worldMatrix[0][0],worldMatrix[1][0]));
    worldTransform.scale.y=glm::length(glm::vec2(worldMatrix[0][1],worldMatrix[1][1]));
    return worldTransform;
}

Transform TransformUtil::computeWorldToViewTransform(const Transform &worldTransform,const std::shared_ptr<Scene> &scene,const Entity camera) {
    Transform cameraTransform=ecs::getComponent<Transform>(scene, camera).value();
    Transform cameraWorldTransform=computeLocalToWorldTransform(cameraTransform,scene);
    Transform viewTransform = computeRelativeTransform(cameraWorldTransform, worldTransform, scene);
    return viewTransform;
}

Transform TransformUtil::computeRelativeTransform(const Transform &from, const Transform &to,
    const std::shared_ptr<Scene> &scene) {

    Transform worldTransformOA=computeLocalToWorldTransform(from,scene);
    Transform worldTransform1=computeLocalToWorldTransform(to,scene);
    return computeWorldToLocalTransform(worldTransform1,worldTransformOA,scene);
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

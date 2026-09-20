//
// Created by XL0002 on 2026/9/20.
//

#include "TransformSceneUtil.h"
#include "../Util.h"

Transform2D  TransformSceneUtil::computeWorldToLocalTransform(const Transform2D &worldTransform,const Transform2D &parentWorldTransform) {
    // 递归计算世界坐标
    glm::mat3 worldToParentMatrix=Transform2DUtil::getReverseTransformToMatrix(parentWorldTransform);
    glm::mat3 worldMatrix=Transform2DUtil::transformToMatrix(worldTransform);
    glm::mat3 localMatrix=worldToParentMatrix * worldMatrix;
    return Transform2DUtil::matrixToTransform(localMatrix);
}
Transform3D TransformSceneUtil::computeWorldToLocalTransform(const Transform3D &worldTransform,const Transform3D &parentWorldTransform) {
    // 递归计算世界坐标
    glm::mat4 worldToParentMatrix=Transform3DUtil::getReverseTransformToMatrix(parentWorldTransform);
    glm::mat4 worldMatrix=Transform3DUtil::transformToMatrix(worldTransform);
    glm::mat4 localMatrix=worldToParentMatrix * worldMatrix;
    return Transform3DUtil::matrixToTransform(localMatrix);
}

Transform2D TransformSceneUtil::computeLocalToWorldTransform(const Transform2DComp &localTransformComp, ecs::Scene* scene) {
    if (!localTransformComp.parent.has_value()) {
        return localTransformComp.transform;
    }

    auto parentEntity = localTransformComp.parent.value();
    auto parentTransformOpt = ecs::getComponent<Transform2DComp>(scene, parentEntity);
    if (!parentTransformOpt.has_value()) {
        return localTransformComp.transform;
    }
    // 递归计算世界坐标
    Transform2D parentWorldTransform = computeLocalToWorldTransform(parentTransformOpt.value(),scene);
    glm::mat3 localMatrix=Transform2DUtil::transformToMatrix(localTransformComp.transform);
    glm::mat3 parentWorldMatrix=Transform2DUtil::transformToMatrix(parentWorldTransform);
    glm::mat3 worldMatrix=parentWorldMatrix*localMatrix;
    return Transform2DUtil::matrixToTransform(worldMatrix);
}

Transform3D TransformSceneUtil::computeLocalToWorldTransform(const Transform3DComp &localTransformComp, ecs::Scene *scene) {
    if (!localTransformComp.parent.has_value()) {
        return localTransformComp.transform;
    }

    auto parentEntity = localTransformComp.parent.value();
    auto parentTransformOpt = ecs::getComponent<Transform3DComp>(scene, parentEntity);
    if (!parentTransformOpt.has_value()) {
        return localTransformComp.transform;
    }
    // 递归计算世界坐标
    Transform3D parentWorldTransform = computeLocalToWorldTransform(parentTransformOpt.value(),scene);
    glm::mat4 localMatrix=Transform3DUtil::transformToMatrix(localTransformComp.transform);
    glm::mat4 parentWorldMatrix=Transform3DUtil::transformToMatrix(parentWorldTransform);
    glm::mat4 worldMatrix=parentWorldMatrix*localMatrix;
    return Transform3DUtil::matrixToTransform(worldMatrix);
}


Transform2D TransformSceneUtil::computeRelativeTransform(const Transform2DComp &from, const Transform2DComp &to,
                                                         ecs::Scene* scene) {

    Transform2D worldTransformOA=computeLocalToWorldTransform(from,scene);
    Transform2D worldTransform1=computeLocalToWorldTransform(to,scene);
    return computeWorldToLocalTransform(worldTransform1,worldTransformOA);
}

Transform3D TransformSceneUtil::computeRelativeTransform(const Transform3DComp &from, const Transform3DComp &to,
    ecs::Scene *scene) {
    Transform3D worldTransformOA=computeLocalToWorldTransform(from,scene);
    Transform3D worldTransform1=computeLocalToWorldTransform(to,scene);
    return computeWorldToLocalTransform(worldTransform1,worldTransformOA);
}


std::vector<Entity> TransformSceneUtil::getChildEntities( ecs::Scene* scene,const Entity e1) {
    std::vector<Entity> children;
    for (auto entity : ecs::getEntities<Transform2DComp>(scene)) {
        if (ecs::getComponent<Transform2DComp>(scene, entity).value().parent==e1) {
            children.push_back(entity);
        }
    }
    return children;
}

//
// Created by XL0002 on 2026/7/21.
//

#include "TransformUtil.h"
#include "Transform2dComponents.h"
#include "../../Context.hpp"
#include "../../ecs/Util.h"
#include "../../Config.h"
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
glm::mat3 TransformUtil::getReverseTransformToMatrix(const Transform &transform) {
    Transform reverseTransform;
    reverseTransform.scale.x=1.0f/transform.scale.x;
    reverseTransform.scale.y=1.0f/transform.scale.y;
    reverseTransform.rotation.angle=-transform.rotation.angle;
    reverseTransform.position.x=-transform.position.x;
    reverseTransform.position.y=-transform.position.y;
    float sx=reverseTransform.scale.x;
    float sy=reverseTransform.scale.y;
    glm::mat3 matScale=glm::mat3(
        sx,0,0,
        0,sy,0,
        0,0,1
    );
    float c=glm::cos(glm::radians(reverseTransform.rotation.angle));
    float s=glm::sin(glm::radians(reverseTransform.rotation.angle));
    glm::mat3 matRotate=glm::mat3(
        c, s, 0,
        -s, c, 0,
        0, 0, 1
    );
    float px=reverseTransform.position.x;
    float py=reverseTransform.position.y;
    glm::mat3 matTranslate=glm::mat3(
        1,0,0,
        0,1,0,
        px,py,1
    );
    return matScale*matRotate*matTranslate;//反过来乘
}
Transform TransformUtil::matrixToTransform(const glm::mat3 &matrix) {
    Transform transform;
    transform.position.x=matrix[2][0];
    transform.position.y=matrix[2][1];
    float sx=glm::length(glm::vec2(matrix[0][0],matrix[1][0]));
    float sy=glm::length(glm::vec2(matrix[0][1],matrix[1][1]));
    float cos=(sx>0.0f)?matrix[0][0]/sx:1.0f;
    float sin=(sy>0.0f)?matrix[0][1]/sy:0.0f;
    transform.rotation.angle=glm::degrees(glm::atan(sin,cos));
    transform.scale.x=sx;
    transform.scale.y=sy;
    return transform;
}
Transform  TransformUtil::computeWorldToLocalTransform(const Transform &worldTransform,const Transform &parentWorldTransform) {
    // 递归计算世界坐标
    glm::mat3 parentWorldMatrix=getReverseTransformToMatrix(parentWorldTransform);
    glm::mat3 worldMatrix=transformToMatrix(worldTransform);
    glm::mat3 localMatrix=parentWorldMatrix*worldMatrix;
    return matrixToTransform(localMatrix);
}

Transform TransformUtil::computeLocalToWorldTransform(const TransformComp &localTransform, ecs::Scene* scene) {
    if (!localTransform.parent.has_value()) {
        return localTransform.transform;
    }

    auto parentEntity = localTransform.parent.value();
    auto parentTransformOpt = ecs::getComponent<TransformComp>(scene, parentEntity);
    if (!parentTransformOpt.has_value()) {
        return localTransform.transform;
    }
    Transform worldTransform;
    // 递归计算世界坐标
    Transform parentWorldTransform = computeLocalToWorldTransform(parentTransformOpt.value(),scene);
    glm::mat3 localMatrix=transformToMatrix(localTransform.transform);
    glm::mat3 parentWorldMatrix=transformToMatrix(parentWorldTransform);
    glm::mat3 worldMatrix=parentWorldMatrix*localMatrix;
    return matrixToTransform(worldMatrix);
}


Transform TransformUtil::computeRelativeTransform(const TransformComp &from, const TransformComp &to,
                                                  ecs::Scene* scene) {

    Transform worldTransformOA=computeLocalToWorldTransform(from,scene);
    Transform worldTransform1=computeLocalToWorldTransform(to,scene);
    return computeWorldToLocalTransform(worldTransform1,worldTransformOA);
}

TransformComp TransformUtil::rotate(const TransformComp &transform, float angle,Position pivot) {
    float rad = glm::radians(angle);
    //正反平移矩阵
    glm::mat3 Tneg = glm::mat3(1,0,0, 0,1,0, -pivot.x,-pivot.y,1);
    glm::mat3 Tpos = glm::mat3(1,0,0, 0,1,0, pivot.x,pivot.y,1);
    float c=glm::cos(rad), s=glm::sin(rad);
    glm::mat3 R = glm::mat3(c, s, 0, -s, c, 0, 0, 0, 1);
    glm::mat3 m = Tpos * R * Tneg * transformToMatrix(transform.transform);
    // 抽取回 Transform（与 computeLocalToWorldTransform 一致）
    Transform out_transform=matrixToTransform(m);
    TransformComp out;
    out.transform = out_transform;
    out.parent = transform.parent;
    return out;
}

Position TransformUtil::screenToWorldPosition(Position screenPos, TransformComp cameraTransformComp, CameraComp camera_comp) {
    auto config=ApplicationContext::getInstance().get<Config>("config");
    auto x=(screenPos.x/config.LOGIC_WIDTH-0.5f)*2*camera_comp.captureWidth;
    auto y=(screenPos.y/config.LOGIC_HEIGHT-0.5f)*2*camera_comp.captureHeight;
    auto viewTransform=Transform{{x,y},{0},{1,1}};
    auto viewToWorldMatrix = getReverseTransformToMatrix(cameraTransformComp.transform);
    auto worldMatrix=viewToWorldMatrix*transformToMatrix(viewTransform);
    return matrixToTransform(worldMatrix).position;
}

std::vector<Entity> TransformUtil::getChildEntities( ecs::Scene* scene,const Entity e1) {
    std::vector<Entity> children;
    for (auto entity : ecs::getEntities<TransformComp>(scene)) {
        if (ecs::getComponent<TransformComp>(scene, entity).value().parent==e1) {
            children.push_back(entity);
        }
    }
    return children;
}

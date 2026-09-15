//
// Created by XL0002 on 2026/7/21.
//

#include "TransformUtil.h"
#include "Transform2dComponents.h"
#include "glm/glm.hpp"
#include "../../core/ecs/Util.h"
#include "../../core/Context.hpp"
#include "../../Config.h"

glm::mat3 TransformUtil::transformToMatrix(const Transform2D &transform) {
    float sx=transform.scale.x;
    float sy=transform.scale.y;
    glm::mat3 matScale=glm::mat3(
        sx,0,0,
        0,sy,0,
        0,0,1
    );
    float c=glm::cos(glm::radians(transform.rotation));
    float s=glm::sin(glm::radians(transform.rotation));
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
glm::mat3 TransformUtil::getReverseTransformToMatrix(const Transform2D &transform) {
    Transform2D reverseTransform;
    reverseTransform.scale.x=1.0f/transform.scale.x;
    reverseTransform.scale.y=1.0f/transform.scale.y;
    reverseTransform.rotation=-transform.rotation;
    reverseTransform.position.x=-transform.position.x;
    reverseTransform.position.y=-transform.position.y;
    float sx=reverseTransform.scale.x;
    float sy=reverseTransform.scale.y;
    glm::mat3 matScale=glm::mat3(
        sx,0,0,
        0,sy,0,
        0,0,1
    );
    float c=glm::cos(glm::radians(reverseTransform.rotation));
    float s=glm::sin(glm::radians(reverseTransform.rotation));
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
Transform2D TransformUtil::matrixToTransform(const glm::mat3 &matrix) {
    Transform2D transform;
    transform.position.x=matrix[2][0];
    transform.position.y=matrix[2][1];
    float sx=glm::length(glm::vec2(matrix[0][0],matrix[1][0]));
    float sy=glm::length(glm::vec2(matrix[0][1],matrix[1][1]));
    float cos=(sx>0.0f)?matrix[0][0]/sx:1.0f;
    float sin=(sy>0.0f)?matrix[0][1]/sy:0.0f;
    transform.rotation=glm::degrees(glm::atan(sin,cos));
    transform.scale.x=sx;
    transform.scale.y=sy;
    return transform;
}
Transform2D  TransformUtil::computeWorldToLocalTransform(const Transform2D &worldTransform,const Transform2D &parentWorldTransform) {
    // 递归计算世界坐标
    glm::mat3 parentWorldMatrix=getReverseTransformToMatrix(parentWorldTransform);
    glm::mat3 worldMatrix=transformToMatrix(worldTransform);
    glm::mat3 localMatrix=parentWorldMatrix*worldMatrix;
    return matrixToTransform(localMatrix);
}

Transform2D TransformUtil::computeLocalToWorldTransform(const Transform2DComp &localTransform, ecs::Scene* scene) {
    if (!localTransform.parent.has_value()) {
        return localTransform.transform;
    }

    auto parentEntity = localTransform.parent.value();
    auto parentTransformOpt = ecs::getComponent<Transform2DComp>(scene, parentEntity);
    if (!parentTransformOpt.has_value()) {
        return localTransform.transform;
    }
    Transform2D worldTransform;
    // 递归计算世界坐标
    Transform2D parentWorldTransform = computeLocalToWorldTransform(parentTransformOpt.value(),scene);
    glm::mat3 localMatrix=transformToMatrix(localTransform.transform);
    glm::mat3 parentWorldMatrix=transformToMatrix(parentWorldTransform);
    glm::mat3 worldMatrix=parentWorldMatrix*localMatrix;
    return matrixToTransform(worldMatrix);
}


Transform2D TransformUtil::computeRelativeTransform(const Transform2DComp &from, const Transform2DComp &to,
                                                  ecs::Scene* scene) {

    Transform2D worldTransformOA=computeLocalToWorldTransform(from,scene);
    Transform2D worldTransform1=computeLocalToWorldTransform(to,scene);
    return computeWorldToLocalTransform(worldTransform1,worldTransformOA);
}
Transform2DComp TransformUtil::rotate(const Transform2DComp &transform, float angle,glm::vec2 pivot) {
    float rad = glm::radians(angle);
    //正反平移矩阵
    glm::mat3 Tneg = glm::mat3(1,0,0, 0,1,0, -pivot.x,-pivot.y,1);
    glm::mat3 Tpos = glm::mat3(1,0,0, 0,1,0, pivot.x,pivot.y,1);
    float c=glm::cos(rad), s=glm::sin(rad);
    glm::mat3 R = glm::mat3(c, s, 0, -s, c, 0, 0, 0, 1);
    glm::mat3 m = Tpos * R * Tneg * transformToMatrix(transform.transform);
    // 抽取回 Transform（与 computeLocalToWorldTransform 一致）
    Transform2D out_transform=matrixToTransform(m);
    Transform2DComp out;
    out.transform = out_transform;
    out.parent = transform.parent;
    return out;
}

glm::vec2 TransformUtil::screenToWorldPosition(glm::vec2 screenPos, Transform2DComp cameraTransformComp, Camera2DComp camera_comp) {
    auto config=ApplicationContext::getInstance().get<Config>("config");
    auto x=(screenPos.x/config.WINDOW_WIDTH)*camera_comp.captureWidth;
    auto y=(screenPos.y/config.WINDOW_HEIGHT)*camera_comp.captureHeight;
    auto viewTransform=Transform2D{{x,y},{0},{1,1}};
    auto viewToWorldMatrix = getReverseTransformToMatrix(cameraTransformComp.transform);
    auto worldMatrix=viewToWorldMatrix*transformToMatrix(viewTransform);
    return matrixToTransform(worldMatrix).position;
}

std::vector<Entity> TransformUtil::getChildEntities( ecs::Scene* scene,const Entity e1) {
    std::vector<Entity> children;
    for (auto entity : ecs::getEntities<Transform2DComp>(scene)) {
        if (ecs::getComponent<Transform2DComp>(scene, entity).value().parent==e1) {
            children.push_back(entity);
        }
    }
    return children;
}

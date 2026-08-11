//
// Created by XL0002 on 2026/7/21.
//

#ifndef GAMEENGINE_TRANSFORMUTIL_H
#define GAMEENGINE_TRANSFORMUTIL_H
#include "Transform2dComponents.h"
#include "../../ecs/BaseComponents.h"
#include "glm/fwd.hpp"
#include "glm/detail/type_mat3x3.hpp"


class TransformUtil {
    public:
    static glm::mat3 transformToMatrix(const Transform &transform);

    static glm::mat3 getReverseTransformToMatrix(const Transform &transform);
    static Transform matrixToTransform(const glm::mat3 &matrix);
    static Transform computeWorldToLocalTransform(const Transform &worldTransform,
                                                  const Transform &parentWorldTransform);

    static Transform computeLocalToWorldTransform(const TransformComp &localTransform,
                                                  Scene* scene);
    static Transform computeRelativeTransform(const TransformComp &from, const TransformComp &to, Scene* scene);
    //在局部坐标旋转，不涉及世界变换
    static TransformComp rotate(const TransformComp &transform, float angle, Position pivot);
    static Position screenToWorldPosition(Position ,TransformComp,CameraComp);
    static std::vector<Entity> getChildEntities(
        Entity e1, Scene* scene);
};


#endif //GAMEENGINE_TRANSFORMUTIL_H

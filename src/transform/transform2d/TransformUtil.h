//
// Created by XL0002 on 2026/7/21.
//

#ifndef GAMEENGINE_TRANSFORMUTIL_H
#define GAMEENGINE_TRANSFORMUTIL_H
#include "Transform2dComponents.h"
#include "glm/fwd.hpp"
#include "glm/detail/type_mat3x3.hpp"
#include "../../core/ecs/Scene.h"


class TransformUtil {
    public:
    static glm::mat3 transformToMatrix(const Transform2D &transform);

    static glm::mat3 getReverseTransformToMatrix(const Transform2D &transform);
    static Transform2D matrixToTransform(const glm::mat3 &matrix);
    static Transform2D computeWorldToLocalTransform(const Transform2D &worldTransform,
                                                  const Transform2D &parentWorldTransform);

    static Transform2D computeLocalToWorldTransform(const Transform2DComp &localTransform,
                                                  ecs::Scene* scene);
    static Transform2D computeRelativeTransform(const Transform2DComp &from,
        const Transform2DComp &to, ecs::Scene* scene);
    //在局部坐标旋转，不涉及世界变换
    static Transform2DComp rotate(const Transform2DComp &transform, float angle, glm::vec2 pivot);
    static glm::vec2 screenToWorldPosition(glm::vec2 screenPos,Transform2DComp,Camera2DComp);
    static std::vector<Entity> getChildEntities(
        ecs::Scene *scene, Entity e1);
};


#endif //GAMEENGINE_TRANSFORMUTIL_H

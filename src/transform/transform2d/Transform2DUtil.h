//
// Created by XL0002 on 2026/7/21.
//

#ifndef GAMEENGINE_TRANSFORMUTIL_H
#define GAMEENGINE_TRANSFORMUTIL_H
#include "Transform2DComponents.h"
#include "glm/fwd.hpp"
#include "glm/detail/type_mat3x3.hpp"


class Transform2DUtil {
    public:
    static glm::mat3 transformToMatrix(const Transform2D &transform);

    static Transform2D matrixToTransform(const glm::mat3 &matrix);
    //在局部坐标旋转，不涉及世界变换
    static Transform2D rotate(const Transform2D &transform, float angle, glm::vec2 pivot);
    static glm::mat3 getReverseTransformToMatrix(const Transform2D &transform);

};


#endif //GAMEENGINE_TRANSFORMUTIL_H

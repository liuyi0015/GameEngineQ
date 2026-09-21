//
// Created by XL0002 on 2026/9/16.
//

#ifndef GAMEENGINEQ_TRANSFORM3DUTIL_H
#define GAMEENGINEQ_TRANSFORM3DUTIL_H
#include "Transform3dComponents.h"
#include "glm/fwd.hpp"


class Transform3DUtil {
public:
    //垂直视场角，屏幕宽高比，近z，远z
    static glm::mat4 matProject(float fovY,float aspect, float nearZ, float farZ);
    static glm::mat4 matViewport(float W, float H);
    static glm::mat4 transformToMatrix(Transform3D worldTransform);
    static Transform3D matrixToTransform(glm::mat4 matrix) ;
    static glm::mat4 getReverseTransformToMatrix(const Transform3D & transform);
};


#endif //GAMEENGINEQ_TRANSFORM3DUTIL_H

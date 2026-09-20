//
// Created by XL0002 on 2026/9/16.
//

#include "Transform3DUtil.h"
#include "glm/glm.hpp"

static glm::mat4 matScale(float sx, float sy, float sz) {
    glm::mat4 result=glm::mat4(
    sx,0,0,0,
    0,sy,0,0,
    0,0,sz,0,
    0,0,0,1
        );
    return result;
}
static glm::mat4 matRotate(float angleX, float angleY,float angleZ) {
    float sinX=glm::sin(angleX);
    float cosX=glm::cos(angleX);
    glm::mat4 resultX=glm::mat4(
        1,0,0,0,
        0,cosX,-sinX,0,
        0,sinX,cosX,0,
        0,0,0,1
        );
    //y轴负号在左下角，和左右手系无关，是z->x变换写在x->z矩阵的效果
    float sinY=glm::sin(angleY);
    float cosY=glm::cos(angleY);
    glm::mat4 resultY=glm::mat4(
        cosY,0,sinY,0,
        0,1,0,0,
        -sinY,0,cosY,0,
        0,0,0,1
        );
    float sinZ=glm::sin(angleZ);
    float cosZ=glm::cos(angleZ);
    glm::mat4 resultZ=glm::mat4(
        cosZ,-sinZ,0,0,
        sinZ,cosZ,0,0,
        0,0,1,0,
        0,0,0,1
        );
    glm::mat4 result=resultZ*resultY*resultX;
    return result;
}
static glm::mat4 matTranslate(float x, float y, float z) {
    glm::mat4 result=glm::mat4(
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        x,y,z,1
        );
    return result;
}

glm::mat4 Transform3DUtil::matProject(float fov,float a, float n, float f) {
    glm::mat4 result=glm::mat4(
        1/(a*glm::tan(fov/2)),0,0,0,
        0,1/glm::tan(fov/2),0,0,
        0,0,f/(n-f), -f*n/(f-n),
        0,0,1,0
        );
    return result;
}
glm::mat4 Transform3DUtil::matViewport(float W, float H) {
    //注意缩放H的负号，表示y轴反向
    glm::mat4 result=glm::mat4(
        W/2,0,0,W/2,
        0,-H/2,0,H/2,
        0,0,1,0,
        0,0,0,1
        );
    return result;
}
glm::mat4 Transform3DUtil::transformToMatrix(Transform3D transform) {
    float sx=transform.scale.x;
    float sy=transform.scale.y;
    float sz=transform.scale.z;
    glm::mat3 scaleMat=matScale(sx,sy,sz);
    glm::mat3 rotMat=matRotate(glm::radians(transform.rotation.x),
        glm::radians(transform.rotation.y),glm::radians(transform.rotation.z));
    float px=transform.position.x;
    float py=transform.position.y;
    float pz=transform.position.z;
    glm::mat3 transMat=matTranslate(px,py,pz);
    //从右往左应用是列向量约定，v在最右边，所以是trs
    return transMat*rotMat*scaleMat;
}
Transform3D Transform3DUtil::matrixToTransform(glm::mat4 matrix) {
    Transform3D result;
    return result;
}

glm::mat4 Transform3DUtil::getReverseTransformToMatrix(const Transform3D &transform) {
    Transform3D reverseTransform;
    reverseTransform.scale.x=1.0f/transform.scale.x;
    reverseTransform.scale.y=1.0f/transform.scale.y;
    reverseTransform.scale.z=1.0f/transform.scale.z;
    reverseTransform.rotation=-transform.rotation;
    reverseTransform.position=-transform.position;
    float sx=reverseTransform.scale.x;
    float sy=reverseTransform.scale.y;
    float sz=reverseTransform.scale.z;
    glm::mat4 scaleMat=matScale(sx,sy,sz);
    glm::mat4 rotMat=matRotate(glm::radians(transform.rotation.x),
        glm::radians(transform.rotation.y),glm::radians(transform.rotation.z));
    float px=reverseTransform.position.x;
    float py=reverseTransform.position.y;
    float pz=reverseTransform.position.z;
    glm::mat4 transMat=matTranslate(px,py,pz);
    //反过来乘，表示一步一步撤销变换，先撤销平移再撤销旋转最后缩放
    return scaleMat*rotMat*transMat;
}


//
// Created by XL0002 on 2026/7/21.
//

#include "Transform2DUtil.h"
#include "Transform2DComponents.h"
#include "glm/glm.hpp"
//glm默认列主序，但写的时候排版是行主序
static glm::mat3 matScale(float sx,float sy) {
    glm::mat3 matScale=glm::mat3(
        sx,0,0,
        0,sy,0,
        0,0,1);
    return matScale;
}
static glm::mat3 matRotate(float angle) {
    float sin=glm::sin(angle);
    float cos=glm::cos(angle);
    glm::mat3 matScale=glm::mat3(
        cos,-sin,0,
        sin,cos,0,
        0,0,1);
    return matScale;
}
static glm::mat3 matTranslate(float px,float py) {
    glm::mat3 matTranslate=glm::mat3(
        1,0,0,
        0,1,0,
        px,py,1
        );
    return matTranslate;
}

glm::mat3 Transform2DUtil::transformToMatrix(const Transform2D &transform) {
    float sx=transform.scale.x;
    float sy=transform.scale.y;
    glm::mat3 scaleMat=matScale(sx,sy);
    glm::mat3 rotMat=matRotate(glm::radians(transform.rotation));
    float px=transform.position.x;
    float py=transform.position.y;
    glm::mat3 transMat=matTranslate(px,py);
    //从右往左应用是列向量约定，v在最右边，所以是trs
    return transMat*rotMat*scaleMat;
}
Transform2D Transform2DUtil::matrixToTransform(const glm::mat3 &matrix) {
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

Transform2D Transform2DUtil::rotate(const Transform2D &transform, float angle, glm::vec2 pivot) {
    float rad = glm::radians(angle);
    //正反平移矩阵
    glm::mat3 Tneg = matTranslate(-pivot.x,-pivot.y);
    glm::mat3 Tpos = matTranslate(pivot.x,pivot.y);
    float c=glm::cos(rad), s=glm::sin(rad);
    glm::mat3 R = matScale(s,c);
    glm::mat3 m = Tpos * R * Tneg * transformToMatrix(transform);//todo
    Transform2D out=matrixToTransform(m);
    return out;
}
glm::mat3 Transform2DUtil::getReverseTransformToMatrix(const Transform2D &transform) {
    Transform2D reverseTransform;
    reverseTransform.scale.x=1.0f/transform.scale.x;
    reverseTransform.scale.y=1.0f/transform.scale.y;
    reverseTransform.rotation=-transform.rotation;
    reverseTransform.position.x=-transform.position.x;
    reverseTransform.position.y=-transform.position.y;
    float sx=reverseTransform.scale.x;
    float sy=reverseTransform.scale.y;
    glm::mat3 scaleMat=matScale(sx,sy);
    glm::mat3 rotMat=matRotate(glm::radians(transform.rotation));
    float px=reverseTransform.position.x;
    float py=reverseTransform.position.y;
    glm::mat3 transMat=matTranslate(px,py);
    //反过来乘，表示一步一步撤销变换，先撤销平移再撤销旋转最后缩放
    return scaleMat*rotMat*transMat;

}
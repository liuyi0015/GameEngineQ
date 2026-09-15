//
// Created by XL0002 on 2026/8/25.
//

#include "Shape2DBuilder.h"

Shape2D Shape2DBuilder::createRect(glm::vec2 pivot, glm::vec2 size) {
    Shape2D shape;
    // 坐标系 y 轴向上
    glm::vec2 leftDown =  {-1,-1};
    glm::vec2 leftUp =    {-1, 1};
    glm::vec2 rightUp =   { 1, 1};
    glm::vec2 rightDown = { 1,-1};
    leftDown=leftDown*glm::vec2{size.x/2,size.y/2}+pivot;
    leftUp=leftUp*glm::vec2{size.x/2,size.y/2}+pivot;
    rightUp=rightUp*glm::vec2{size.x/2,size.y/2}+pivot;
    rightDown=rightDown*glm::vec2{size.x/2,size.y/2}+pivot;

    shape.points.push_back(leftUp);     // 0
    shape.points.push_back(rightUp);    // 1
    shape.points.push_back(rightDown);  // 2
    shape.points.push_back(leftDown);   // 3
    //顺时针
    shape.indices.push_back({0, 1, 2});//直角右上
    shape.indices.push_back({0, 2, 3});//直角左下

    return shape;
}

Shape2D Shape2DBuilder::createCircle(glm::vec2 pivot, float radius, int segments) {
    Shape2D shape;
    shape.points.push_back(pivot);

    constexpr float kPi = 3.14159265358979323846f;
    const float tau = 2.0f * kPi;
    for (int i = 0; i < segments; ++i) {
        float angle = tau * static_cast<float>(i) / static_cast<float>(segments);
        float lx = std::cos(angle) * radius;
        float ly = std::sin(angle) * radius;
        shape.points.push_back({lx, ly});
    }

    for (int i = 0; i < segments; ++i) {
        int a = 0;
        int b = i + 1;
        int c = (i + 2 <= segments) ? (i + 2) : 1;
        shape.indices.push_back({a, b, c});
    }
    return shape;
}// namespace Mesh
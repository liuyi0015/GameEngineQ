//
// Created by XL0002 on 2026/8/25.
//

#include "MeshBuilder.h"

Mesh::Shape2D createRect(glm::vec2 center,glm::vec2 size) {
    Mesh::Shape2D shape;
    //坐标系 y轴向上
    glm::vec2 leftDown = {center.x - size.x/2,center.y - size.y/2};
    glm::vec2 rightDown = {center.x + size.x/2,center.y - size.y/2};
    glm::vec2 leftUp = {center.x - size.x/2,center.y + size.y/2};
    glm::vec2 rightUp = {center.x + size.x/2,center.y + size.y/2};

    shape.points.push_back(leftDown);//0
    shape.points.push_back(rightDown);//1
    shape.points.push_back(leftUp);//2
    shape.points.push_back(rightUp);//3

    shape.indices.push_back({0,1,2});//直角在左下
    shape.indices.push_back({3,1,2});//直角在右上

    return shape;
};

Mesh::Shape2D createCircle(glm::vec2 center,float radius,int segments) {
    Mesh::Shape2D shape;
    shape.points.push_back(center);
    // 使用多个线段近似圆形
    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        float lx = std::cos(angle) * radius;
        float ly = std::sin(angle) * radius;
        shape.points.push_back({lx,ly});
    }
    for (int i = 0; i < segments; ++i) {
        int a = 0;
        int b = i + 1;
        int c = (i + 2 <= segments) ? (i + 2) : 1;
        shape.indices.push_back({a,b,c});
    }
    return shape;
}
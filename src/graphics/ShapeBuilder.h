//
// Created by XL0002 on 2026/8/25.
//

#ifndef GAMEENGINEQ_GEOMETRYBUILDER_H
#define GAMEENGINEQ_GEOMETRYBUILDER_H
#include <cmath>
#include <iostream>
#include <ostream>
#include <vector>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
namespace Geometry {

    struct CircleComponent {
        float radius;
        // 分段数，越多越平滑
        int segments;
    };

    struct RectComponent {
        float width;
        float height;
    };
    //直接存坐标的三角形
    struct PointTriangle2D {
        glm::vec2 p1,p2,p3;
        bool contains(const glm::vec2& p)const {
            auto sign = [](const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3){
                return (p1.x - p3.x)*(p2.y - p3.y) - (p2.x - p3.x)*(p1.y - p3.y);
            };
            bool b1 = sign(p,p1,p2) < 0.0f;
            bool b2 = sign(p,p2,p3) < 0.0f;
            bool b3 = sign(p,p3,p1) < 0.0f;
            return (b1==b2) && (b2==b3);
        }
        //面积
        [[nodiscard]] float area()const {
            // std::cout<<"area()"<<std::abs(p1.x*(p2.y-p3.y)+p2.x*(p3.y-p1.y)+p3.x*(p1.y-p2.y)/2.0f)<<std::endl;
            return std::abs(p1.x*(p2.y-p3.y)+p2.x*(p3.y-p1.y)+p3.x*(p1.y-p2.y)/2.0f);
        }
    };
    struct Shape2D {
        std::vector<glm::vec2> points;
        std::vector<glm::ivec3> indices;
    };
    Shape2D createCircle(glm::vec2 center,float radius,int segments);
     Shape2D createRect(glm::vec2 center,glm::vec2 size);
}

#endif //GAMEENGINEQ_GEOMETRYBUILDER_H

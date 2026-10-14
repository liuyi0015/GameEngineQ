//
// Created by XL0002 on 2026/10/12.
//

#ifndef GAMEENGINEQ_RENDERCOMPONENTS_H
#define GAMEENGINEQ_RENDERCOMPONENTS_H
#include "glm/vec3.hpp"
#include <any>
struct Mesh {
    std::vector<std::any> vertices;
    std::vector<glm::ivec3> indices;
};
struct Material {
    //每个物体在不同pass用到的着色器等参数不同
    std::unordered_map<std::string,std::any> passRecipeMap;
};
//绘制声明，和资源声明互不干扰
struct Drawable2DFlag {
    int z_order = 0;
    Material material;
    Mesh mesh;
};
struct Drawable3DFlag {
    Material material;
    Mesh mesh;
};

struct Camera2DComp {
    float captureWidth;
    float captureHeight;
    std::string targetName;
};
struct Camera3DComp {
    //垂直视场角
    float fov;
    //视锥宽高比
    float a;
    float nearZ;
    float farZ;
    std::string targetName;
};

#endif //GAMEENGINEQ_RENDERCOMPONENTS_H

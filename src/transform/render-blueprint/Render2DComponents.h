//
// Created by XL0002 on 2026/7/22.
//

#ifndef GAMEENGINE_RENDERCOMPONENTS_H
#define GAMEENGINE_RENDERCOMPONENTS_H
#include <string>
#include <vector>

#include "Pipelines.h"
#include "../../graphics/Shape2DBuilder.h"
#include "SDL3/SDL_pixels.h"
//后面可能分成不同的材质类型
struct Material {
    std::string pipeline_name;
    SDL_Color color;
    std::string texResourceId;
};

struct Mesh2D {
    std::vector<VertexAttrib2D> vertices;
    std::vector<glm::ivec3> indices;
};
//绘制声明，和资源声明互不干扰
struct Drawable2DFlag {
    int z_order = 0;
    Material material;
    Mesh2D mesh;
};

struct Mesh3D {
    std::vector<glm::vec3> vertices;
    std::vector<glm::ivec3> indices;
};
struct Drawable3DFlag {
    int z_order = 0;
    Material material;
    Mesh3D mesh;
};
#endif //GAMEENGINE_RENDERCOMPONENTS_H

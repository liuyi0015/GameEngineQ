//
// Created by XL0002 on 2026/7/22.
//

#ifndef GAMEENGINE_RENDERCOMPONENTS_H
#define GAMEENGINE_RENDERCOMPONENTS_H
#include <string>
#include <vector>

#include "../../graphics/MeshBuilder.h"
#include "SDL3/SDL_pixels.h"
//后面可能分成不同的材质类型
struct Material {
    std::string pipeline_name;
    SDL_Color color;
    std::string texResourceId;
    std::vector<glm::vec2>uvs;
};
//绘制声明，和资源声明互不干扰
struct Drawable2DFlag {
    int z_order = 0;
    std::string renderpass_name;//target
    Material material;
    Mesh::Shape2D shape;
    // Position pivot{0,0};
};

#endif //GAMEENGINE_RENDERCOMPONENTS_H

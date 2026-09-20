//
// Created by XL0002 on 2026/7/22.
//

#ifndef GAMEENGINE_RENDERCOMPONENTS_H
#define GAMEENGINE_RENDERCOMPONENTS_H
#include <string>
#include <vector>
#include "SDL3/SDL_pixels.h"
#include "glm/glm.hpp"
#include "../../../soft-render/GpuSimulator.h"


//后面可能分成不同的材质类型
struct Material {
    std::string pipeline_name;
    SDL_Color color;
    std::string texResourceId;
};

struct Camera2DComp {
    float captureWidth;
    float captureHeight;
    ColorBuffer* target;
};
struct VertexAttrib2D {//每个顶点各自的数据
    glm::vec2 pos;// ndc
    glm::vec2 uv;
    glm::vec4 color;
};
struct Uniform2D:Uniform {
    ColorBuffer* texture;
    glm::mat3 mvpMatrix;
    ~Uniform2D() override {
        delete texture;
    }

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

struct VertexAttrib3D {
    glm::vec3 pos;//w要留下做透视除法和裁剪
    glm::vec2 uv;
    glm::vec4 color;
};
struct Uniform3D:Uniform {
    ColorBuffer* texture;
    glm::mat4 mvpMatrix;
    ~Uniform3D() override {
        delete texture;
    }
};
struct Camera3DComp {
    float captureWidth;
    float captureHeight;
    float captureZ;
    ColorBuffer* target;
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

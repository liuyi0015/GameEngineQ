//
// Created by abc17 on 2026/9/6.
//

#ifndef GAMEENGINEQ_PIPELINES_H
#define GAMEENGINEQ_PIPELINES_H

#include "../../soft-render/SoftRenderer2D.h"

struct Uniform {//一次绘制中全局不变的数据
    glm::mat3 mvpMatrix;//需要投影到ndc坐标
    ColorBuffer* texture;
    // 其他全局数据，比如时间、透明度、混合颜色、光照、高度图等也可以放这里
};

struct VertexAttrib {//每个顶点各自的数据
    glm::vec2 pos;// ndc
    glm::vec2 uv;
    glm::vec4 color;
    glm::vec2 normal;
};
struct VertexShaderOutput {
    glm::vec2 pos;
    glm::vec2 uv;
    glm::vec4 color;
};
struct FragmentAttrib {
    glm::vec2 viewPos;
    glm::vec4 color;
    glm::vec2 uv;
};
class DefaultPipeline:public IPipeline {
protected:
    void* vertexShader(void* in, const void *uniform) override;
    glm::vec4 fragmentShader(void* in, const void *uniform) override;
};

struct ComposeUniform {
    std::vector<ColorBuffer*>textures;
};
class ComposePipeline:public IPipeline {
protected:
    void* vertexShader(void* in, const void *uniform) override;
    glm::vec4 fragmentShader(void* in, const void *uniform) override;
};

#endif //GAMEENGINEQ_PIPELINES_H

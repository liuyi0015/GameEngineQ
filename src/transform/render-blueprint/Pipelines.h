//
// Created by abc17 on 2026/9/6.
//

#ifndef GAMEENGINEQ_PIPELINES_H
#define GAMEENGINEQ_PIPELINES_H

#include "../../soft-render/GpuSimulator.h"
struct VertexAttrib2D {//每个顶点各自的数据
    glm::vec2 pos;// ndc
    glm::vec2 uv;
    glm::vec4 color;
};
struct VertexAttrib3D {
    glm::vec3 pos;//w要留下做透视除法和裁剪
    glm::vec2 uv;
    glm::vec4 color;
};
struct UniformCompositor:Uniform {
    //合成器用的
    std::vector<ColorBuffer*>textures;
    // 其他全局数据，比如时间、透明度、混合颜色、光照、高度图等也可以放这里
    ~UniformCompositor() {
        for (auto* tb : textures) {
            delete tb;
        }
    }
};
struct Uniform2D:Uniform {
    ColorBuffer* texture;
    glm::mat3 mvpMatrix;
    ~Uniform2D() {
        delete texture;
    }
};
struct Uniform3D:Uniform {
    ColorBuffer* texture;
    glm::mat4 mvpMatrix;
    ~Uniform3D() {
        delete texture;
    }
};
class Default2DPipeline:public IPipeline {
protected:
    VertexShaderOutput* vertexShader(const std::any& in, const Uniform* uniform) override;
    glm::vec4 fragmentShader(const FragmentAttrib* in, const Uniform* uniform) override;
};

class ComposePipeline:public IPipeline {
protected:
    VertexShaderOutput* vertexShader(const std::any& in, const Uniform* uniform) override;
    glm::vec4 fragmentShader(const FragmentAttrib* in, const Uniform* uniform) override;
};

#endif //GAMEENGINEQ_PIPELINES_H

//
// Created by abc17 on 2026/9/6.
//

#ifndef GAMEENGINEQ_PIPELINES_H
#define GAMEENGINEQ_PIPELINES_H

#include "../../../soft-render/GpuSimulator.h"


class Default2DShader:public IShader {
public:
    VertexShaderOutput* vertexShader(const std::any& in, const Uniform* uniform) override;
    glm::vec4 fragmentShader(const FragmentAttrib* in, const Uniform* uniform) override;
};

class Default3DShader:public IShader {
public:
    VertexShaderOutput* vertexShader(const std::any& in, const Uniform* uniform) override;
    glm::vec4 fragmentShader(const FragmentAttrib* in, const Uniform* uniform) override;
};

class ComposeShader:public IShader {
public:
    VertexShaderOutput* vertexShader(const std::any& in, const Uniform* uniform) override;
    glm::vec4 fragmentShader(const FragmentAttrib* in, const Uniform* uniform) override;
};
#endif //GAMEENGINEQ_PIPELINES_H

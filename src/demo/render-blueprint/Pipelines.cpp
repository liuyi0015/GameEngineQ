//
// Created by abc17 on 2026/9/6.
//

#include "Pipelines.h"

VertexShaderOutput *DefaultPipeline::vertexShader(const VertexAttrib *in, const Uniform *uniform) {
    auto out= new VertexShaderOutput();
    //把顶点从模型空间换到裁剪空间直接到ndc空间
    glm::vec3 ndcPos = uniform->mvpMatrix * glm::vec3(in->pos.x, in->pos.y, 1.0f);
    out->pos=ndcPos;
    //采样
    glm::vec4 color={1,1,1,1};
    if (uniform->texture!=nullptr) {
        color.r=uniform->texture->at(in->uv.x,in->uv.y).r;
        color.g=uniform->texture->at(in->uv.x,in->uv.y).g;
        color.b=uniform->texture->at(in->uv.x,in->uv.y).b;
        color.a=uniform->texture->at(in->uv.x,in->uv.y).a;
    }
    out->color = color;
    return out;
}

glm::vec4 DefaultPipeline::fragmentShader(const FragmentAttrib *in, const Uniform *uniform) {
    glm::vec4 out;
    if (uniform->texture!=nullptr) {
        out.r=uniform->texture->at(in->uv.x,in->uv.y).r;
        out.g=uniform->texture->at(in->uv.x,in->uv.y).g;
        out.b=uniform->texture->at(in->uv.x,in->uv.y).b;
        out.a=uniform->texture->at(in->uv.x,in->uv.y).a;
    }else {
        out.r=in->color.r;
        out.g=in->color.g;
        out.b=in->color.b;
        out.a=in->color.a;
    }
    return out;
}

VertexShaderOutput *ComposePipeline::vertexShader(const VertexAttrib *in, const Uniform *uniform) {
    auto out=new VertexShaderOutput();

    return out;
}

glm::vec4 ComposePipeline::fragmentShader(const FragmentAttrib *in, const Uniform *uniform) {
    glm::vec4 out;

    return out;
}

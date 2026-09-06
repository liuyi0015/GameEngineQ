//
// Created by abc17 on 2026/9/6.
//

#include "Pipelines.h"

void *DefaultPipeline::vertexShader(void *inData, const void *uniformData) {
    //转换为具体类型
    auto uniform=static_cast<const Uniform*>(uniformData);
    VertexAttrib in=*static_cast<VertexAttrib *>(inData);
    auto out= new VertexShaderOutput();
    //把顶点从模型空间换到裁剪空间直接到ndc空间
    glm::vec3 ndcPos = uniform->mvpMatrix * glm::vec3(in.pos.x, in.pos.y, 1.0f);
    out->pos=ndcPos;
    //采样
    glm::vec4 color={1,1,1,1};
    if (uniform->texture!=nullptr) {
        color.r=uniform->texture->at(in.uv.x,in.uv.y).r;
        color.g=uniform->texture->at(in.uv.x,in.uv.y).g;
        color.b=uniform->texture->at(in.uv.x,in.uv.y).b;
        color.a=uniform->texture->at(in.uv.x,in.uv.y).a;
    }
    out->color = color;
    return out;
}

glm::vec4 DefaultPipeline::fragmentShader(void *inData, const void *uniformData) {
    auto* in=static_cast<FragmentAttrib *>(inData);
    //没做混合，直接覆盖
    glm::vec4 out;
    out.r=in->color.r;
    out.g=in->color.g;
    out.b=in->color.b;
    out.a=in->color.a;
    return out;
}

void *ComposePipeline::vertexShader(void *inData, const void *uniformData) {
    //转换为具体类型
    auto uniform=static_cast<const Uniform*>(uniformData);
    VertexAttrib in=*static_cast<VertexAttrib *>(inData);
    auto out=new VertexShaderOutput();

    return out;
}

glm::vec4 ComposePipeline::fragmentShader(void *inData, const void *uniformData) {
    auto* in=static_cast<FragmentAttrib *>(inData);
    glm::vec4 out;

    return out;
}

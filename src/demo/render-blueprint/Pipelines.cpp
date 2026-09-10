//
// Created by abc17 on 2026/9/6.
//

#include "Pipelines.h"

VertexShaderOutput *DefaultPipeline::vertexShader(const VertexAttrib *in, const Uniform *uniform) {
    auto out= new VertexShaderOutput();
    //把顶点从模型空间换到裁剪空间直接到ndc空间
    glm::vec3 ndcPos = uniform->mvpMatrix * glm::vec3(in->pos.x, in->pos.y, 1.0f);
    out->pos=ndcPos;
    out->uv=glm::vec2(in->uv.x, in->uv.y);
    glm::vec4 color={1,1,1,1};
    out->color = color;
    return out;
}

glm::vec4 DefaultPipeline::fragmentShader(const FragmentAttrib *in, const Uniform *uniform) {
    glm::vec4 out;
    if (uniform->texture!=nullptr) {
    //采样
        glm::vec4 srcColor=uniform->texture->at(in->uv.x*uniform->texture->width,(in->uv.y)*uniform->texture->height);
        out.r=srcColor.r;
        out.g=srcColor.g;
        out.b=srcColor.b;
        out.a=srcColor.a;
        // std::cout<<in->viewPos.x<<","<<in->viewPos.y<<" uv:"<<in->uv.x<<","<<in->uv.y<<std::endl;
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
    out->pos=in->pos;
    out->uv=glm::vec2(in->uv.x, in->uv.y);
    out->color=in->color;
    return out;
}

glm::vec4 ComposePipeline::fragmentShader(const FragmentAttrib *in, const Uniform *uniform) {
    glm::vec4 out;
    //混合
    out=in->color;
    for (ColorBuffer* src:uniform->textures) {
        glm::vec4 srcColor=src->at(in->uv.x*src->width,(in->uv.y)*src->height);
        float a=1-(1-out.a)*(1-srcColor.a);
        out.r=(srcColor.r*srcColor.a+out.r*(1-out.a))/a;
        out.g=(srcColor.g*srcColor.a+out.g*(1-out.a))/a;
        out.b=(srcColor.b*srcColor.a+out.b*(1-out.a))/a;
        out.a=a;
    }
    return out;
}

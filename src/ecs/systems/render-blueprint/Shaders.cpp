//
// Created by abc17 on 2026/9/6.
//

#include "Shaders.h"

#include "RenderComponents.h"
#include "Render2DProcess.h"
#include "RenderCompositorProcess.h"


VertexShaderOutput *Default2DShader::vertexShader(const std::any& in, const Uniform *uniformBase) {
    auto out= new VertexShaderOutput();
    auto vert = std::any_cast<VertexAttrib2D>(in);
    auto* uniform=(Uniform2D*)uniformBase;
    //把顶点从模型空间换到裁剪空间直接到ndc空间
    //列向量约定
    glm::vec3 ndcPos = uniform ->mvpMatrix * glm::vec3(vert.pos.x, vert.pos.y, 1.0f);
    out->pos={ndcPos.x, ndcPos.y, 0, 1};
    out->uv=glm::vec2(vert.uv.x, vert.uv.y);
    out->color = {1,1,1,1};
    return out;
}

glm::vec4 Default2DShader::fragmentShader(const FragmentAttrib *in, const Uniform *uniformBase) {
    glm::vec4 out;
    auto* uniform=(Uniform2D*)uniformBase;
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

VertexShaderOutput *Default3DShader::vertexShader(const std::any &in, const Uniform *uniformBase) {
    auto out= new VertexShaderOutput();
    auto vert = std::any_cast<VertexAttrib3D>(in);
    auto* uniform=(Uniform3D*)uniformBase;
    //把顶点从模型空间换到裁剪空间直接到ndc空间
    glm::vec4 vScreenPos = uniform ->mvpMatrix * glm::vec4(vert.pos.x, vert.pos.y,vert.pos.z, 1.0f);
    glm::vec4 ndcPos={vScreenPos.x/vScreenPos.w,vScreenPos.y/vScreenPos.w,vScreenPos.z/vScreenPos.w,1};
    out->pos={ndcPos.x, ndcPos.y, ndcPos.z,ndcPos.w};
    out->uv=glm::vec2(vert.uv.x, vert.uv.y);
    out->color = {1,1,1,1};
    return out;
}

glm::vec4 Default3DShader::fragmentShader(const FragmentAttrib *in, const Uniform *uniform) {
    glm::vec4 out;
    out.r=in->color.r;
    out.g=in->color.g;
    out.b=in->color.b;
    out.a=in->color.a;
    return out;
}

VertexShaderOutput *ComposeShader::vertexShader(const std::any& in, const Uniform *uniform) {
    auto out=new VertexShaderOutput();
    auto vert=std::any_cast<VertexCompose>(in);
    out->pos={vert.pos.x,vert.pos.y,0,1};
    out->uv=vert.uv;
    out->color={1,1,1,1};
    return out;
}

glm::vec4 ComposeShader::fragmentShader(const FragmentAttrib* in, const Uniform *uniformBase) {
    glm::vec4 out;
    auto* uniform=(UniformCompositor*)uniformBase;
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

//
// Created by abc17 on 2026/8/12.
//

#include "RenderCompositorProcess.h"

#include "Shaders.h"


void RenderCompositorProcess::registerPipelines() {

    auto pipeline=new ComposeShader();
    gpu->pipelines["compose_pipeline"]=pipeline;
}

void RenderCompositorProcess::initBuffers() {

    gpu->vert_buffers.push_back(new VertexBuffer());
    this->vert_buffer_index=gpu->vert_buffers.size()-1;
    gpu->index_buffers.push_back(new IndiceBuffer());
    this->index_buffer_index=gpu->index_buffers.size()-1;
    gpu->uniform_buffers.push_back(new UniformBuffer());
    this->uniform_buffer_index=gpu->uniform_buffers.size()-1;
}

void RenderCompositorProcess::uploadData() {

    //合成器顶点：屏幕矩形
    renderContext->composeVertStart= gpu->vert_buffers[this->vert_buffer_index]->vertices.size();
    VertexCompose vert0={};
    //直接填ndc坐标，也是先x后y
    vert0.pos={-1,1};//左上
    vert0.uv={0,0};
    gpu->vert_buffers[this->vert_buffer_index]->vertices.push_back(vert0);
    VertexCompose vert1={};
    vert1.pos={1,1};//右上
    vert1.uv={1,0};
    gpu->vert_buffers[this->vert_buffer_index]->vertices.push_back(vert1);
    VertexCompose vert2={};
    vert2.pos={1,-1};//右下
    vert2.uv={1,1};
    gpu->vert_buffers[this->vert_buffer_index]->vertices.push_back(vert2);
    VertexCompose vert3={};
    vert3.pos={-1,-1};//左下
    vert3.uv={0,1};
    gpu->vert_buffers[this->vert_buffer_index]->vertices.push_back(vert3);
    //indice
    renderContext->composeIndexStart= gpu->index_buffers[this->index_buffer_index]->indices.size();
    auto index0=glm::ivec3(0,1,2);
    auto index1=glm::ivec3(0,2,3);
    gpu->index_buffers[this->index_buffer_index]->indices.push_back(index0);
    gpu->index_buffers[this->index_buffer_index]->indices.push_back(index1);
    //uniform
    auto uniform=new UniformCompositor();
    for (ColorBuffer* src:srcs) {
        uniform->textures.push_back(src);
    }
    gpu->uniform_buffers[this->uniform_buffer_index]->uniforms.push_back(uniform);

}

void RenderCompositorProcess::draw() {
    target->clear();

    RenderPass compose_render_pass{gpu->swapchain_texture,this->vert_buffer_index,this->index_buffer_index,this->uniform_buffer_index};
    compose_render_pass.cur_shader=gpu->pipelines["compose_pipeline"];

    gpu->drawcall(compose_render_pass,renderContext->composeIndexStart,2,renderContext->composeVertStart,0);
}

void RenderCompositorProcess::endFrame() {

    gpu->vert_buffers[this->vert_buffer_index]->vertices.clear();
    gpu->index_buffers[this->index_buffer_index]->indices.clear();
    gpu->uniform_buffers[this->uniform_buffer_index]->uniforms.clear();
    //合成器的目标通常不变，不用释放
}

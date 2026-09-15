//
// Created by abc17 on 2026/8/12.
//

#include "RenderCompositorProcess.h"

#include "Pipelines.h"

void RenderCompositorProcess::start() {
    auto pipeline=new ComposePipeline();
    gpu->pipelines["compose_pipeline"]=pipeline;
}

void RenderCompositorProcess::draw() {
    target->clear();
    auto uniform=new UniformCompositor();
    for (ColorBuffer* src:srcs) {
        uniform->textures.push_back(src);
    }
    RenderPass compose_render_pass{gpu->swapchain_texture};
    compose_render_pass.cur_pipeline=gpu->pipelines["compose_pipeline"];
    gpu->drawcall(compose_render_pass,renderContext->composeIndexStart,2,renderContext->composeVertStart,-1);
}

//
// Created by abc17 on 2026/8/12.
//

#include "RenderCompositorSystem.h"

#include "Pipelines.h"

void RenderCompositorSystem::start() {
    auto pipeline=new ComposePipeline();
    gpu->pipelines["compose_pipeline"]=pipeline;
}

void RenderCompositorSystem::draw() {
    target->clear();
    auto uniform=new Uniform();
    for (ColorBuffer* src:srcs) {
        uniform->textures.push_back(src);
    }
    const RenderPass compose_render_pass{"compose_pipeline",gpu->swapchain_texture};
    gpu->cur_renderpass=compose_render_pass;
    gpu->drawcall(renderContext->composeIndexStart,2,renderContext->composeVertStart,uniform);
}

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
    std::vector<ColorBuffer*>cbs;
    for (const std::string& name:src_names) {
        cbs.push_back(gpu->render_targets[name]);
    }
    auto uniform=new ComposeUniform {cbs};
    // gpu->drawcall();
}

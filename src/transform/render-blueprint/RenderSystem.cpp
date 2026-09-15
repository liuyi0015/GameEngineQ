//
// Created by XL0002 on 2026/9/15.
//

#include "RenderSystem.h"

#include "Pipelines.h"
#include "Render2DProcess.h"
#include "RenderContext.h"
#include "UploadProcess.h"

RenderSystem::RenderSystem(ecs::Scene *scene): System(scene){

    gpu=ApplicationContext::getInstance().get<SoftGPU*>("mygpu");
    //就用默认的
    auto pipeline=new Default2DPipeline();
    gpu->pipelines["default_pipeline"]=pipeline;
    //让其他类也可以通过名字访问target
    // gpu->render_targets[this->targetName]=target;
    renderContext=new RenderContext();
    uploadProcess=new UploadProcess (scene,renderContext);
    render2dProcess=new Render2DProcess (scene,renderContext);
    renderCompositorProcess=new RenderCompositorProcess (scene,renderContext);
}

void RenderSystem::start() {

}


void RenderSystem::draw() {
    //upload
    uploadProcess->draw();
    //2d
    render2dProcess->draw();
    //3d
    //ui
    //compositor
    renderCompositorProcess->draw();

    gpu->present();
    //todo delete uniform;每帧创建可受不了
}

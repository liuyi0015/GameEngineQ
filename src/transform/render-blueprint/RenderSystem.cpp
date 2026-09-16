//
// Created by XL0002 on 2026/9/15.
//

#include "RenderSystem.h"

#include "Pipelines.h"
#include "Render2DProcess.h"
#include "RenderContext.h"
#include "../../core/ecs/Util.h"
#include "../transform2d/Transform2dComponents.h"

RenderSystem::RenderSystem(ecs::Scene *scene): System(scene){

    gpu=ApplicationContext::getInstance().get<SoftGPU*>("mygpu");
    //让其他类也可以通过名字访问target
    // gpu->render_targets[this->targetName]=target;
    renderContext=new RenderContext();
    render2dProcess=new Render2DProcess (scene,renderContext);
    renderCompositorProcess=new RenderCompositorProcess (scene,renderContext,gpu->swapchain_texture);
}

void RenderSystem::start() {
    render2dProcess->initBuffers();
    renderCompositorProcess->initBuffers();
    render2dProcess->registerPipelines();
    renderCompositorProcess->registerPipelines();
    Entity cameraEntity=ecs::searchEntity<Camera2DComp>(scene)[0];
    ColorBuffer* target2d=ecs::getComponent<Camera2DComp>(scene,cameraEntity).value().target;
    renderCompositorProcess->srcs.push_back(target2d);
}


void RenderSystem::draw() {
    //2d
    render2dProcess->uploadData();
    render2dProcess->draw();
    render2dProcess->endFrame();
    //3d
    //ui
    //compositor
    renderCompositorProcess->uploadData();
    renderCompositorProcess->draw();
    renderCompositorProcess->endFrame();

    gpu->present();

}

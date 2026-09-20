//
// Created by XL0002 on 2026/9/15.
//

#include "RenderSystem.h"

#include "RenderComponents.h"
#include "../../Util.h"
#include "../../../core/Context.hpp"

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
}


void RenderSystem::draw() {
    renderCompositorProcess->srcs.clear();
    Entity cameraEntity=ecs::searchEntity<Camera2DComp>(scene)[0];
    ColorBuffer* target2d=ecs::getComponent<Camera2DComp>(scene,cameraEntity).value().target;
    renderCompositorProcess->srcs.push_back(target2d);
    //2d
    render2dProcess->uploadData();
    render2dProcess->draw();
    render2dProcess->endFrame();
    //3d
    //ui
    //合成器
    renderCompositorProcess->uploadData();
    renderCompositorProcess->draw();
    renderCompositorProcess->endFrame();

    gpu->present();

}

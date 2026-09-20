//
// Created by XL0002 on 2026/9/15.
//

#include "RenderSystem.h"

#include "Render3DProcess.h"
#include "RenderComponents.h"
#include "../../Util.h"
#include "../../../core/Context.hpp"

RenderSystem::RenderSystem(ecs::Scene *scene): System(scene){

    gpu=ApplicationContext::getInstance().get<SoftGPU*>("mygpu");
    //让其他类也可以通过名字访问target
    // gpu->render_targets[this->targetName]=target;
    renderContext=new RenderContext();
    render2dProcess=new Render2DProcess (scene,renderContext);
    render3dProcess=new Render3DProcess(scene,renderContext);
    renderCompositorProcess=new RenderCompositorProcess (scene,renderContext,gpu->swapchain_texture);
}

void RenderSystem::start() {
    render2dProcess->initBuffers();
    render2dProcess->registerPipelines();

    render3dProcess->initBuffers();
    render3dProcess->registerPipelines();

    renderCompositorProcess->initBuffers();
    renderCompositorProcess->registerPipelines();
}


void RenderSystem::draw() {
    renderCompositorProcess->srcs.clear();
    for (Entity camera2d:ecs::searchEntity<Camera2DComp>(scene)) {
        ColorBuffer* target=ecs::getComponent<Camera2DComp>(scene,camera2d).value().target;
        renderCompositorProcess->srcs.push_back(target);
    }
    for (Entity camera3d:ecs::searchEntity<Camera3DComp>(scene)) {
        ColorBuffer* target=ecs::getComponent<Camera3DComp>(scene,camera3d).value().target;
        renderCompositorProcess->srcs.push_back(target);
    }
    //2d
    render2dProcess->uploadData();
    render2dProcess->draw();
    render2dProcess->endFrame();
    //3d
    render3dProcess->uploadData();
    render3dProcess->draw();
    render3dProcess->endFrame();
    //ui
    //合成器
    renderCompositorProcess->uploadData();
    renderCompositorProcess->draw();
    renderCompositorProcess->endFrame();

    gpu->present();

}
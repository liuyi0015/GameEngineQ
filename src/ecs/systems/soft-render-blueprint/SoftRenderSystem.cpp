//
// Created by XL0002 on 2026/9/15.
//

#include "SoftRenderSystem.h"

#include "Render3DProcess.h"
#include "RenderComponents.h"
#include "../../Util.h"
#include "../../../core/Context.hpp"

SoftRenderSystem::SoftRenderSystem(){

    gpu=ApplicationContext::getInstance().get<SoftGPU*>("mygpu");
    //让其他类也可以通过名字访问target
    // gpu->render_targets[this->targetName]=target;
    renderContext=new RenderContext();
    render2dProcess=new Render2DProcess (renderContext);
    render3dProcess=new Render3DProcess(renderContext);
    renderCompositorProcess=new RenderCompositorProcess (renderContext,gpu->swapchain_texture);
    render2dProcess->initBuffers();
    render2dProcess->registerPipelines();

    render3dProcess->initBuffers();
    render3dProcess->registerPipelines();

    renderCompositorProcess->initBuffers();
    renderCompositorProcess->registerPipelines();
}
//场景加载时调用
void SoftRenderSystem::start() {
}


void SoftRenderSystem::draw() {
    renderCompositorProcess->srcs.clear();
    for (Entity camera2d:ecs::searchEntity<Camera2DComp>(scene)) {
        ColorBuffer* target=ecs::getComponent<Camera2DComp>(scene,camera2d)->target;
        renderCompositorProcess->srcs.push_back(target);
    }
    for (Entity camera3d:ecs::searchEntity<Camera3DComp>(scene)) {
        ColorBuffer* target=ecs::getComponent<Camera3DComp>(scene,camera3d)->target;
        renderCompositorProcess->srcs.push_back(target);
    }
    //2d
    render2dProcess->uploadData(scene);
    render2dProcess->draw(scene);
    render2dProcess->endFrame();
    //3d
    render3dProcess->uploadData(scene);
    render3dProcess->draw(scene);
    render3dProcess->endFrame();
    //ui
    //合成器
    renderCompositorProcess->uploadData();
    renderCompositorProcess->draw();
    renderCompositorProcess->endFrame();

    gpu->present();

}
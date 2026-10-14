//
// Created by XL0002 on 2026/9/15.
//

#include "SoftRenderSystem.h"

#include "Render3DProcess.h"
#include "RenderComponents.h"
#include "../../Util.h"
#include "../../../core/Context.hpp"
#include "../../comps/RenderComponents.h"

SoftRenderSystem::SoftRenderSystem(){
    SDL_Window* window=ApplicationContext::getInstance().get<SDL_Window*>("window");
    // 创建渲染器
    gpu=new SoftGPU(window);
    //让其他类也可以通过名字访问target
    // gpu->render_targets[this->targetName]=target;
    renderContext=new RenderContext();
    render2dProcess=new Render2DProcess (renderContext,gpu);
    render3dProcess=new Render3DProcess(renderContext,gpu);
    renderCompositorProcess=new RenderCompositorProcess (renderContext,gpu->swapchain_texture,gpu);
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
    //2d
    render2dProcess->uploadData(scene);
    for (Entity camera2d:ecs::searchEntity<Camera2DComp>(scene)) {
        std::string targetName=ecs::getComponent<Camera2DComp>(scene,camera2d)->targetName;
        ColorBuffer* target=targets[targetName];
        renderCompositorProcess->srcs.push_back(target);
        render2dProcess->draw(scene,target,camera2d);
    }
    render2dProcess->endFrame();
    //3d
    render3dProcess->uploadData(scene);
    for (Entity camera3d:ecs::searchEntity<Camera3DComp>(scene)) {
        std::string targetName=ecs::getComponent<Camera3DComp>(scene,camera3d)->targetName;
        ColorBuffer* target=targets[targetName];
        renderCompositorProcess->srcs.push_back(target);
        render3dProcess->draw(scene,target,camera3d);
    }
    render3dProcess->endFrame();
    //ui
    //合成器
    renderCompositorProcess->uploadData();
    renderCompositorProcess->draw();
    renderCompositorProcess->endFrame();

    gpu->present();

}
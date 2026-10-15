
//
// Created by XL0002 on 2026/9/29.
//

#include "Render3DPass.h"
#include "SdlGpuRenderSystem.h"
#include "../../../ecs/comps/RenderComponents.h"
#include "../../Util.h"
#include <cassert>

#include "../../../core/Context.hpp"
SdlGpuRenderSystem::SdlGpuRenderSystem() {

    device=SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV,true,"vulkan");
    assert(device!=nullptr);
    window=ApplicationContext::getInstance().get<SDL_Window*>("window");
    SDL_ClaimWindowForGPUDevice(device,window);
    render3dPass=new Render3DPass(device,window);
    upload3dPass=new Upload3DPass(device);
}

void SdlGpuRenderSystem::start() {
    assert(scene!=nullptr);
    upload3dPass->uploadData(scene, drawableMeshes);
}
void SdlGpuRenderSystem::draw() {
    assert(scene!=nullptr);
    // 创建命令缓冲区
    SDL_GPUCommandBuffer* commandBuffer = SDL_AcquireGPUCommandBuffer(device);
    assert(commandBuffer != nullptr);
    // 获取下一张交换链纹理
    unsigned int swapchainTextureWidth, swapchainTextureHeight;
    SDL_AcquireGPUSwapchainTexture(commandBuffer, window, &targets["swapchain"], &swapchainTextureWidth, &swapchainTextureHeight);
    if (targets["swapchain"] == nullptr) {
        //这里不能抛异常退出，因为窗口最小化就会进入这里
        // SDL_Log("没有交换链纹理！");
        return;
    }
    if (ecs::searchEntity<Camera3DComp>(scene).empty())return;
    //目前只拿第一个摄像机
    Entity camera=ecs::searchEntity<Camera3DComp>(scene)[0];
    auto* cameraComp=ecs::getComponent<Camera3DComp>(scene,camera);
    render3dPass->draw(scene,targets[cameraComp->targetName],commandBuffer,drawableMeshes,camera);

    // 提交命令缓冲区
    SDL_SubmitGPUCommandBuffer(commandBuffer);
}

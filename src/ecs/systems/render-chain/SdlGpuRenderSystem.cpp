#include "Render3DPass.h"
#include "SdlGpuRenderSystem.h"

#include <cassert>
//
// Created by XL0002 on 2026/9/29.
//
SdlGpuRenderSystem::SdlGpuRenderSystem(SDL_GPUDevice* device, SDL_Window *window) :device(device), window(window) {
    render3dPass=new Render3DPass(device,window);
}

void SdlGpuRenderSystem::start() {
    assert(scene!=nullptr);
    render3dPass->uploadData(scene);
}
void SdlGpuRenderSystem::draw() {
    assert(scene!=nullptr);
    // 创建命令缓冲区
    SDL_GPUCommandBuffer* commandBuffer = SDL_AcquireGPUCommandBuffer(device);
    assert(commandBuffer != nullptr);
    // 获取下一张交换链纹理
    SDL_GPUTexture* swapchainTexture;
    unsigned int swapchainTextureWidth, swapchainTextureHeight;
    SDL_AcquireGPUSwapchainTexture(commandBuffer, window, &swapchainTexture, &swapchainTextureWidth, &swapchainTextureHeight);
    if (swapchainTexture == nullptr) {
        //这里不能抛异常退出，因为窗口最小化就会进入这里
        // SDL_Log("没有交换链纹理！");
    }
    // 让render3dpass把场景画到交换链纹理上
    render3dPass->draw(scene,swapchainTexture,commandBuffer,drawableMeshes);

    // 提交命令缓冲区
    SDL_SubmitGPUCommandBuffer(commandBuffer);
}

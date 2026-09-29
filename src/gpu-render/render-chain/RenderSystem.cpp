#include "Render3DPass.h"
#include "RenderSystem.h"
//
// Created by XL0002 on 2026/9/29.
//
RenderSystem::RenderSystem()  {
    render3dProcess=new Render3DPass(scene);
}

void RenderSystem::start() {
    render3dProcess->registerPipelines();
}

void RenderSystem::draw() {

}

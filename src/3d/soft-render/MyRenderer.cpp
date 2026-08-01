//
// Created by XL0002 on 2026/7/30.
//
#include "MyRenderer.h"

#include <vector>

#include "../../Context.hpp"
#define INIT_RENDER_WIDTH 800
#define INIT_RENDER_HEIGHT 600
MyRenderer::MyRenderer() {
    renderer=ApplicationContext::getInstance().get<SDL_Renderer*>("renderer");
    width=INIT_RENDER_WIDTH;
    height=INIT_RENDER_HEIGHT;
    texture=SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
}

void MyRenderer::render(double verts[][3], int triangles[][3]) {
    // RGBA pixel buffer // z-buffer
    for (int i=0;i<width*height;i++) {
        pixels[i]=0;
        depthBuffer[i]=1.0f;
    }
    //计算pixels到底是什么

    // 上传像素并一次性绘制
    SDL_UpdateTexture(texture, NULL, pixels.data(), width * sizeof(Uint32));
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
}


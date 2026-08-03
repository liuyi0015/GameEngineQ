//
// Created by XL0002 on 2026/7/30.
//
#include "MyRenderer2D.h"

#include <memory>
#include <vector>

#include "../../Context.hpp"
#define INIT_RENDER_WIDTH 800
#define INIT_RENDER_HEIGHT 600
MyRenderer2D::MyRenderer2D() {
    renderer=ApplicationContext::getInstance().get<SDL_Renderer*>("renderer");
    width=INIT_RENDER_WIDTH;
    height=INIT_RENDER_HEIGHT;
    SDL_Texture* texture=SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    auto texture_ptr = std::shared_ptr<SDL_Texture>(texture, SDL_DestroyTexture);
    ApplicationContext::getInstance().set("targetTexture", texture);
}

void MyRenderer2D::render(double verts[][2], int triangles[][3]) {
    // RGBA pixel buffer
    for (int i=0;i<width*height;i++) {
        pixels[i]=0;
    }
    //计算pixels到底是什么

    // 上传像素并绘制
    auto targetTexture=ApplicationContext::getInstance().get<std::shared_ptr<SDL_Texture>>("targetTexture");
    SDL_SetRenderTarget(renderer, targetTexture.get());
    SDL_Texture* texture=SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    SDL_UpdateTexture(texture, NULL, pixels.data(), width * sizeof(Uint32));
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_SetRenderTarget(renderer,NULL);
    //记得target画上屏幕去
}


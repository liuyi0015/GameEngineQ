//
// Created by XL0002 on 2026/8/11.
//

#include "UIRenderSystem.h"

void UIRenderSystem::draw() {
    SDL_SetRenderTarget(renderer,canvas);
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);
    SDL_SetRenderTarget(renderer, nullptr);
}

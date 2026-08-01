//
// Created by XL0002 on 2026/7/22.
//

#include "RenderUtil.h"

#include <cmath>

void RenderUtil::drawCircle(SDL_Renderer *renderer, float x, float y, float radius, const Color &color, int segments) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // 使用多个线段近似圆形

    for (int i = 0; i < segments; ++i) {
        float angle1 = 2.0f * M_PI * i / segments;
        float angle2 = 2.0f * M_PI * (i + 1) / segments;
        float x1 = x + std::cos(angle1) * radius;
        float y1 = y + std::sin(angle1) * radius;
        float x2 = x + std::cos(angle2) * radius;
        float y2 = y + std::sin(angle2) * radius;

        SDL_RenderLine(renderer, x1, y1, x2, y2);
    }
}

void RenderUtil::drawImage(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_FRect dstRect, const Color color,float angle) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_FPoint center = { dstRect.w / 2.0f, dstRect.h / 2.0f };
    SDL_RenderTextureRotated(renderer, texture, nullptr, &dstRect, angle, &center, SDL_FLIP_NONE);
}
void RenderUtil::drawRect(SDL_Renderer *renderer, const SDL_FRect rect, const Color &color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer,&rect);
}
void RenderUtil::drawText(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_FRect dstRect) {
    SDL_RenderTexture(renderer, texture, nullptr, &dstRect);
}
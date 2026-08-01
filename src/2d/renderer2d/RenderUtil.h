//
// Created by XL0002 on 2026/7/22.
//

#ifndef GAMEENGINE_RENDERUTIL_H
#define GAMEENGINE_RENDERUTIL_H
#include "SDL3/SDL_render.h"
#include "RenderComponents.h"

class RenderUtil {
    public:
    static void drawCircle(SDL_Renderer* renderer, float x, float y, float radius, const Color& color, int segments = 32);
    static void drawImage(SDL_Renderer *renderer, SDL_Texture *texture, SDL_FRect dstRect, Color color, float angle);
    static void drawRect(SDL_Renderer *renderer, const SDL_FRect rect, const Color &color);
    static void drawText(SDL_Renderer *renderer, SDL_Texture *texture, SDL_FRect dstRect);
};


#endif //GAMEENGINE_RENDERUTIL_H

//
// Created by XL0002 on 2026/8/17.
//

#ifndef GAMEENGINEQ_RENDER2DUTIL_H
#define GAMEENGINEQ_RENDER2DUTIL_H
#include "SDL3/SDL_render.h"
#include "glm/glm.hpp"

class Render2dUtil {
    public:
    // 使用多个线段近似圆形
    static void drawCircle(SDL_Renderer* renderer, const glm::mat3 mvpMatrix, float radius, const SDL_Color &color, int segments,float clipWidth, float clipHeight,float viewportW, float viewportH);
    static void drawTexture(SDL_Renderer *renderer, SDL_Texture *texture, glm::vec2 uvs[], SDL_Color color, const glm::mat3 &mvpMatrix, float
                            cameraWidth, float cameraHeight, float viewportW, float viewportH);
};


#endif //GAMEENGINEQ_RENDER2DUTIL_H

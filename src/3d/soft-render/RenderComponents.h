//
// Created by XL0002 on 2026/8/5.
//

#ifndef GAMEENGINE_RENDERCOMPONENTS_H
#define GAMEENGINE_RENDERCOMPONENTS_H
#include <vector>

#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_rect.h"

struct Position3D {
    float x;
    float y;
    float z;
};
struct Vertex3D{
    Position3D position;
    SDL_Color color;
    SDL_FPoint tex_coord;
};
struct DrawableFlag {
    SDL_Color color;
    std::vector<Vertex3D> vertices;
};
#endif //GAMEENGINE_RENDERCOMPONENTS_H

//
// Created by XL0002 on 2026/8/5.
//
#pragma once

#define GAMEENGINE_RENDERCOMPONENTS_H
#include <vector>

#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_rect.h"
#include "../transform3d/Transform3dComponents.h"
struct Vertex3D{
    Position3D position;
    SDL_Color color{255,255,255,255};
    SDL_FPoint tex_coord{0,0};
};
struct Drawable3DFlag {
    SDL_Color color;
    std::vector<Vertex3D> vertices;
};

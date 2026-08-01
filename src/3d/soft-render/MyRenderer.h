//
// Created by XL0002 on 2026/7/30.
//

#ifndef GAMEENGINE_MYRENDERER_H
#define GAMEENGINE_MYRENDERER_H
#include <vector>

#include "SDL3/SDL_render.h"


class MyRenderer {
private:
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    std::vector<Uint32>pixels;
    std::vector<double> depthBuffer;
    int width, height;
    public:
    MyRenderer();
    ~MyRenderer()=default;
    void render(double verts[][3],int triangles[][3]);
};


#endif //GAMEENGINE_MYRENDERER_H

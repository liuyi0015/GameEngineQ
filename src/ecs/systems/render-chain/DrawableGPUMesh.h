//
// Created by XL0002 on 2026/10/15.
//

#ifndef GAMEENGINEQ_DRAWABLEGPUMESH_H
#define GAMEENGINEQ_DRAWABLEGPUMESH_H
#include "SDL3/SDL_gpu.h"

//物体在GPU的资源位置
struct DrawableGPUMesh {
    SDL_GPUBuffer* vertexBuffer;
    size_t vertexCount;
    Uint32 vertexOffset;

    SDL_GPUBuffer* indexBuffer;
    size_t indexCount;
    Uint32 indexOffset;
};
#endif //GAMEENGINEQ_DRAWABLEGPUMESH_H

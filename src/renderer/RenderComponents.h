//
// Created by XL0002 on 2026/7/22.
//

#ifndef GAMEENGINE_RENDERCOMPONENTS_H
#define GAMEENGINE_RENDERCOMPONENTS_H
#include <string>

#include "SDL3/SDL_stdinc.h"

struct Color {
    Uint8 r = 255;
    Uint8 g = 255;
    Uint8 b = 255;
    Uint8 a = 255;
};

//标记组件
struct DrawableFlag {
    int z_order = 0;
    Color color;
};
struct CircleRendererFlag {
    float radius;
    // 分段数，越多越平滑
    int segments;
};
struct ImageRendererFlag {
    std::string resourceId;  // 资源ID，用于从ResourceManager获取纹理
    float width = 0.0f;
    float height = 0.0f;
};

struct RectRendererFlag {
    float width;
    float height;
};//标记
struct TextRendererFlag {
    std::string text;
    std::string fontResourceId;  // 字体资源ID
    std::string surfResourceId;
    std::string texResourceId;
};
#endif //GAMEENGINE_RENDERCOMPONENTS_H

//
// Created by XL0002 on 2026/7/24.
//

#ifndef GAMEENGINE_ANIMATIONCOMPONENTS_H
#define GAMEENGINE_ANIMATIONCOMPONENTS_H
#include <string>

#include "SDL3_image/SDL_image.h"
#include <vector>
struct FrameAnimationFlag {
    std::string animId;
    int counter=0;

    bool loop;
    double timer=0;
};
struct AnimationFlag {

    bool loop=true;
    double timer=0;
};
#endif //GAMEENGINE_ANIMATIONCOMPONENTS_H

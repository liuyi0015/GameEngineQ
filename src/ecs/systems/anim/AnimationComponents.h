//
// Created by XL0002 on 2026/7/24.
//

#ifndef GAMEENGINE_ANIMATIONCOMPONENTS_H
#define GAMEENGINE_ANIMATIONCOMPONENTS_H
#include <string>

#include "SDL3_image/SDL_image.h"
#include "AnimationClip.h"
#include <vector>
struct AnimationFlag {
    std::vector<AnimationClip>anims;
};
#endif //GAMEENGINE_ANIMATIONCOMPONENTS_H

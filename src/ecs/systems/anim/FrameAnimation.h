//
// Created by XL0002 on 2026/9/17.
//

#ifndef GAMEENGINEQ_FRAMEANIMATOR_H
#define GAMEENGINEQ_FRAMEANIMATOR_H
#include <string>

#include "AnimationComponents.h"
#include "../render-blueprint/RenderComponents.h"
#include "SDL3_image/SDL_image.h"



class FrameAnimation {
public:
    FrameAnimation( FrameAnimationFlag frameAnimatorFlag,const Drawable2DFlag& drawableFlag);
    FrameAnimationFlag update(double dt);

    FrameAnimationFlag frameAnimatorFlag;
    Drawable2DFlag drawableFlag;
};

#endif //GAMEENGINEQ_FRAMEANIMATOR_H

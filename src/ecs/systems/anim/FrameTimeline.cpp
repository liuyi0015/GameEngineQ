//
// Created by XL0002 on 2026/9/17.
//

#include "FrameTimeline.h"
#include "../render-blueprint/RenderComponents.h"
#include "../../../core/ResourceManager.hpp"
#include "AnimationComponents.h"
#include "../../Util.h"

FrameTimeline::FrameTimeline(std::string animId, SDL_Surface** drawableCompTarget) : target(drawableCompTarget) {
    anim = ResourceManager::getInstance().getAnimationCache().get(animId);
    double time = 0;
    for (int i = 0; i < anim->count; i++) {
        frames.push_back({time, anim->frames[i]});
        time += anim->delays[i];
    }
}

FrameTimeline::~FrameTimeline() {
    // IMG_FreeAnimation(anim);
}

void FrameTimeline::updateValue(double timer,double duration) {
    int i=0;
    while (i<frames.size()&&timer>frames[i].first) {
        i++;
    }
    //todo边界处理
    //动画surface*传给绘制surface*
    *target=frames[i].second;
}

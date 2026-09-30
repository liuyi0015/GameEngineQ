//
// Created by XL0002 on 2026/9/18.
//

#ifndef GAMEENGINEQ_ANIMATIONCLIP_H
#define GAMEENGINEQ_ANIMATIONCLIP_H
#include <memory>

#include "Timeline.h"


class AnimationClip {
    public:
    bool loop=true;
    double duration=0;
    double timer=0;
    //不得不用指针，不然切片，但又要托管释放，所以用智能指针
    std::vector<std::shared_ptr<Timeline>> timelines;
    void update(double dt);
};


#endif //GAMEENGINEQ_ANIMATIONCLIP_H

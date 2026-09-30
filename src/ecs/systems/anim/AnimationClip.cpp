//
// Created by XL0002 on 2026/9/18.
//

#include "AnimationClip.h"

void AnimationClip::update(double dt) {
    timer+=dt;
    while (loop && timer>duration) {
        timer-=duration;
    }
    for (auto& timeline: timelines) {
        timeline->updateValue(timer,duration);
    }
}

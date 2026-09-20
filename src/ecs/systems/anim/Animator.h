//
// Created by XL0002 on 2026/9/18.
//

#ifndef GAMEENGINEQ_ANIMATOR_H
#define GAMEENGINEQ_ANIMATOR_H
#include <vector>

#include "AnimationClip.h"


class Animator {
    public:
    std::vector<AnimationClip*>anims;
    void update(double dt);
};


#endif //GAMEENGINEQ_ANIMATOR_H

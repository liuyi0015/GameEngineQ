//
// Created by XL0002 on 2026/7/24.
//

#ifndef GAMEENGINE_ANIMATIONCOMPONENTS_H
#define GAMEENGINE_ANIMATIONCOMPONENTS_H
#include <string>

struct FrameAnimatorFlag {
    std::string name;
    std::string path;
    bool loop;
    int counter=0;
    double timer=0;
};
#endif //GAMEENGINE_ANIMATIONCOMPONENTS_H

//
// Created by XL0002 on 2026/9/17.
//

#ifndef GAMEENGINEQ_FRAMEANIMATOR_H
#define GAMEENGINEQ_FRAMEANIMATOR_H
#include <string>

#include "AnimationComponents.h"
#include "Timeline.h"
#include "../render-blueprint/RenderComponents.h"
#include "SDL3_image/SDL_image.h"



class FrameTimeline:public Timeline {
private:
    IMG_Animation * anim;
public:
    //仅持有指针，数据在anim里，析构时释放，所以提取anim为字段
    std::vector<std::pair<double,SDL_Surface*> >frames;
    //右const表示总是指向drawable组件的surface指针而可以修改目标指针的指向
    SDL_Surface** const target;
    explicit FrameTimeline(std::string animId, SDL_Surface** drawableCompTarget);
    ~FrameTimeline() override;
    void updateValue(double timer,double duration) override;

};

#endif //GAMEENGINEQ_FRAMEANIMATOR_H

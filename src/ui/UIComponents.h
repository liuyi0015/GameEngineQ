//
// Created by XL0002 on 2026/7/24.
//

#ifndef GAMEENGINE_UICOMPONENTS_H
#define GAMEENGINE_UICOMPONENTS_H
#include <string>
#include <vector>

#include "SDL3/SDL_rect.h"

//响应事件不是输入事件
struct LeftMouseColliderEvents {
    std::string pressed_event;
    std::string released_event;
    std::string clicked_event;
};
struct RightMouseColliderEvents {
    std::string pressed_event;
    std::string released_event;
    std::string clicked_event;
};
struct WheelMouseColliderEvents {
    std::string scroll_event;
    std::string pressed_event;
    std::string released_event;
    std::string clicked_event;
};
struct MoveMouseColliderEvents {
    std::string move_event;
    std::string drag_event;
    std::string hover_event;
    std::string unhover_event;
};
struct MouseEventParam {
    float x,y;
};
struct Triangle {
    SDL_FPoint p1,p2,p3;
    bool contains(const SDL_FPoint& p)const {
        auto sign = [](const SDL_FPoint &p1, const SDL_FPoint &p2, const SDL_FPoint &p3){
            return (p1.x - p3.x)*(p2.y - p3.y) - (p2.x - p3.x)*(p1.y - p3.y);
        };
        bool b1 = sign(p,p1,p2) < 0.0f;
        bool b2 = sign(p,p2,p3) < 0.0f;
        bool b3 = sign(p,p3,p1) < 0.0f;
        return (b1==b2) && (b2==b3);
    }
    //面积
    [[nodiscard]] float area(const SDL_FPoint& point)const {
        return std::abs(p1.x*(p2.y-p3.y)+p2.x*(p3.y-p1.y)+p3.x*(p1.y-p2.y)/2.0f);
    }
};
struct MouseColliderFlag {
    std::vector<Triangle> triangles;
    int order;
    bool block;
};
struct CanvasComp {
    int width;
    int height;
};
struct UIFlag{

};
#endif //GAMEENGINE_UICOMPONENTS_H

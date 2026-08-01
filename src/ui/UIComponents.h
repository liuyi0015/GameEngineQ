//
// Created by XL0002 on 2026/7/24.
//

#ifndef GAMEENGINE_UICOMPONENTS_H
#define GAMEENGINE_UICOMPONENTS_H
#include <string>

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
    std::string up_event;
    std::string down_event;
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
struct MouseColliderFlag {
    float w;
    float h;
    int order;
    bool block;
};

#endif //GAMEENGINE_UICOMPONENTS_H

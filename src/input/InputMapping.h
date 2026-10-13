//
// Created by XL0002 on 2026/10/8.
//

#ifndef GAMEENGINEQ_MOUSEINPUTMAPPING_H
#define GAMEENGINEQ_MOUSEINPUTMAPPING_H
#include "SDL3/SDL_events.h"

class InputMapping {
private:
    static void publishMouseButtonEvent(const SDL_MouseButtonEvent &event);

    static void publishMouseWheelEvent(const SDL_MouseWheelEvent& event);

    static void publishMouseMotionEvent(const SDL_MouseMotionEvent& event);
    static void publishKeyboardEvent(const SDL_KeyboardEvent &event);
public:
    static void handleInput(const SDL_Event& event);
};


#endif //GAMEENGINEQ_MOUSEINPUTMAPPING_H

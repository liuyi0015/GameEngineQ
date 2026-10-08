//
// Created by XL0002 on 2026/10/8.
//

#ifndef GAMEENGINEQ_MOUSEINPUTMAPPING_H
#define GAMEENGINEQ_MOUSEINPUTMAPPING_H
#include "SDL3/SDL_events.h"


class MouseInputMapping {
public:
    static void leftMousePressed(const SDL_MouseButtonEvent& event);
    static void leftMouseReleased(const SDL_MouseButtonEvent& event);
    static void rightMousePressed(const SDL_MouseButtonEvent& event);
    static void rightMouseReleased(const SDL_MouseButtonEvent& event);
    static void middleMousePressed(const SDL_MouseButtonEvent& event);
    static void middleMouseReleased(const SDL_MouseButtonEvent& event);
    static void wheelScrollUp(const SDL_MouseWheelEvent& event);

    static void wheelScrollDown(const SDL_MouseWheelEvent &event);

    static void mouseMoved(const SDL_MouseMotionEvent& event);
};


#endif //GAMEENGINEQ_MOUSEINPUTMAPPING_H

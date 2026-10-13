//
// Created by XL0002 on 2026/10/8.
//

#include "InputMapping.h"

#include "../core/EventBus.h"

void InputMapping::publishMouseButtonEvent(const SDL_MouseButtonEvent &event) {
    EventBus::getInstance().publish("input.mousebutton", event);
}
void InputMapping::publishMouseWheelEvent(const SDL_MouseWheelEvent &event) {
    EventBus::getInstance().publish("input.mousewheel", event);
}
void InputMapping::publishMouseMotionEvent(const SDL_MouseMotionEvent &event) {
    EventBus::getInstance().publish("input.mousemotion", event);
}

void InputMapping::publishKeyboardEvent(const SDL_KeyboardEvent &event) {
    EventBus::getInstance().publish("input.keyboard", event);
}

void InputMapping::handleInput(const SDL_Event &event) {

    if (event.type==SDL_EVENT_MOUSE_BUTTON_DOWN) {
        publishMouseButtonEvent(event.button);
    }else if (event.type==SDL_EVENT_MOUSE_BUTTON_UP) {
        publishMouseButtonEvent(event.button);
    }else if (event.type==SDL_EVENT_MOUSE_MOTION) {
        publishMouseMotionEvent(event.motion);
    }else if (event.type==SDL_EVENT_MOUSE_WHEEL){
        publishMouseWheelEvent(event.wheel);
    }else if (event.type==SDL_EVENT_KEY_DOWN) {
        publishKeyboardEvent(event.key);
    }else if (event.type==SDL_EVENT_KEY_UP) {
        publishKeyboardEvent(event.key);
    }
}

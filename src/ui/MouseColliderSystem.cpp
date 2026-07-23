//
// Created by XL0002 on 2026/7/21.
//

#include "MouseColliderSystem.h"

#include "../EventDispatcher.h"
#include "../ecs/Scene.h"

void MouseColliderSystem::start() {
    EventDispatcher::getInstance().subscribe("left mouse pressed",[this](std::any param) {
        leftMousePressed(param);
    });
    EventDispatcher::getInstance().subscribe("left mouse released",[this](std::any param) {
        leftMouseReleased(param);
    });
    EventDispatcher::getInstance().subscribe("left mouse clicked",[this](std::any param) {
        leftMouseClicked(param);
    });
    EventDispatcher::getInstance().subscribe("right mouse pressed",[this](std::any param) {
        rightMousePressed(param);
    });
    EventDispatcher::getInstance().subscribe("right mouse released",[this](std::any param) {
        rightMouseReleased(param);
    });
    EventDispatcher::getInstance().subscribe("right mouse clicked",[this](std::any param) {
        rightMouseClicked(param);
    });
    EventDispatcher::getInstance().subscribe("wheel mouse pressed",[this](std::any param) {
        wheelMousePressed(param);
    });
    EventDispatcher::getInstance().subscribe("wheel mouse released",[this](std::any param) {
        wheelMouseReleased(param);
    });
    EventDispatcher::getInstance().subscribe("wheel mouse clicked",[this](std::any param) {
        wheelMouseClicked(param);
    });
    EventDispatcher::getInstance().subscribe("wheel mouse up",[this](std::any param) {
        wheelMouesUp(param);
    });
    EventDispatcher::getInstance().subscribe("wheel mouse down",[this](std::any param) {
        wheelMouseDown(param);
    });
    EventDispatcher::getInstance().subscribe("mouse moved",[this](std::any param) {
        mouseMoved(param);
    });
    EventDispatcher::getInstance().subscribe("mouse dragged",[this](std::any param) {
        mouseDragged(param);
    });


}


void MouseColliderSystem::leftMousePressed(const std::any&  param) {
    for (Entity entity:getSortedMouseColliders<LeftMouseColliderEvents>(scene)) {
        EventDispatcher::getInstance().publish(scene->getComponent<LeftMouseColliderEvents>(entity)
            .value().pressed_event,param);
        bool block=scene->getComponent<MouseColliderFlag>(entity).value().block;
        if (block)break;
    }
}

void MouseColliderSystem::leftMouseReleased(const std::any &param) {
}

void MouseColliderSystem::leftMouseClicked(const std::any &param) {
}

void MouseColliderSystem::rightMousePressed(const std::any &param) {
}

void MouseColliderSystem::rightMouseReleased(const std::any &param) {
}

void MouseColliderSystem::rightMouseClicked(const std::any &param) {
}

void MouseColliderSystem::wheelMouesUp(const std::any &param) {
}

void MouseColliderSystem::wheelMouseDown(const std::any &param) {
}

void MouseColliderSystem::wheelMousePressed(const std::any &any) {
}

void MouseColliderSystem::wheelMouseReleased(const std::any &any) {

}

void MouseColliderSystem::wheelMouseClicked(const std::any &any) {
}

void MouseColliderSystem::mouseMoved(const std::any &param) {
}

void MouseColliderSystem::mouseDragged(const std::any &param) {
}

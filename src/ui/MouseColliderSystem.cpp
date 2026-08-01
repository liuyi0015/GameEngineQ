//
// Created by XL0002 on 2026/7/21.
//

#include "MouseColliderSystem.h"

#include "../EventDispatcher.h"
#include "../ecs/BaseComponents.h"
#include "../ecs/Scene.h"
#include "../2d/transform2d/TransformUtil.h"
#include "../ecs/Util.h"

void MouseColliderSystem::start() {
    EventDispatcher::getInstance().subscribe("left mouse pressed",[this](std::any param) {
        leftMousePressed(param);
    });
    EventDispatcher::getInstance().subscribe("left mouse released",[this](std::any param) {
        leftMouseReleased(param);
    });
    EventDispatcher::getInstance().subscribe("right mouse pressed",[this](std::any param) {
        rightMousePressed(param);
    });
    EventDispatcher::getInstance().subscribe("right mouse released",[this](std::any param) {
        rightMouseReleased(param);
    });
    EventDispatcher::getInstance().subscribe("middle mouse pressed",[this](std::any param) {
        middleMousePressed(param);
    });
    EventDispatcher::getInstance().subscribe("middle mouse released",[this](std::any param) {
        middleMouseReleased(param);
    });
    EventDispatcher::getInstance().subscribe("mouse wheeled",[this](std::any param) {
        wheelMouse(param);
    });
    EventDispatcher::getInstance().subscribe("mouse moved",[this](std::any param) {
        mouseMoved(param);
    });

}

bool inRect(const float mx, const float my, const float x, const float y, const float w, const float h) {
    return mx >= x and mx <= x + w&&my >= y and my <= y + h;
}
void MouseColliderSystem::leftMousePressed(const std::any&  param) {
    for (Entity entity:getSortedMouseColliders<LeftMouseColliderEvents>(scene)) {
        auto mouseColliderFlag=ecs::getComponent<MouseColliderFlag>(scene,entity).value();
        auto transformComp=ecs::getComponent<Transform>(scene,entity).value();
        auto worldTransform=TransformUtil::computeLocalToWorldTransform(transformComp,scene);
        auto pos=std::any_cast<MouseEventParam>(param);
        if (inRect(pos.x,pos.y,worldTransform.position.x,worldTransform.position.y,mouseColliderFlag.w,mouseColliderFlag.h)) {
            EventDispatcher::getInstance().publish(ecs::getComponent<LeftMouseColliderEvents>(scene,entity)
                .value().pressed_event,param);
            if (mouseColliderFlag.block)break;
        }
    }
}

void MouseColliderSystem::leftMouseReleased(const std::any &param) {
}

void MouseColliderSystem::rightMousePressed(const std::any &param) {
}

void MouseColliderSystem::rightMouseReleased(const std::any &param) {
}


void MouseColliderSystem::middleMousePressed(const std::any &param) {
}

void MouseColliderSystem::middleMouseReleased(const std::any &param) {
}

void MouseColliderSystem::wheelMouse(const std::any &param) {
}
void MouseColliderSystem::mouseMoved(const std::any &param) {
    for (Entity entity:getSortedMouseColliders<MoveMouseColliderEvents>(scene)) {
        auto mouseColliderFlag=ecs::getComponent<MouseColliderFlag>(scene,entity).value();
        auto transformComp=ecs::getComponent<Transform>(scene,entity).value();
        auto worldTransform=TransformUtil::computeLocalToWorldTransform(transformComp,scene);
        auto pos=std::any_cast<MouseEventParam>(param);
        if (inRect(pos.x,pos.y,worldTransform.position.x,worldTransform.position.y,mouseColliderFlag.w,mouseColliderFlag.h)) {
            EventDispatcher::getInstance().publish(ecs::getComponent<MoveMouseColliderEvents>(scene,entity)
                .value().hover_event,param);
        }else {
            EventDispatcher::getInstance().publish(ecs::getComponent<MoveMouseColliderEvents>(scene,entity)
                .value().unhover_event,param);
        }
    }
}

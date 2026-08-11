//
// Created by XL0002 on 2026/7/21.
//

#include "MouseUISystem.h"

#include "../EventDispatcher.h"
#include "../ecs/BaseComponents.h"
#include "../ecs/Scene.h"
#include "../2d/transform2d/TransformUtil.h"
#include "../ecs/Util.h"
#include "SDL3/SDL_rect.h"

void MouseUISystem::start() {
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

static bool inRect(const float mx, const float my, const float x, const float y, const float w, const float h) {
    return mx >= x and mx <= x + w&&my >= y and my <= y + h;
}
// 点在三角形内（符号叉积法）
static bool pointInTriangle(const SDL_FPoint &p, const SDL_FPoint &a, const SDL_FPoint &b, const SDL_FPoint &c){
    auto sign = [](const SDL_FPoint &p1, const SDL_FPoint &p2, const SDL_FPoint &p3){
        return (p1.x - p3.x)*(p2.y - p3.y) - (p2.x - p3.x)*(p1.y - p3.y);
    };
    bool b1 = sign(p,a,b) < 0.0f;
    bool b2 = sign(p,b,c) < 0.0f;
    bool b3 = sign(p,c,a) < 0.0f;
    return (b1==b2) && (b2==b3);
}

// 点在多边形内（射线法）
static bool pointInPolygon(const SDL_FPoint &pt, const std::vector<SDL_FPoint> &poly){
    bool inside = false;
    for(size_t i=0,j=poly.size()-1;i<poly.size();j=i++){
        const auto& vi = poly[i];
        const auto& vj = poly[j];
        if ( ((vi.y > pt.y) != (vj.y > pt.y)) &&
             (pt.x < (vj.x-vi.x)*(pt.y-vi.y)/(vj.y-vi.y+1e-12f) + vi.x) )
            inside = !inside;
    }
    return inside;
}
void MouseUISystem::leftMousePressed(const std::any&  param) {
    for (Entity entity:getSortedMouseColliders<LeftMouseColliderEvents>(scene)) {
        auto mouseColliderFlag=ecs::getComponent<MouseColliderFlag>(scene,entity).value();
        auto transformComp=ecs::getComponent<TransformComp>(scene,entity).value();
        auto worldTransform=TransformUtil::computeLocalToWorldTransform(transformComp,scene);
        auto pos=std::any_cast<MouseEventParam>(param);
        if (inRect(pos.x,pos.y,worldTransform.position.x,worldTransform.position.y,mouseColliderFlag.w,mouseColliderFlag.h)) {
            EventDispatcher::getInstance().publish(ecs::getComponent<LeftMouseColliderEvents>(scene,entity)
                .value().pressed_event,param);
            if (mouseColliderFlag.block)break;
        }
    }
}

void MouseUISystem::leftMouseReleased(const std::any &param) {
}

void MouseUISystem::rightMousePressed(const std::any &param) {
}

void MouseUISystem::rightMouseReleased(const std::any &param) {
}


void MouseUISystem::middleMousePressed(const std::any &param) {
}

void MouseUISystem::middleMouseReleased(const std::any &param) {
}

void MouseUISystem::wheelMouse(const std::any &param) {
}
void MouseUISystem::mouseMoved(const std::any &param) {
    for (Entity entity:getSortedMouseColliders<MoveMouseColliderEvents>(scene)) {
        auto mouseColliderFlag=ecs::getComponent<MouseColliderFlag>(scene,entity).value();
        auto transformComp=ecs::getComponent<TransformComp>(scene,entity).value();
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

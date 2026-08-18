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
static std::vector<size_t> subscribeIds;
MouseUISystem::~MouseUISystem() {
    for (auto id:subscribeIds) {
        EventDispatcher::getInstance().unsubscribe(id);
    }
}

void MouseUISystem::start() {
    size_t id=EventDispatcher::getInstance().subscribe("left mouse pressed",[this](std::any param) {
        leftMousePressed(param);
    });
    subscribeIds.push_back(id);
    id=EventDispatcher::getInstance().subscribe("left mouse released",[this](std::any param) {
        leftMouseReleased(param);
    });
    subscribeIds.push_back(id);
    id=EventDispatcher::getInstance().subscribe("right mouse pressed",[this](std::any param) {
        rightMousePressed(param);
    });
    subscribeIds.push_back(id);
    id=EventDispatcher::getInstance().subscribe("right mouse released",[this](std::any param) {
        rightMouseReleased(param);
    });
    subscribeIds.push_back(id);
    id=EventDispatcher::getInstance().subscribe("middle mouse pressed",[this](std::any param) {
        middleMousePressed(param);
    });
    subscribeIds.push_back(id);
    id=EventDispatcher::getInstance().subscribe("middle mouse released",[this](std::any param) {
        middleMouseReleased(param);
    });
    subscribeIds.push_back(id);
    id=EventDispatcher::getInstance().subscribe("mouse wheeled",[this](std::any param) {
        wheelMouse(param);
    });
    subscribeIds.push_back(id);
    id=EventDispatcher::getInstance().subscribe("mouse moved",[this](std::any param) {
        mouseMoved(param);
    });
    subscribeIds.push_back(id);
}

static bool inRect(const float mx, const float my, const float x, const float y, const float w, const float h) {
    return mx >= x and mx <= x + w&&my >= y and my <= y + h;
}

//
/**
 * 判断点在多边形内，目前仅UI，因为没考虑取景和投影和视图变换
 * @param mousePos 鼠标屏幕坐标
 * @param worldTransform UI物体的世界坐标，就是屏幕坐标
 * @param vertices  UI物体内每个顶点的模型坐标
 * @return
 */
static bool pointInPolygon(const Position& mousePos,const Transform& worldTransform, const std::vector<Triangle>vertices){
    //正常算是三角形每个点转换到世界坐标，再跟鼠标点检测
    //这里反算，注意是反算到正交矩阵，而不是物体的local矩阵，相当于把物体拉到了原点，于是可以用顶点坐标直接和鼠标坐标比较了
    auto mouseTransform=Transform{mousePos,{},{}};
    glm::mat3 pm=TransformUtil::transformToMatrix(mouseTransform);
    glm::mat3 reverse_wm=TransformUtil::getReverseTransformToMatrix(worldTransform);
    glm::mat3 reverse_pm=reverse_wm*pm;//应用变换总是从右往左乘
    Transform reverse_pt=TransformUtil::matrixToTransform(reverse_pm);
    SDL_FPoint ppos={reverse_pt.position.x,reverse_pt.position.y};
    bool inside = false;
    for(const auto & vertice : vertices){
        if (vertice.contains(ppos)) {
            inside = true;
            break;
        }
    }
    return inside;
}
void MouseUISystem::leftMousePressed(const std::any&  param) {
    for (Entity entity:getSortedMouseColliders<LeftMouseColliderEvents>(scene)) {
        auto mouseColliderFlag=ecs::getComponent<MouseColliderFlag>(scene,entity).value();
        auto transformComp=ecs::getComponent<TransformComp>(scene,entity).value();
        auto worldTransform=TransformUtil::computeLocalToWorldTransform(transformComp,scene);
        auto pos=std::any_cast<MouseEventParam>(param);
        if (pointInPolygon({pos.x,pos.y},worldTransform,mouseColliderFlag.triangles)) {
            EventDispatcher::getInstance().publish(ecs::getComponent<LeftMouseColliderEvents>(scene,entity)
                .value().pressed_event,param);
            if (mouseColliderFlag.block)break;
        }
    }
}

void MouseUISystem::leftMouseReleased(const std::any &param) {
    for (Entity entity:getSortedMouseColliders<LeftMouseColliderEvents>(scene)) {
        auto mouseColliderFlag=ecs::getComponent<MouseColliderFlag>(scene,entity).value();
        auto transformComp=ecs::getComponent<TransformComp>(scene,entity).value();
        auto worldTransform=TransformUtil::computeLocalToWorldTransform(transformComp,scene);
        //world to screen
        auto pos=std::any_cast<MouseEventParam>(param);
        if (pointInPolygon({pos.x,pos.y},worldTransform,mouseColliderFlag.triangles)) {
            EventDispatcher::getInstance().publish(ecs::getComponent<LeftMouseColliderEvents>(scene,entity)
                .value().clicked_event,param);
        }
    }
}

void MouseUISystem::rightMousePressed(const std::any &param) {
    for (Entity entity:getSortedMouseColliders<MoveMouseColliderEvents>(scene)) {
        auto mouseColliderFlag=ecs::getComponent<MouseColliderFlag>(scene,entity).value();
        auto transformComp=ecs::getComponent<TransformComp>(scene,entity).value();
        auto worldTransform=TransformUtil::computeLocalToWorldTransform(transformComp,scene);
        //world to screen
        auto pos=std::any_cast<MouseEventParam>(param);
        if (pointInPolygon({pos.x,pos.y},worldTransform,mouseColliderFlag.triangles)) {
            EventDispatcher::getInstance().publish(ecs::getComponent<MoveMouseColliderEvents>(scene,entity)
                .value().hover_event,param);
        }else {
            EventDispatcher::getInstance().publish(ecs::getComponent<MoveMouseColliderEvents>(scene,entity)
                .value().unhover_event,param);
        }
    }
}

void MouseUISystem::rightMouseReleased(const std::any &param) {
    for (Entity entity:getSortedMouseColliders<MoveMouseColliderEvents>(scene)) {
        auto mouseColliderFlag=ecs::getComponent<MouseColliderFlag>(scene,entity).value();
        auto transformComp=ecs::getComponent<TransformComp>(scene,entity).value();
        auto worldTransform=TransformUtil::computeLocalToWorldTransform(transformComp,scene);
        //world to screen
        auto pos=std::any_cast<MouseEventParam>(param);
        if (pointInPolygon({pos.x,pos.y},worldTransform,mouseColliderFlag.triangles)) {
            EventDispatcher::getInstance().publish(ecs::getComponent<MoveMouseColliderEvents>(scene,entity)
                .value().hover_event,param);
        }else {
            EventDispatcher::getInstance().publish(ecs::getComponent<MoveMouseColliderEvents>(scene,entity)
                .value().unhover_event,param);
        }
    }
}


void MouseUISystem::middleMousePressed(const std::any &param) {
}

void MouseUISystem::middleMouseReleased(const std::any &param) {
}

void MouseUISystem::wheelMouse(const std::any &param) {
    for (Entity entity:getSortedMouseColliders<WheelMouseColliderEvents>(scene)) {
        auto mouseColliderFlag=ecs::getComponent<MouseColliderFlag>(scene,entity).value();
        auto transformComp=ecs::getComponent<TransformComp>(scene,entity).value();
        auto worldTransform=TransformUtil::computeLocalToWorldTransform(transformComp,scene);
        //world to screen
        auto pos=std::any_cast<MouseEventParam>(param);
        if (pointInPolygon({pos.x,pos.y},worldTransform,mouseColliderFlag.triangles)) {
            EventDispatcher::getInstance().publish(ecs::getComponent<WheelMouseColliderEvents>(scene,entity)
                .value().scroll_event,param);
        }
    }
}
void MouseUISystem::mouseMoved(const std::any &param) {
    for (Entity entity:getSortedMouseColliders<MoveMouseColliderEvents>(scene)) {
        auto mouseColliderFlag=ecs::getComponent<MouseColliderFlag>(scene,entity).value();
        auto transformComp=ecs::getComponent<TransformComp>(scene,entity).value();
        auto worldTransform=TransformUtil::computeLocalToWorldTransform(transformComp,scene);
        //world to screen
        auto pos=std::any_cast<MouseEventParam>(param);
        if (pointInPolygon({pos.x,pos.y},worldTransform,mouseColliderFlag.triangles)) {
            EventDispatcher::getInstance().publish(ecs::getComponent<MoveMouseColliderEvents>(scene,entity)
                .value().hover_event,param);
        }else {
            EventDispatcher::getInstance().publish(ecs::getComponent<MoveMouseColliderEvents>(scene,entity)
                .value().unhover_event,param);
        }
    }
}

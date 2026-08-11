//
// Created by XL0002 on 2026/7/28.
//

#include "CameraInputListenerSystem.h"

#include <cassert>

#include "../../EventDispatcher.h"
#include "../../2d/transform2d/Transform2dComponents.h"
#include "../../2d/transform2d/TransformUtil.h"
#include "SDL3/SDL_events.h"

void CameraInputListenerSystem::onStart() {
    EventDispatcher::getInstance().subscribe("mouse wheeled",[this](std::any param) {
        auto eventInfo=std::any_cast<SDL_MouseWheelEvent>(param);
        auto cameraComp = ecs::getComponent<CameraComp>(scene,curEntity).value();
        auto cameraTransformComp=ecs::getComponent<TransformComp>(scene,curEntity).value();
        //获取鼠标指向的世界坐标
        Position preMouseWorldPos=TransformUtil::screenToWorldPosition({(eventInfo.mouse_x),(eventInfo.mouse_y)},cameraTransformComp,cameraComp);
        //通常上滑滚轮是为了放大，即缩小视口
        cameraComp.viewportWidth-=eventInfo.y*10;
        cameraComp.viewportHeight-=eventInfo.y*10;
        ecs::setComponent<CameraComp>(scene,curEntity, cameraComp);
        //修改完宽高后再求一次
        Position laterMouseWorldPos=TransformUtil::screenToWorldPosition({(eventInfo.mouse_x),(eventInfo.mouse_y)},cameraTransformComp,cameraComp);
        //对齐差值
        glm::vec2 offset={laterMouseWorldPos.x-preMouseWorldPos.x,laterMouseWorldPos.y-preMouseWorldPos.y};
        cameraTransformComp.transform.position.x-=offset.x;
        cameraTransformComp.transform.position.y-=offset.y;
        ecs::setComponent<TransformComp>(scene,curEntity, cameraTransformComp);
    });
    EventDispatcher::getInstance().subscribe("key w",[this](std::any param) {
        assert(ecs::getComponent<TransformComp>(scene,curEntity).has_value());
        auto transformComp = ecs::getComponent<TransformComp>(scene,curEntity).value();
        transformComp.transform.position.y-=100;//左上角是原点，所以-是向上
        ecs::setComponent<TransformComp>(scene,curEntity, transformComp);
    });
    EventDispatcher::getInstance().subscribe("key s",[this](std::any param) {
        assert(ecs::getComponent<TransformComp>(scene,curEntity).has_value());
        auto transformComp = ecs::getComponent<TransformComp>(scene,curEntity).value();
        transformComp.transform.position.y+=100;//左上角是原点，所以+是向下
        ecs::setComponent<TransformComp>(scene,curEntity, transformComp);
    });
    EventDispatcher::getInstance().subscribe("key a",[this](std::any param) {
        assert(ecs::getComponent<TransformComp>(scene,curEntity).has_value());
        auto transformComp = ecs::getComponent<TransformComp>(scene,curEntity).value();
        transformComp.transform.position.x-=100;//左上角是原点，所以-是向左
        ecs::setComponent<TransformComp>(scene,curEntity, transformComp);
    });
    EventDispatcher::getInstance().subscribe("key d",[this](std::any param) {
        assert(ecs::getComponent<TransformComp>(scene,curEntity).has_value());
        auto transformComp = ecs::getComponent<TransformComp>(scene,curEntity).value();
        transformComp.transform.position.x+=100;//左上角是原点，所以+是向右
        ecs::setComponent<TransformComp>(scene,curEntity, transformComp);
    });
    EventDispatcher::getInstance().subscribe("key q",[this](std::any param) {
        auto transformComp = ecs::getComponent<TransformComp>(scene,curEntity).value();
        transformComp.transform.rotation.angle-=10;//逆时针
        ecs::setComponent<TransformComp>(scene,curEntity, transformComp);
    });
    EventDispatcher::getInstance().subscribe("key e",[this](std::any param) {
        auto transformComp = ecs::getComponent<TransformComp>(scene,curEntity).value();
        transformComp.transform.rotation.angle+=10;//顺时针
        ecs::setComponent<TransformComp>(scene,curEntity, transformComp);
    });
}

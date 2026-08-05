//
// Created by XL0002 on 2026/7/28.
//

#include "CameraInputListenerSystem.h"

#include <cassert>

#include "../EventDispatcher.h"
#include "../2d/transform2d/Transform2dComponents.h"
void CameraInputListenerSystem::onStart() {
    EventDispatcher::getInstance().subscribe("mouse wheeled",[this](std::any param) {
        auto cameraComp = ecs::getComponent<CameraComp>(scene,curEntity).value();
        auto y=std::any_cast<float>(param);
        //通常上滑滚轮是为了放大，即缩小视口
        cameraComp.viewportWidth-=y;
        cameraComp.viewportHeight-=y;
        assert(ecs::getComponent<TransformComp>(scene,curEntity).has_value());
        auto transformComp = ecs::getComponent<TransformComp>(scene,curEntity).value();
        transformComp.transform.scale.x-=y/100;
        transformComp.transform.scale.y-=y/100;
        ecs::setComponent<TransformComp>(scene,curEntity, transformComp);
        ecs::setComponent<CameraComp>(scene,curEntity, cameraComp);
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
        transformComp.transform.rotation.angle-=10;//左上角是原点，所以-是逆时针
        ecs::setComponent<TransformComp>(scene,curEntity, transformComp);
    });
    EventDispatcher::getInstance().subscribe("key e",[this](std::any param) {
        auto transformComp = ecs::getComponent<TransformComp>(scene,curEntity).value();
        transformComp.transform.rotation.angle+=10;//左上角是原点，所以+是顺时针
        ecs::setComponent<TransformComp>(scene,curEntity, transformComp);
    });
}

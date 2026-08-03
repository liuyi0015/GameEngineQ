//
// Created by XL0002 on 2026/7/28.
//

#include "CameraInputListenerSystem.h"

#include "../EventDispatcher.h"
#include "../2d/transform2d/Transform2dComponents.h"
void CameraInputListenerSystem::onStart() {
    EventDispatcher::getInstance().subscribe("mouse wheeled",[this](std::any param) {
        auto cameraComp = ecs::getComponent<CameraComp>(scene,curEntity).value();
        auto y=std::any_cast<float>(param);
        cameraComp.viewportWidth+=y;
        cameraComp.viewportHeight+=y;
        auto transformComp = ecs::getComponent<Transform>(scene,curEntity).value();
        transformComp.scale.x+=y/100;
        transformComp.scale.y+=y/100;
        ecs::setComponent<Transform>(scene,curEntity, transformComp);
        ecs::setComponent<CameraComp>(scene,curEntity, cameraComp);
    });
    EventDispatcher::getInstance().subscribe("key w",[this](std::any param) {
        auto transformComp = ecs::getComponent<Transform>(scene,curEntity).value();
        transformComp.position.y+=100;
        ecs::setComponent<Transform>(scene,curEntity, transformComp);
    });
    EventDispatcher::getInstance().subscribe("key s",[this](std::any param) {
        auto transformComp = ecs::getComponent<Transform>(scene,curEntity).value();
        transformComp.position.y-=100;
        ecs::setComponent<Transform>(scene,curEntity, transformComp);
    });
    EventDispatcher::getInstance().subscribe("key a",[this](std::any param) {
        auto transformComp = ecs::getComponent<Transform>(scene,curEntity).value();
        transformComp.position.x-=100;
        ecs::setComponent<Transform>(scene,curEntity, transformComp);
    });
    EventDispatcher::getInstance().subscribe("key d",[this](std::any param) {
        auto transformComp = ecs::getComponent<Transform>(scene,curEntity).value();
        transformComp.position.x+=100;
        ecs::setComponent<Transform>(scene,curEntity, transformComp);
    });
}

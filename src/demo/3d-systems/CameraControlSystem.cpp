//
// Created by XL0002 on 2026/10/13.
//

#include "CameraControlSystem.h"

#include "SDL3/SDL_events.h"
#include "../../core/EventBus.h"
#include "../../ecs/Util.h"
#include "../../transform/transform3d/Transform3dComponents.h"
CameraControlSystem::CameraControlSystem() {
    motionSubscribeId=EventBus::getInstance().subscribe("input.mousemotion", [this](std::any param) {
        SDL_MouseMotionEvent event = std::any_cast<const SDL_MouseMotionEvent&>(param);
        rotateCamera(event);
    });
    keyboardSubscribeId=EventBus::getInstance().subscribe("input.keyboard", [this](std::any param) {
        SDL_KeyboardEvent event = std::any_cast<const SDL_KeyboardEvent&>(param);
        forwardCamera(event);
    });
}
CameraControlSystem::~CameraControlSystem() {
    EventBus::getInstance().unsubscribe(motionSubscribeId);
    EventBus::getInstance().unsubscribe(keyboardSubscribeId);
}

void CameraControlSystem::rotateCamera(const SDL_MouseMotionEvent& event) {
    //根据鼠标移动更新摄像机的朝向
    //这里可以使用摄像机的旋转矩阵或者四元数来更新朝向
    //假设我们有一个摄像机对象camera，它有一个方法rotate(dx, dy)来旋转摄像机
    float sensitivity = 0.1f; // 鼠标灵敏度
    float dx = event.xrel * sensitivity;
    float dy = event.yrel * sensitivity;

    // 更新摄像机的朝向
    ecs::getComponent<Transform3DComp>(scene, camera)->transform.rotation.x += dy;
    ecs::getComponent<Transform3DComp>(scene, camera)->transform.rotation.y += dx;
}
void CameraControlSystem::forwardCamera(const SDL_KeyboardEvent& event) {
    float speed = 100.0f;
    // 根据键盘事件更新摄像机位置
    if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key == SDLK_W) {
            ecs::getComponent<Transform3DComp>(scene, camera)->transform.position.z -= speed;
        } else if (event.key == SDLK_S) {
            ecs::getComponent<Transform3DComp>(scene, camera)->transform.position.z += speed;
        } else if (event.key == SDLK_A) {
            ecs::getComponent<Transform3DComp>(scene, camera)->transform.position.x -= speed;
        } else if (event.key == SDLK_D) {
            ecs::getComponent<Transform3DComp>(scene, camera)->transform.position.x += speed;
        }
    }
}
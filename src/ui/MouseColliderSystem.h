//
// Created by XL0002 on 2026/7/21.
//

#ifndef GAMEENGINE_MOUSECOLLIDERSYSTEM_H
#define GAMEENGINE_MOUSECOLLIDERSYSTEM_H
#include <any>
#include <string>
#include <vector>

#include "../ecs/System.h"
#include "../ecs/Scene.h"
//响应事件不是输入事件
struct LeftMouseColliderEvents {
    std::string pressed_event;
    std::string released_event;
    std::string clicked_event;
};
struct RightMouseColliderEvents {
    std::string pressed_event;
    std::string released_event;
    std::string clicked_event;
};
struct WheelMouseColliderEvents {
    std::string up_event;
    std::string down_event;
    std::string pressed_event;
    std::string released_event;
    std::string clicked_event;
};
struct MoveMouseColliderEvents {
    std::string move_event;
    std::string drag_event;
};
struct MouseEventParam {
    float x,y;
};
struct MouseColliderFlag {
    float w;
    float h;
    int order;
    bool block;
};

template<typename T>
static std::vector<Entity> getSortedMouseColliders(const std::shared_ptr<Scene>&scene) {
    auto entities = std::vector<Entity>();
    for (auto entity:scene->getEntities()) {
        if (scene->getComponent<MouseColliderFlag>(entity).has_value()&&scene->getComponent<T>(entity).has_value()) {
            entities.push_back(entity);
        }
    }
    return entities;
}
class MouseColliderSystem :public ecs::System{
public:
    explicit MouseColliderSystem(const std::shared_ptr<Scene> &scene)
        : System(scene){
    }
    void start() override;

    void leftMousePressed(const std::any& param);
    void leftMouseReleased(const std::any& param);
    void leftMouseClicked(const std::any& param);
    void rightMousePressed(const std::any& param);
    void rightMouseReleased(const std::any& param);
    void rightMouseClicked(const std::any& param);
    void wheelMouesUp(const std::any& param);
    void wheelMouseDown(const std::any& param);
    void wheelMousePressed(const std::any& any);
    void wheelMouseReleased(const std::any& any);
    void wheelMouseClicked(const std::any& any);
    void mouseMoved(const std::any& param);
    void mouseDragged(const std::any& param);

    void update(double deltaTime) override{};
    void fixed_update(double deltaTime) override{};
    void draw() override{};

};


#endif //GAMEENGINE_MOUSECOLLIDERSYSTEM_H

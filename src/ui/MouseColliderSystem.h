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
#include "UIComponents.h"
#include "../ecs/Util.h"
template<typename EventType>
static std::vector<Entity> getSortedMouseColliders(const std::shared_ptr<Scene>&scene) {
    auto entities = std::vector<Entity>();
    for (auto entity : ecs::getEntities<EventType>(scene)) {
        if (ecs::getComponent<MouseColliderFlag>(scene,entity).has_value()&&ecs::getComponent<EventType>(scene,entity).has_value()) {
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
    void rightMousePressed(const std::any& param);
    void rightMouseReleased(const std::any& param);
    void middleMousePressed(const std::any& param);
    void middleMouseReleased(const std::any& param);
    void wheelMouse(const std::any& param);
    void mouseMoved(const std::any& param);

    void update(double deltaTime) override{}

    void fixed_update(double deltaTime) override{};
    void draw() override{};

};


#endif //GAMEENGINE_MOUSECOLLIDERSYSTEM_H

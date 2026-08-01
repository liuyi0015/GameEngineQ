//
// Created by XL0002 on 2026/7/16.
//

#ifndef GAMEENGINE_APPLICATION_H
#define GAMEENGINE_APPLICATION_H
#include <algorithm>
#include <iostream>
#include <memory>

#include "Context.hpp"
#include "EventDispatcher.h"
#include "ecs/Scene.h"
#include "ecs/System.h"


struct SystemComparer{
    bool operator()(const SystemPhaseOrder& a, const SystemPhaseOrder& b) const {
        return a.second < b.second;
    }
};
class EcsApplication {
protected:
    std::shared_ptr<Scene > scene;
    std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;
    std::vector<SystemPhaseOrder> global_systems;
public:
    EcsApplication() {
        scene=std::make_shared<Scene>();
    }
    virtual ~EcsApplication() = default;

    virtual void init() {
        EventDispatcher::getInstance().subscribe("switch_scene",[this](std::any param){
            auto newSceneName=std::any_cast<std::string>(param);
            if (scenes.find(newSceneName) != scenes.end()) {
                scene=scenes[newSceneName];
                for (const auto& gSystem:global_systems) {
                    gSystem.first->scene=scene;
                }
                start();
            }
        },false,false);
        std::sort(global_systems.begin(), global_systems.end(), SystemComparer());
        for (const auto& [a,scene]:scenes) {
            std::sort(scene->system_start_orders.begin(), scene->system_start_orders.end(), SystemComparer());
            std::sort(scene->system_update_orders.begin(), scene->system_update_orders.end(), SystemComparer());
            std::sort(scene->system_fixed_update_orders.begin(), scene->system_fixed_update_orders.end(), SystemComparer());
            std::sort(scene->system_draw_orders.begin(), scene->system_draw_orders.end(), SystemComparer());
        }
    }
    void start() {
        for (const auto& global_system:global_systems) {
            global_system.first->start();
        }
        for (const auto& systemOrder:scene->system_start_orders) {
            systemOrder.first->start();
        }
    }
    void update(double deltaTime) {
        for (const auto& global_system:global_systems) {
            global_system.first->update(deltaTime);
        }
        for (const auto& systemOrder:scene->system_update_orders) {
            systemOrder.first->update(deltaTime);
        }
    }
    void fixed_update(double deltaTime) {
        for (const auto& global_system:global_systems) {
            global_system.first->fixed_update(deltaTime);
        }
        for (const auto& systemOrder:scene->system_fixed_update_orders) {
            systemOrder.first->fixed_update(deltaTime);
        }
    }
    void draw() {
        for (const auto& global_system:global_systems) {
            global_system.first->draw();
        }
        for (const auto& systemOrder:scene->system_draw_orders) {

            systemOrder.first->draw();
        }
    }
};

#endif //GAMEENGINE_APPLICATION_H

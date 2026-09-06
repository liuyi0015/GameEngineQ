//
// Created by XL0002 on 2026/7/16.
//

#ifndef GAMEENGINE_APPLICATION_H
#define GAMEENGINE_APPLICATION_H
#include <algorithm>
#include <iostream>
#include <memory>

#include "Context.hpp"
#include "EventBus.h"
#include "../demo/render-blueprint/RenderCompositorSystem.h"
#include "ecs/Scene.h"
#include "ecs/System.h"
#include "ecs/Util.h"
#include "SDL3/SDL_log.h"

struct SystemComparer{
    bool operator()(const SystemOrder& a, const SystemOrder& b) const {
        return a.second < b.second;
    }
};
//可以直接用在main里，相当于空应用
class EcsApplication {
protected:
    ecs::Scene* scene;
public:
    virtual ~EcsApplication() = default;
    virtual void init() {}
    void start() {
        std::sort(scene->system_start_orders.begin(), scene->system_start_orders.end(), SystemComparer());
        std::sort(scene->system_update_orders.begin(), scene->system_update_orders.end(), SystemComparer());
        std::sort(scene->system_fixed_update_orders.begin(), scene->system_fixed_update_orders.end(), SystemComparer());
        std::sort(scene->system_draw_orders.begin(), scene->system_draw_orders.end(), SystemComparer());

        for (const auto& systemOrder:scene->system_start_orders) {
            auto system=ecs::getSystemById(scene,systemOrder.first);
            system->start();
        }
    }
    void update(double deltaTime) {
        for (const auto& systemOrder:scene->system_update_orders) {
            auto system=ecs::getSystemById(scene,systemOrder.first);
            system->update(deltaTime);
        }
    }
    void fixed_update(double deltaTime) {
        for (const auto& systemOrder:scene->system_fixed_update_orders) {
            auto system=ecs::getSystemById(scene,systemOrder.first);
            system->fixed_update(deltaTime);
        }
    }
    void draw() {
        for (const auto& systemOrder:scene->system_draw_orders) {
            auto system=ecs::getSystemById(scene,systemOrder.first);
            system->draw();
        }
    }
};

#endif //GAMEENGINE_APPLICATION_H

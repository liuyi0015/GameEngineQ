//
// Created by XL0002 on 2026/7/16.
//

#ifndef GAMEENGINE_APPLICATION_H
#define GAMEENGINE_APPLICATION_H
#include <algorithm>
#include <iostream>
#include <memory>

#include "Scene.h"
#include "System.h"
#include "Util.h"
#include "../core/Application.h"

struct SystemComparer{
    bool operator()(const SystemOrder& a, const SystemOrder& b) const {
        return a.second < b.second;
    }
};
//可以直接用在main里，相当于空应用
class EcsApplication:public Application{
protected:
    ecs::Scene* scene=nullptr;
public:
    virtual ~EcsApplication() {
        delete scene;
    };
    //默认实现空，会被继承者覆盖
    void init()override {
        scene=new ecs::Scene();
    }
    void start() override {
        std::sort(scene->system_start_orders.begin(), scene->system_start_orders.end(), SystemComparer());
        std::sort(scene->system_fixed_update_orders.begin(), scene->system_fixed_update_orders.end(), SystemComparer());
        std::sort(scene->system_update_orders.begin(), scene->system_update_orders.end(), SystemComparer());
        std::sort(scene->system_draw_orders.begin(), scene->system_draw_orders.end(), SystemComparer());

        for (const auto& systemOrder:scene->system_start_orders) {
            auto system=ecs::getSystemById(scene,systemOrder.first);
            system->start();
        }
    }
    void fixed_update(double deltaTime)override {
        for (const auto& systemOrder:scene->system_fixed_update_orders) {
            auto system=ecs::getSystemById(scene,systemOrder.first);
            system->fixed_update(deltaTime);
        }
    }
    void update(double deltaTime) override{
        for (const auto& systemOrder:scene->system_update_orders) {
            auto system=ecs::getSystemById(scene,systemOrder.first);
            system->update(deltaTime);
        }
    }
    void draw() override {
        for (const auto& systemOrder:scene->system_draw_orders) {
            auto system=ecs::getSystemById(scene,systemOrder.first);
            system->draw();
        }
    }
};

#endif //GAMEENGINE_APPLICATION_H

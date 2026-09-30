//
// Created by XL0002 on 2026/7/15.
//

#include "Scene.h"

#include <algorithm>

struct SystemComparer{
    bool operator()(const SystemOrder& a, const SystemOrder& b) const {
        return a.second < b.second;
    }
};
ecs::Scene::~Scene() {
    for (const auto& systemOrder:system_end_orders) {
        auto system=systemIds[systemOrder.first];
        system->end();
    }
}
void ecs::Scene::init() {

    std::sort(system_start_orders.begin(), system_start_orders.end(), SystemComparer());
    std::sort(system_end_orders.begin(), system_end_orders.end(), SystemComparer());
    std::sort(system_fixed_update_orders.begin(), system_fixed_update_orders.end(), SystemComparer());
    std::sort(system_update_orders.begin(), system_update_orders.end(), SystemComparer());
    std::sort(system_draw_orders.begin(), system_draw_orders.end(), SystemComparer());

    start();
}
void ecs::Scene::start(){
    for (const auto& systemOrder:system_start_orders) {
        auto system=systemIds[systemOrder.first];
        system->start();
    }
}
void ecs::Scene::fixed_update(double deltaTime) {
    for (const auto& systemOrder:system_fixed_update_orders) {
        auto system=systemIds[systemOrder.first];
        system->fixed_update(deltaTime);
    }
}
void ecs::Scene::update(double deltaTime){
    for (const auto& systemOrder:system_update_orders) {
        auto system=systemIds[systemOrder.first];
        system->update(deltaTime);
    }
}
void ecs::Scene::draw() {
    for (const auto& systemOrder:system_draw_orders) {
        auto system=systemIds[systemOrder.first];
        system->draw();
    }
}
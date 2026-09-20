//
// Created by XL0002 on 2026/7/29.
//

#ifndef GAMEENGINE_UTIL_H
#define GAMEENGINE_UTIL_H
#include <optional>

#include "BaseComponents.h"
#include "Scene.h"

namespace ecs{

    static Entity createEntity(Scene* scene) {
        Entity entity = scene->nextEntityId++;
        return entity;
    }
    template <typename T>
    static std::optional<T> getComponent(Scene* scene, const Entity entity) {
        auto typeIt = scene->ce_storage.find(typeid(T));
        if (typeIt == scene->ce_storage.end()) {
            // std::cout<<entity<<"no component:"<<typeid(T).name()<<std::endl;
            return std::nullopt;
        }

        auto entityIt = typeIt->second.find(entity);
        if (entityIt == typeIt->second.end()) {
            // std::cout<<typeid(T).name()<<"no entity:"<<entity<<std::endl;
            // std::cout<<"（组件里找不到就是实体没有该组件）"<<std::endl;
            return std::nullopt;
        }

        try {
            return std::any_cast<T>(entityIt->second);
        } catch (const std::bad_any_cast&  err) {
            std::cerr<<"bad_any_cast"<<err.what()<<std::endl;
            return std::nullopt;
        }
    }
    template <typename T>
    static bool setComponent(Scene* scene, const Entity entity, const T& component) {
        scene->ce_storage[typeid(T)][entity] = component;
        return true;
    }
    static std::vector<Entity> getEntities(Scene* scene) {
        // todo ?临时变量作用域结束被回收了吧
        std::unordered_set<Entity> entitySet;
        for (auto& [typeId, ceMap] : scene->ce_storage) {
            //所有带该组件的实体
            for (auto& [entity, component] : ceMap) {
                entitySet.insert(entity);
            }
        }
        return std::vector(entitySet.begin(), entitySet.end());
    }
    template<typename ComponentType>
    static std::vector<Entity> getEntities(Scene* scene) {
        // todo ?临时变量作用域结束被回收了吧
        std::unordered_set<Entity> entitySet;
          for (auto& [entity, componentVal] : scene->ce_storage[typeid(ComponentType)]) {
              entitySet.insert(entity);
        }
        return std::vector(entitySet.begin(), entitySet.end());
    }
    //
    template<typename ComponentType>
    static std::vector<Entity> searchEntity(Scene* scene) {
        std::vector<Entity> entities;
        for (const auto& [entity, component] : scene->ce_storage[typeid(ComponentType)]) {
            entities.push_back(entity);
        }
        return entities;
    }

    struct ExecutionOrders {
        int start_order=0;
        int update_order=0;
        int fixed_update_order=0;
        int draw_order=0;
    };
    static SystemId addSystem(Scene* scene, System* system, const ExecutionOrders& orders) {
        SystemId si=scene->nextSystemId++;
        scene->systemIds[si]=std::shared_ptr<System>(system);
        scene->system_start_orders.push_back({si, orders.start_order});
        scene->system_update_orders.push_back({si, orders.update_order});
        scene->system_fixed_update_orders.push_back({si, orders.fixed_update_order});
        scene->system_draw_orders.push_back({si, orders.draw_order});
        return si;
    }
    static System* getSystemById(Scene* scene, const SystemId& systemId) {
        return scene->systemIds[systemId].get();
    }
}

#endif //GAMEENGINE_UTIL_H

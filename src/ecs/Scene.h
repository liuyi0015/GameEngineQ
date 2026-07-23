//
// Created by XL0002 on 2026/7/15.
//

#ifndef OPENVISUALNOVEL_SCENE_H
#define OPENVISUALNOVEL_SCENE_H
#include <any>
#include <iostream>
#include <optional>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "System.h"

using Entity=long;

class Scene {
private:
    Entity nextEntityId = 0;
    std::unordered_map<std::type_index,std::unordered_map<Entity,std::any>> ce_storage;
    // std::unordered_map<Entity,std::any>ec_storage;//反向索引加速查询实体有哪些组件
public:
    std::vector<std::shared_ptr< ecs::System> > systems;
    Entity createEntity() {
        Entity entity = nextEntityId++;
        return entity;
    }
    template <typename T>
    std::optional<T> getComponent(const Entity entity) {
        auto typeIt = ce_storage.find(typeid(T));
        if (typeIt == ce_storage.end()) {
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
    bool setComponent(const Entity entity,const T& component) {
        ce_storage[typeid(T)][entity]=component;
        return true;
    }
    std::vector<Entity> getEntities() {
        // todo ?临时变量作用域结束被回收了吧
        std::unordered_set<Entity> entitySet;
        for (auto& [typeId, ceMap] : ce_storage) {
            //所有带该组件的实体
            for (auto& [entity, component] : ceMap) {
                entitySet.insert(entity);
            }
        }
        return std::vector(entitySet.begin(), entitySet.end());
    }
};


#endif //OPENVISUALNOVEL_SCENE_H

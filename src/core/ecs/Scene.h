//
// Created by XL0002 on 2026/7/15.
//

#ifndef OPENVISUALNOVEL_SCENE_H
#define OPENVISUALNOVEL_SCENE_H
#include <any>
#include <iostream>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "Entity.h"
#include "System.h"
#include "SDL3/SDL_render.h"
using SystemId=size_t;
using WorldId=size_t;
using SystemOrder=std::pair<SystemId,int>;
namespace ecs {
    class Scene {
    public:
        Entity nextEntityId = 0;
        SystemId nextSystemId = 0;
        WorldId nextWorldId = 0;
        std::unordered_map<std::type_index,std::unordered_map<Entity,std::any>> ce_storage;
        // std::unordered_map<Entity,std::any>ec_storage;//反向索引加速查询实体有哪些组件

        //使用shared_ptr切换场景自动释放system
        std::unordered_map<SystemId,std::shared_ptr<ecs::System>> systemIds;

        std::vector<SystemOrder> system_start_orders;
        std::vector<SystemOrder> system_update_orders;
        std::vector<SystemOrder> system_fixed_update_orders;
        std::vector<SystemOrder> system_draw_orders;
    };

}

#endif //OPENVISUALNOVEL_SCENE_H

//
// Created by XL0002 on 2026/7/15.
//

#ifndef OPENVISUALNOVEL_SCENE_H
#define OPENVISUALNOVEL_SCENE_H
#include <any>
#include <iostream>
#include <optional>
#include <queue>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "System.h"

using Entity=long;

using SystemPhaseOrder=std::pair<std::shared_ptr<ecs::System>,int>;

class Scene {
public:
    Entity nextEntityId = 0;
    std::unordered_map<std::type_index,std::unordered_map<Entity,std::any>> ce_storage;
    // std::unordered_map<Entity,std::any>ec_storage;//反向索引加速查询实体有哪些组件
    std::vector<SystemPhaseOrder> system_start_orders;
    std::vector<SystemPhaseOrder> system_update_orders;
    std::vector<SystemPhaseOrder> system_fixed_update_orders;
    std::vector<SystemPhaseOrder> system_draw_orders;

};


#endif //OPENVISUALNOVEL_SCENE_H
